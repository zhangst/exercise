#include "stack_arr.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define EMPTY_INDEX (-1)
#define MIN_ELEMENTS (5)
struct stack {
	int capacity;
	int top_index;
	int *arr;
};

struct stack *stack_create(int max_elements)
{
	struct stack *tmp = NULL;
	
	max_elements = (max_elements < MIN_ELEMENTS)?MIN_ELEMENTS:max_elements;
	
	tmp = malloc(sizeof(struct stack));
	if (NULL == tmp) {
		fprintf(stderr, "%s:%d malloc error.\n",
			__func__, __LINE__);
		return NULL;
	}
	tmp->arr = malloc(sizeof(int) * max_elements);
	if (NULL == tmp->arr) {
		fprintf(stderr, "%s:%d malloc error.\n",
			__func__, __LINE__);
                free(tmp);
		return NULL;
	}
	tmp->top_index = EMPTY_INDEX;
	tmp->capacity = max_elements;

	return tmp;
}

void stack_dispose(struct stack *s)
{
	assert(NULL != s);
	
	free(s->arr);
	free(s);
}

int stack_isempty(struct stack *s)
{
	assert(NULL != s);
	
	return s->top_index == EMPTY_INDEX;
}

int stack_isfull(struct stack *s)
{
	assert(NULL != s);

	return s->top_index + 1 == s->capacity;
}

void stack_push(struct stack *s, int element)
{
	assert(!stack_isfull(s));

	s->arr[++s->top_index] = element;
}

void stack_pop(struct stack *s)
{
	assert(!stack_isempty(s));

	--s->top_index;
}

int stack_top(struct stack *s)
{
	assert(!stack_isempty(s));

	return s->arr[s->top_index];
}

int stack_topandpop(struct stack *s)
{
	assert(!stack_isempty(s));

	return s->arr[s->top_index--];
}
