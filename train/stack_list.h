#ifndef _STACK_LIST_
#define _STACK_LIST_

struct stack;

struct stack *stack_create(int max_elements);
void stack_dispose(struct stack *head);
int stack_isfull(struct stack *head);
int stack_isempty(struct stack *head);
void stack_push(struct stack *head, int element);
void stack_pop(struct stack *head);
int stack_top(struct stack *head);
int stack_topandpop(struct stack *head);
void stack_dump(struct stack *head);


#endif	/* _STACK_LIST_ */
