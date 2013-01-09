#ifndef _FIFO_H_
#define _FIFO_H_

/**
 * 使用堆栈实现一个先进先出队列
 * 
 */

struct fifo * fifo_create(int max_elements);
void fifo_dispose(struct fifo * f);
int fifo_isempty(struct fifo * f);
int fifo_isfull(struct fifo * f);
void fifo_push(struct fifo * f, int element);
int fifo_pop(struct fifo * f);


#endif  /* _FIFO_H_ */
