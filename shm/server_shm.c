/**
 * 服务器包含一个父进程、多个子进程。父进程创建共享内存，子进程使用共享内存。
 * 父进程定时输出共享内存内容，子进程accept连接，并更新共享内存。
 * 根据IP、Port区分统计客户端访问量
 *
 * 注意：
 * 1、信号处理函数，Ctrl + C时，注销共享内存、锁
 * 2、缓冲区满了，如何处理
 *     增加time字段，将最早的节点替换掉。每次增加新节点都更新time时间。
 * 3、创建内存，之后如果失败需要回滚
 *
 *
 * 锁的设计：我的加锁对象是谁?共享内存、共享内存存储的内容
 * 本例子的设计对共享内存加锁，加锁后可以读取、更新共享内存内容
 *
 * 疑问：
 * 什么应该告诉调用者，我错了!就是什么时候需要函数返回值，或者用void
 *
 */


#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SSHM_OK 0
#define SSHM_ERROR 1
#define SSHM_NODENUM 20
#define SSHM_INT64_LEN   sizeof("-9223372036854775808") - 1

struct sshm_manage_ipnode {
	int node_num;
	char data[1];		/* 为什么是char数组，为什么不是int？？ */
};

struct sshm_ipnode {
	uint32_t ip;
	uint16_t port;
	uint16_t num;
};

/**
 * 共享内存，访问共享内存时需要加锁；
 * 存储的内容由使用者负责解析
 */
struct sshm_shm {
	char *addr;
	size_t size;
	int mutex;
};

struct sshm_config {
	struct sshm_shm shm;
};

/**
 * 创建pid文件，并将进程pid写入文件
 */
static int sshm_create_pidfile(const char *file)
{
	int fd;
	char pid[SSHM_INT64_LEN + 2];

	fd = open(file, O_RDWR | O_CREAT | O_TRUNC,
		  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1) {
		fprintf(stderr, "open %s failed.\n", file);
		return SSHM_ERROR;
	}

	snprintf(pid, sizeof(pid), "%d\n", getpid());
	if (write(fd, pid, strlen(pid)) == -1) {
		fprintf(stderr, "write %s failed.\n", file);
		return SSHM_ERROR;
	}

	if (close(fd) == -1)
		fprintf(stderr, "close %s failed.\n", file);

	return SSHM_OK;
}

static void sshm_delete_pidfile(const char *file)
{
	if (unlink(file) == -1)
		fprintf(stderr, "unlink %s failed.\n", file);
}

/**
 * 共享内存创建、注销
 */
static int sshm_shm_alloc(struct sshm_shm *shm)
{
	shm->addr = (char *)mmap(NULL, shm->size,
				 PROT_READ | PROT_WRITE,
				 MAP_ANON | MAP_SHARED, -1, 0);
	if (shm->addr == MAP_FAILED) {
		fprintf(stderr,
			"mmap(MAP_ANON | MAP_SHARED, %uz) failed.\n",
			shm->size);
		return SSHM_ERROR;
	}

	return SSHM_OK;
}
static void sshm_shm_free(struct sshm_shm *shm)
{
	if (munmap((void *)shm->addr, shm->size) == -1) {
		fprintf(stderr, "munmap(%p, %uz) failed.\n",
			shm->addr, shm->size);
	}
}

/**
 * 文件锁初始化、注销、加锁、解锁
 */
#define SHM_PIDFILE "/tmp/shm.pid"
static int sshm_shmtx_init(struct sshm_shm *shm)
{
	int ret;
	
	ret = sshm_create_pidfile(SHM_PIDFILE);
	if (ret != SSHM_OK)
		return SSHM_ERROR;
	
	shm->mutex = open(SHM_PIDFILE, O_RDWR);
	if (shm->mutex == -1) {
		fprintf(stderr, "open %s failed.\n",
			SHM_PIDFILE);
		sshm_delete_pidfile(SHM_PIDFILE);
		return SSHM_ERROR;
	}
	
	return SSHM_OK;
}
static void sshm_shmtx_fini(struct sshm_shm *shm)
{
	sshm_delete_pidfile(SHM_PIDFILE);
	if (shm->mutex >= 0) {
		close(shm->mutex);
	}
}
int sshm_shmtx_lock(struct sshm_shm *shm)
{
	struct flock fl;
	int ret;
	
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;

	while (1) {
		ret = fcntl(shm->mutex, F_SETLKW, &fl);
		if (ret == -1) {
			if (errno == EINTR)
				continue;
			else
				return SSHM_ERROR;
		} else {
			return SSHM_OK;
		}
	}
}
int sshm_shmtx_unlock(struct sshm_shm *shm)
{
	struct flock fl;
	int ret;
	
	fl.l_type = F_UNLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;

	while (1) {
		ret = fcntl(shm->mutex, F_SETLK, &fl);
		if (ret == -1) {
			if (errno == EINTR)
				continue;
			else
				return SSHM_ERROR;
		} else {
			return SSHM_OK;
		}
	}
}

/**
 * 共享内存初始化：新建共享内存，新建锁
 */
int sshm_shm_init(struct sshm_shm *shm)
{
	int ret;
	
	ret = sshm_shm_alloc(shm);
	if (ret != SSHM_OK)
		return SSHM_ERROR;

	ret = sshm_shmtx_init(shm);
	if (ret != SSHM_OK) {
		sshm_shm_free(shm);
		return SSHM_ERROR;
	}

	return SSHM_OK;
}
void sshm_shm_fini(struct sshm_shm *shm)
{
	sshm_shm_free(shm);

	sshm_shmtx_fini(shm);
}

/**
 * 以上可看为共享内存基础函数，包含了创建、注销、加锁、解锁四个接口
 * ============================================================
 */


/**
 * 初始化：对共享内存中存储的结构体节点赋初始值
 * 添加：填充一个节点
 */
static void sshm_node_init(struct sshm_shm *shm)
{
	int i;
	struct sshm_manage_ipnode *mi;
	struct sshm_ipnode *node;

	mi = (struct sshm_manage_ipnode *)shm->addr;
	node = (struct sshm_ipnode *)mi->data;
	mi->node_num = SSHM_NODENUM;
	for (i = 0; i < mi->node_num; ++i) {
		node[i].ip = 0;
	}
}
static int sshm_node_add(struct sockaddr_in *sa, struct sshm_shm *shm)
{
	int i, ret;
	struct sshm_manage_ipnode *mi;
	struct sshm_ipnode *node;
	
	sshm_shmtx_lock(shm);

	mi = (struct sshm_manage_ipnode *)shm->addr;
	node = (struct sshm_ipnode *)mi->data;
	for (i = 0; i < mi->node_num; ++i) {
		if (node[i].ip == sa->sin_addr.s_addr
		    && node[i].port == sa->sin_port) {
			node[i].num++;
			ret = SSHM_OK;
			goto unlock_ret;
		}
	}

	for (i = 0; i < mi->node_num; ++i) {
		if (node[i].ip == 0) {
			node[i].ip = sa->sin_addr.s_addr;
			node[i].port = sa->sin_port;
			node[i].num = 1;
			ret = SSHM_OK;
			goto unlock_ret;
		}
	}

	ret = SSHM_ERROR;

unlock_ret:
	sshm_shmtx_unlock(shm);
	return ret;
}
static void sshm_node_print(struct sshm_shm *shm)
{
	int i;
	struct sshm_manage_ipnode *mi;
	struct sshm_ipnode *node;
	char ip_buf[INET_ADDRSTRLEN];

	sshm_shmtx_lock(shm);

	mi = (struct sshm_manage_ipnode *)shm->addr;
	node = (struct sshm_ipnode *)mi->data;
	for (i = 0; i < mi->node_num; ++i) {
		printf("%d %s:%hu %d\n", i,
		       inet_ntop(AF_INET, (void *)&(node[i].ip),
				 ip_buf, INET_ADDRSTRLEN),
		       ntohs(node[i].port), node[i].num);
	}
	
	sshm_shmtx_unlock(shm);
}


static void sshm_process_child(int fd, struct sshm_shm *shm)
{
	int conn_fd;
	struct sockaddr_in client;
	socklen_t client_len = sizeof(client);
	char ip_buf[INET_ADDRSTRLEN];
	
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
static void sshm_process_father(struct sshm_shm *shm)
{
	while (1) {
		sshm_node_print(shm);
		printf("==============================\n");
		sleep(3);
	}
}


static int sshm_init(struct sshm_config *sh_config)
{
	int ret;

	sh_config->shm.size = sizeof(struct sshm_manage_ipnode)
		+ sizeof(struct sshm_ipnode) * SSHM_NODENUM;
	
	/* 创建共享内存 */
	ret = sshm_shm_init(&(sh_config->shm));
	if (ret != SSHM_OK)
		return SSHM_ERROR;
	
	sshm_node_init(&(sh_config->shm));
	
	return SSHM_OK;
}
static void sshm_fini(struct sshm_config *sh_config)
{
	sshm_shm_fini(&(sh_config->shm));
}

int main(void)
{
	int ret, fd, i;
	pid_t pid;
	struct sockaddr_in server;
	struct sshm_config sh_config;
	

	ret = sshm_init(&sh_config);
	if (ret != SSHM_OK)
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
			sshm_process_child(fd, &(sh_config.shm));
		}
	}

	sshm_process_father(&(sh_config.shm));

	sshm_fini(&sh_config);
	
	return 0;
}
