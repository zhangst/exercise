/**
 * 服务器包含一个父进程、多个子进程。父进程创建共享内存，子进程使用共享内存。
 * 父进程定时输出共享内存内容，子进程accept连接，并更新共享内存。
 * 根据IP、Port区分统计客户端访问量
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CC_OK 0
#define CC_ERROR 1
#define CC_NODENUM 20
#define CC_INET64_LEN   sizeof("-9223372036854775808") - 1

struct cc_count {
	int node_num;
	char data[1];
};

struct cc_count_node {
	uint32_t ip;		/* 网络字节序 */
	uint16_t port;		/* 网络字节序 */
	uint16_t num;		/* 访问次数 */
	time_t last_time;	/* 最后更新时间 */
};

struct cc_shm {
	char *addr;
	size_t size;
};

/**
 * 共享内存接口：创建、注销
 */
void* cc_shm_alloc(size_t size)
{
	void *shm_addr = NULL;
	
	shm_addr = mmap(NULL, size,
			PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_SHARED, -1, 0);
	if (shm_addr == MAP_FAILED) {
		fprintf(stderr,
			"mmap(MAP_ANON | MAP_SHARED, %uz) failed.\n",
			size);
		return NULL;
	}

	return shm_addr;
}
void cc_shm_free(void *shm_addr, size_t size)
{
	if (munmap(shm_addr, size) == -1) {
		fprintf(stderr, "munmap(%p, %uz) failed.\n",
			shm_addr, size);
	}
}


/**
 * 记录锁接口：创建、注销、加锁、解锁
 */
static int cc_pidfile_create(const char *file)
{
	int fd = -1;
	char pid[CC_INET64_LEN + 2] = {0};

	fd = open(file, O_RDWR | O_CREAT | O_TRUNC,
		  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1) {
		fprintf(stderr, "open %s failed.\n", file);
		return CC_ERROR;
	}

	snprintf(pid, sizeof(pid), "%d\n", getpid());
	if (write(fd, pid, strlen(pid)) == -1) {
		fprintf(stderr, "write %s failed.\n", file);
		return CC_ERROR;
	}

	if (close(fd) == -1)
		fprintf(stderr, "close %s failed.\n", file);

	return CC_OK;
}

static void cc_pidfile_delete(const char *file)
{
	if (unlink(file) == -1)
		fprintf(stderr, "unlink %s failed.\n", file);
}

#define CC_PIDFILE "/tmp/shm.pid"
static int cc_shmtx_mutex = -1;
int cc_shmtx_create(void)
{
	int ret = 0;
	
	ret = cc_pidfile_create(CC_PIDFILE);
	if (ret != CC_OK)
		return CC_ERROR;
	
	cc_shmtx_mutex = open(CC_PIDFILE, O_RDWR);
	if (cc_shmtx_mutex == -1) {
		fprintf(stderr, "open %s failed.\n",
			CC_PIDFILE);
		cc_pidfile_delete(CC_PIDFILE);
		return CC_ERROR;
	}
	
	return CC_OK;
}
void cc_shmtx_destory(void)
{
	if (cc_shmtx_mutex >= 0) {
		close(cc_shmtx_mutex);
	}
	cc_pidfile_delete(CC_PIDFILE);
	
}
int cc_shmtx_lock(void)
{
	struct flock fl;
	int ret = 0;
	
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;

	while (1) {
		ret = fcntl(cc_shmtx_mutex, F_SETLKW, &fl);
		if (ret == -1) {
			if (errno == EINTR)
				continue;
			else
				return CC_ERROR;
		} else {
			return CC_OK;
		}
	}
}
int cc_shmtx_unlock(void)
{
	struct flock fl;
	int ret = 0;
	
	fl.l_type = F_UNLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;

	while (1) {
		ret = fcntl(cc_shmtx_mutex, F_SETLK, &fl);
		if (ret == -1) {
			if (errno == EINTR)
				continue;
			else
				return CC_ERROR;
		} else {
			return CC_OK;
		}
	}
}

/**
 * 初始化：对共享内存中存储的结构体节点赋初始值
 * 添加：填充一个节点
 */
static void sshm_node_init(struct cc_shm *shm)
{
	int i = 0;
	struct cc_count *mi = NULL;
	struct cc_count_node *node = NULL;

	mi = (struct cc_count *)shm->addr;
	node = (struct cc_count_node *)mi->data;
	mi->node_num = CC_NODENUM;
	for (i = 0; i < mi->node_num; ++i) {
		node[i].ip = 0;
	}
}
static int sshm_node_add(struct sockaddr_in *sa, struct cc_shm *shm)
{
	int i = 0, ret = CC_ERROR,
		i_emptyip = -1, i_firsttime = -1;
	time_t first_time = time(NULL);
	struct cc_count *cc_count = NULL;
	struct cc_count_node *cc_count_node = NULL;
	
	cc_shmtx_lock();

	cc_count = (struct cc_count *)shm->addr;
	cc_count_node = (struct cc_count_node *)cc_count->data;
	/* 匹配 && 找空闲 && 找最晚更新 */
	for (i = 0; i < cc_count->node_num; ++i) {
		if (cc_count_node[i].ip == sa->sin_addr.s_addr
		    && cc_count_node[i].port == sa->sin_port) {
			cc_count_node[i].num++;
			time(&cc_count_node[i].last_time);
			ret = CC_OK;
			goto unlock_ret;
		}
		if (cc_count_node[i].ip == 0 && i_emptyip == -1)
			i_emptyip = i;
		if (first_time >= cc_count_node[i].last_time) {
			i_firsttime = i;
			first_time = cc_count_node[i].last_time;
		}
	}

	/* 填充空节点 */
	if (-1 != i_emptyip) {
		cc_count_node[i_emptyip].ip = sa->sin_addr.s_addr;
		cc_count_node[i_emptyip].port = sa->sin_port;
		cc_count_node[i_emptyip].num = 1;
		time(&cc_count_node[i_emptyip].last_time);
		ret = CC_OK;
		goto unlock_ret;
	}

	/* 替换最早的节点 */
	if (-1 != i_firsttime) {
		cc_count_node[i_firsttime].ip = sa->sin_addr.s_addr;
		cc_count_node[i_firsttime].port = sa->sin_port;
		cc_count_node[i_firsttime].num = 1;
		time(&cc_count_node[i_firsttime].last_time);
		ret = CC_OK;
		goto unlock_ret;
	}

	ret = CC_ERROR;

unlock_ret:
	cc_shmtx_unlock();
	return ret;
}
static void sshm_node_print(struct cc_shm *shm)
{
	int i = 0;
	struct cc_count *cc_count = NULL;
	struct cc_count_node *cc_count_node = NULL;
	char ip_buf[INET_ADDRSTRLEN] = {0};

	cc_shmtx_lock();

	cc_count = (struct cc_count *)shm->addr;
	cc_count_node = (struct cc_count_node *)cc_count->data;
	for (i = 0; i < cc_count->node_num; ++i) {
		printf("%d %s:%hu %d %ld\n", i,
		       inet_ntop(AF_INET,
				 (void *)&(cc_count_node[i].ip),
				 ip_buf, INET_ADDRSTRLEN),
		       ntohs(cc_count_node[i].port),
		       cc_count_node[i].num,
		       cc_count_node[i].last_time);
	}
	
	cc_shmtx_unlock();
}

static void sshm_process_child(int fd, struct cc_shm *shm)
{
	int conn_fd = -1;
	struct sockaddr_in client;
	socklen_t client_len = sizeof(client);
	char ip_buf[INET_ADDRSTRLEN] = {0};
	
	while (1) {
		conn_fd = accept(fd, (struct sockaddr *)&client, &client_len);
		if (conn_fd == -1)
			continue;

		printf("pid:%u-%s------->%s:%hu\n",
		       getpid(),
		       __func__,
		       inet_ntop(AF_INET,
				 (void *)&client.sin_addr.s_addr,
				 ip_buf, INET_ADDRSTRLEN),
		       ntohs(client.sin_port));
		
		sshm_node_add(&client, shm);

		close(conn_fd);
	}
}
static void sshm_process_father(struct cc_shm *shm)
{
	while (1) {
		sshm_node_print(shm);
		printf("==============================\n");
		sleep(3);
	}
}

/* SIGINT SIGTERM处理函数 */
void sig_int_term(int signo)
{
	int errno_save;

	errno_save = errno;

	/* 下面并不是可重入的函数???，信号处理函数如何传递参数啊？？ */
	/* unlink为什么会执行失败？？？？ */
	//cc_shm_free(shm->addr, shm->size);
	cc_shmtx_destory();
	printf("sig_int term is execute.\n");
	
	exit(0);

	errno = errno_save;
}

static int sshm_init(struct cc_shm *shm)
{
	int ret = 0;
	struct sigaction act;

	shm->size = sizeof(struct cc_count)
		+ sizeof(struct cc_count_node) * CC_NODENUM;
	
	/* 创建共享内存 */
	shm->addr = cc_shm_alloc(shm->size);
	if (!shm->addr)
		return CC_ERROR;

	/* 初始化锁 */
	ret = cc_shmtx_create();
	if (ret != CC_OK) {
		cc_shm_free(shm->addr, shm->size);
		return CC_ERROR;
	}
	
	sshm_node_init(shm);

	/* 注册信号处理程序 */
	act.sa_handler = sig_int_term;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	ret = sigaction(SIGINT, &act, NULL);
	if (ret < 0)
		return CC_ERROR;
	ret = sigaction(SIGTERM, &act, NULL);
	if (ret < 0)
		return CC_ERROR;
	
	return CC_OK;
}
static void sshm_fini(struct cc_shm *shm)
{
	cc_shm_free(shm->addr, shm->size);

	cc_shmtx_destory();
}

int main(void)
{
	int ret, fd, i;
	pid_t pid;
	struct sockaddr_in server;
	struct cc_shm shm;

	ret = sshm_init(&shm);
	if (ret != CC_OK)
		return 1;

	/* 初始化socket */
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == -1)
		return 1;
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(7777);
	ret = bind(fd, (struct sockaddr *)&server, sizeof(server));
	if (ret == -1)
		return 1;

	ret = listen(fd, 5);
	if (ret == -1)
		return 1;

	/* fork生成子进程，父子进程使用不同函数 */
	for (i = 0; i < 5; ++i) {
		pid = fork();
		if (pid == 0) {
			sshm_process_child(fd, &shm);
		}
	}

	sshm_process_father(&shm);

	sshm_fini(&shm);
	
	return 0;
}
