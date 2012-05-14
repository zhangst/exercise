#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct list {
	int element;
	struct list *next;
};


static struct list *list_mknode(void)
{
	struct list *tmp = NULL;

	tmp = malloc(sizeof(struct list));
	if (NULL == tmp) {
		fprintf(stderr, "%s:%d malloc error.\n",
			__func__, __LINE__);
		exit(1);
	}
	tmp->next = NULL;

	return tmp;
}

struct list *list_create(void)
{
	struct list *tmp = NULL;

	tmp = list_mknode();
	tmp->next = NULL;

	return tmp;
}

void list_dispose(struct list *head)
{
	struct list *first = NULL;
	assert(NULL != head);

	while ((first = head->next) != NULL) {
		head->next = first->next;
		free(first);
	}

	free(head);
}

void list_append(struct list *head, int element)
{
	struct list *tmp = NULL;
	assert(NULL != head);

	tmp = list_mknode();

	tmp->element = element;
	tmp->next = head->next;
	head->next = tmp;
}

void list_insert(struct list *head, int element, struct list *p)
{
	struct list *tmp = NULL;
	assert(NULL != head && p != NULL);

	tmp = list_mknode();
	tmp->element = element;
	tmp->next = p->next;
	p->next = tmp;
}

void list_delete(struct list *head, int element)
{
	struct list *pre = NULL, *tmp = NULL;
	assert(NULL != head);

	for (pre = head; pre != NULL && NULL != pre->next; pre = pre->next) {
		if (pre->next->element == element) {
			tmp = pre->next;
			pre->next = tmp->next;
			free(tmp);
		}
	}
}

struct list *list_find(struct list *head, int element)
{
	struct list *first = NULL;
	assert(NULL != head);

	for (first = head->next; NULL != first; first = first->next) {
		if (first->element == element)
			return first;
	}

	return NULL;
}

int list_isempty(struct list *head)
{
	assert(NULL != head);

	return head->next == NULL;
}


/**
 * 反转链表，两个方案：
 * 1、创建一个新的List，顺序把原有的链表内容插入
 * 2、直接使用指针操作
 */
struct list *list_rollback(struct list *L)
{
	struct list *l1 = NULL, *l2 = NULL, *l3 = NULL;
	assert(NULL != L);

	if (L->next == NULL)
		return NULL;

	l1 = L;
	l2 = l1->next;
	l3 = l2->next;
	while (l2 != NULL) {
		l2->next = l1;
		l1 = l2;
		l2 = l3;
		if (l3 != NULL)
			l3 = l3->next;
	}

	L->next->next = NULL;
	L->next = l1;

	return L;
}

void list_dump(struct list *head)
{
	assert(NULL != head);
	
	printf("L->");
	
	while (1) {
		head = head->next;
		if(NULL == head) {
			printf("NULL");
			break;
		} else {
			printf("%d->", head->element);
		}
	}
	
	printf("\n");
}
