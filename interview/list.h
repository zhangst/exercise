#ifndef _LIST_H_
#define _LIST_H_

struct list;

struct list *list_create(void);
void list_dispose(struct list *head);
void list_append(struct list *head, int element);
void list_insert(struct list *head, int element, struct list *position);
void list_delete(struct list *head, int element);
struct list *list_find(struct list *head, int element);
int list_isempty(struct list *head);
void list_dump(struct list *head);

/* 非标准接口 */
struct list *list_rollback(struct list *L);
struct list *list_rollforward(struct list *L);

#endif
