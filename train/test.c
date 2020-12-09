#include "list.h"

#include <stdio.h>

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

	list_delete(l, 1);
	list_dump(l);
	list_delete(l, 0);
	list_dump(l);
	list_delete(l, 19);
        list_dump(l);

	printf("\n");

	struct list *l1 = list_create();
	for (i = 0; i < 11; i++) {
		list_append(l1, i);
	}

	list_dump(l1);

        l1 = list_rollforward(l1);

        list_dump(l1);

	printf("\n");
	
}

int main(void)
{
	list_test();
	
	return 0;
}
