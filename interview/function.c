#include <stdio.h>
#include <assert.h>



/**
 * strcpy
 */
static char *z_strcpy(char *dst, const char *src)
{
	char *tmp = dst;
	assert(dst != NULL && src != NULL);

	while ((*tmp++ = *src++) != '\0')
		;

	return dst;
}
static char *z_strncpy(char *dst, const char *src, size_t n)
{
	size_t i = 0;
	assert(dst != NULL && src != NULL);
	
	for (i = 0; i < n && src[i] != '\0'; i++) {
		dst[i] = src[i];
	}
	for ( ; i < n; i++) {
		dst[i] = '\0';
	}
	
	return dst;
}

/**
 * memcpy
 */
static void *z_memcpy(void *dst, const void *src, size_t n)
{
	size_t i = 0;
	assert(dst != NULL && src != NULL);

	while (i < n) {
		*((char *)dst + i) = *((char *)src + i);
		++i;
	}

	return dst;
}

/**
 * memccpy
 * memmove
 */
static void *z_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t i = 0;
	int c_found = 0;		/* 0未发现  1发现 */
	assert(NULL != dst && NULL != src);

	while (i < n) {
		if ((*((char *)dst + i) = *((char *)src + i)) == c) {
			c_found = 1;
			break;
		}
		i++;
	}

	return (c_found == 1)?(void *)((char *)dst + i + 1):NULL;
}
/* glibc */
static void *__memccpy(void *dst, const void *src, int c, size_t n)
{
	register const char *s = src;
	register char *d = dst;
	register const char x = c;
	register size_t i = n;

	while (i-- > 0) {
		if ((*d++ = *s++) == x)
			return d;
	}

	return NULL;
}
static void *z_memmove(void *dst, const void *src, size_t n)
{
	char *d = NULL;
	const char *s = NULL;
	int op = 0;
	size_t i = n;

	if (dst < src) {
		d = dst;
		s = src;
		op = 1;
	} else {
		d = dst + n - 1;
		s = src + n - 1;
		op = -1;
	}

	while (i-- > 0) {
		*d = *s;
		d += op;
		s += op;
	}

	return dst;
}


/**
 * strcmp
 */
static int z_strcmp(const char *s1, const char *s2)
{
	assert(s1 != NULL && s2 != NULL);

	while (*s1 == *s2 && *s1 != '\0') {
		s1++;
		s2++;
	}

	return *s1 - *s2;
}

/**
 * strncmp
 */
static int z_strncmp(const char *s1, const char *s2, size_t n)
{
        size_t i = 0;
        assert(s1 != NULL && s2 != NULL);

        while (*s1 == *s2 && *s1 != '\0') {
                if (++i == n)
			break;
                s1++;
                s2++;
        }

        return *s1 - *s2;
}

/**
 * strcat
 */
static char *z_strcat(char *dst, const char *src)
{
	char *p = dst;
	assert(dst != NULL && src != NULL);

	while (*p != '\0')
		p++;
	while ((*p++ = *src++) != '\0')
		;

	return dst;
}
static char *z_strncat(char *dst, const char *src, size_t n)
{
	size_t dst_len = strlen(dst);
	size_t i;
	assert(dst != NULL && src != NULL);
	
	for (i = 0; i < n && src[i] != '\0'; i++)
		dst[dst_len + i] = src[i];
	dst[dst_len + i] = '\0';
	
	return dst;
}

/**
 * strlen
 */
static size_t z_strlen(const char *s)
{
	size_t i = 0;
	assert(s != NULL);
	
	while (*(s+i) != '\0') {
		i++;
	}

	return i;
}

/**
 * binary search
 */
int z_bsearch(const int *arr, const int arr_size, const int num)
{
	int start = 0, end = arr_size-1, mid = 0;
	assert(arr != NULL);

	while (start <= end) {
		mid = (start+end) / 2;
		if (arr[mid] > num) {
			end = mid - 1;
		} else if (arr[mid] < num) {
			start = mid + 1;
		} else {
			return mid;
		}
	}

	return -1;
}


int main(void)
{
	return 0;
}
