#ifndef _STACK_ARR_H_
#define _STACK_ARR_H_

struct stack;

struct stack *stack_create(int max_elements);
void stack_dispose(struct stack *head);
int stack_isempty(struct stack *head);
int stack_isfull(struct stack *head);
void stack_push(struct stack *head, int element);
void stack_pop(struct stack *head);
int stack_top(struct stack *head);
int stack_topandpop(struct stack *head);

#endif	/* _STACK_ARR_H_ */
