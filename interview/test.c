#include "list.h"

/**
 * test
 */
static void list_test(void)
{
	int i = 0;
	struct list *l = list_create();
	for (i = 0; i < 20; i++) {
		list_append(l, i);
	}

	list_dump(l);

	l = list_rollback(l);

	list_dump(l);
}

int main(void)
{
	list_test();
	
	return 0;
}
