#include "stack_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * 链表实现，使用哑节点，哑节点capacity存储当前栈大小和栈上限
 */
struct stack {
	union {
		int element;
		struct {
			unsigned short capacity;
			unsigned short size;
		} head;
	} u;
/* #define element u.element */
/* #define capacity u.capacity */
/* #define size u.size */
	struct stack *next;
};
#define MIN_ELEMENTS (5)
#define MAX_ELEMENTS (65535)

struct stack *stack_create(int max_elements)
{
	struct stack *tmp = NULL;

	if (max_elements < MIN_ELEMENTS) {
		max_elements = MIN_ELEMENTS;
	} else if (max_elements > MAX_ELEMENTS) {
		fprintf(stderr, "max stack size is %d\n",
			MAX_ELEMENTS);
		return NULL;
	}

	tmp = malloc(sizeof(struct stack));
	if (NULL == tmp) {
		fprintf(stderr, "%s:%d malloc error.\n",
			__func__, __LINE__);
		exit(1);
	}
	tmp->next = NULL;
	tmp->u.head.capacity = max_elements;
	tmp->u.head.size = 0;

	return tmp;
}

void stack_dispose(struct stack *head)
{
	assert(NULL != head);

	while (!stack_isempty(head)) {
		stack_pop(head);
	}
}

int stack_isfull(struct stack *head)
{
	assert(NULL != head);

	return head->u.head.capacity == head->u.head.size;
}

int stack_isempty(struct stack *head)
{
	assert(NULL != head);

	return head->u.head.size == 0;
}

void stack_push(struct stack *head, int elements)
{
	struct stack *tmp = NULL;
	assert(!stack_isfull(head));

	tmp = malloc(sizeof(struct stack));
	if (NULL == tmp) {
		fprintf(stderr, "%s:%d malloc error.\n",
			__func__, __LINE__);
		exit(1);
	}
	tmp->u.element = elements;
	tmp->next = head->next;
	head->next = tmp;
	head->u.head.size++;
}

void stack_pop(struct stack *head)
{
	struct stack *tmp = NULL;
	assert(!stack_isempty(head));

	tmp = head->next;
	head->next = tmp->next;
	head->u.head.size--;

	free(tmp);
}

int stack_top(struct stack *head)
{
	assert(!stack_isempty(head));

	return head->next->u.element;
}

int stack_topandpop(struct stack *head)
{
	struct stack *tmp = NULL;
	int element;
	assert(!stack_isempty(head));

	tmp = head->next;
	head->next = tmp->next;
	head->u.head.size--;
	element = tmp->u.element;

	free(tmp);

	return element;
}

void stack_dump(struct stack *head)
{
	assert(NULL != head);
	
	printf("L(%d:%d)->", head->u.head.capacity, head->u.head.size);
	
	while (1) {
		head = head->next;
		if(NULL == head) {
			printf("NULL");
			break;
		} else {
			printf("%d->", head->u.element);
		}
	}
	
	printf("\n");
}
