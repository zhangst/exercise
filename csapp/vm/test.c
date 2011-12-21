#include <stdio.h>

#include "mm.h"

#define return_if_fail(p) if(!(p))		\
	{printf("%s:%d warning: "#p" failed.\n", \
		__func__, __LINE__); return;}

#define return_val_if_fail(p, val) if (!(p)) \
	{printf("%s:%d warning "#p" failed.\n", \
		__func__, __LINE__); return (val);}

#define PTRS 10
#define mm_free_set_null(p) do {		\
		mm_free(p);			\
		p = NULL;			\
	} while(0);

int main(void)
{
	int ret = 0;
	int *p[PTRS] = {0};

	if ((ret = mm_init()) != 0) {
		fprintf(stderr, "mm_init error.\n");
	}


	/**
	 * 内部碎片：字节对齐造成
	 * 申请1字节，但总占用16字节。
	 */
	return_val_if_fail(p[0] = mm_malloc(1), 0);
	mm_checkheap(1);

	
	/**
	 * 外部碎片：小内存片释放后分散在很多地方，不能被充分利用。
	 * 如下面的例子p[1],p[3]释放的内存不能被p[6]利用。
	 */
	return_val_if_fail(p[1] = mm_malloc(8), 0);
	return_val_if_fail(p[2] = mm_malloc(8), 0);
	return_val_if_fail(p[3] = mm_malloc(8), 0);
	return_val_if_fail(p[4] = mm_malloc(8), 0);
	return_val_if_fail(p[5] = mm_malloc(8), 0);
	mm_checkheap(1);
	mm_free_set_null(p[1]);
	mm_free_set_null(p[3]);
	mm_checkheap(1);
	return_val_if_fail(p[6] = mm_malloc(8), 0);
	mm_checkheap(1);

	/**
	 * realloc
	 * 
	 */
	return_val_if_fail(p[7] = mm_malloc(24), 0);
	return_val_if_fail(p[8] = mm_malloc(24), 0);
	mm_checkheap(1);
	return_val_if_fail(p[7] = mm_realloc(p[7], 32), 0);
	mm_checkheap(1);
	return_val_if_fail(p[6] = mm_realloc(p[6], 16), 0);
	mm_checkheap(1);
 

	return 0;
}
