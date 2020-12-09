#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>


unsigned int ip_str2int(const char *ip)
{
	assert(NULL != ip);
	const char *index = ip;
	unsigned char ip_c = 0;
	unsigned int ip_int = 0;

	while (1) {
		if (*index == '.' || *index == '\0') {
			ip_int = (ip_int<<8) | ip_c;
			ip_c = 0;
			if (*index == '\0')
				break;
		} else {
			ip_c = ip_c * 10 + (*index - '0');
		}
		index++;
	}

	return ip_int;
}

enum edian {
	BIG,
	LITTLE
};
unsigned int _ip_str2int(const char *ip, enum edian e)
{
	assert(NULL != ip);
	const char *index = ip;
	unsigned int ip_int = 0;
	unsigned char ip_c = 0;
	unsigned char *ip_cptr = NULL;
	char op = 0;
	if (e == BIG) {
		ip_cptr = ((unsigned char *)&ip_int);
		op = 1;
	} else if (e == LITTLE) {
		ip_cptr = ((unsigned char *)&ip_int + 3);
		op = -1;
	}

	while (1) {
		if (*index == '.' || *index == '\0') {
			*ip_cptr = ip_c;
			ip_c = 0;
			ip_cptr += op;
			if (*index == '\0')
				break;
		} else {
			ip_c = ip_c * 10 + (*index - '0');
		}
		index++;
	}

	return ip_int;
}





int main(void)
{

	//int inet_pton(int af, const char *src, void *dst);
	char * ip_str = "192.168.1.1";
	struct in_addr addr;
	char s[4] = {0};
	
	inet_pton(AF_INET, ip_str, &addr);
	
	memcpy((void *)s, (void *)&addr.s_addr, sizeof(int));
	int i = 0;
	for (i = 0; i < 4; i++) {
		printf("%hhu\n", s[i]);
	}

	int haddr = ntohl(addr.s_addr);
	memcpy((void *)s, (void *)&haddr, sizeof(int));
        for (i = 0; i < 4; i++) {
                printf("%hhu\n", s[i]);
        }

	unsigned int myaddr = _ip_str2int(ip_str, BIG);
	unsigned char *ma = (unsigned char *)&myaddr;
        for (i = 0; i < 4; i++) {
                printf("%hhu\n", *ma);
		ma++;
        }
	
	return 0;
}
