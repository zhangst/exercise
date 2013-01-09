#include "fifo.h"
#include "../stack_arr.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define EMPTY_SIZE (-1)
#define MIN_ELEMENTS (5)
struct fifo {
    int capacity;
    int size;
    struct stack *s[2];
};

struct fifo * fifo_create(int max_elements) {
    struct fifo * tmp = NULL;
    max_elements = (max_elements < MIN_ELEMENTS)?MIN_ELEMENTS:max_elements;

    tmp = malloc(sizeof(struct fifo));
    if (tmp == NULL) {
        fprintf(stderr, "%s:%d malloc error.\n",
                __func__, __LINE__);
        return NULL;
    }
    
    tmp->s[0] = stack_create(max_elements);
    if (tmp->s[0] == NULL) {
        fprintf(stderr, "%s:%d stack_create error.\n",
                __func__, __LINE__);
        free(tmp);
        return NULL;
    }
    tmp->s[1] = stack_create(max_elements);
    if (tmp->s[1] == NULL) {
        fprintf(stderr, "%s:%d stack_create error.\n",
                __func__, __LINE__);
        stack_dispose(tmp->s[0]);
        free(tmp);
        return NULL;
    }
    tmp->capacity = max_elements;
    tmp->size = EMPTY_SIZE;

    return tmp;
}

void fifo_dispose(struct fifo * f) {
    assert(f != NULL);

    stack_dispose(f->s[0]);
    stack_dispose(f->s[1]);
    free(f);
}

int fifo_isempty(struct fifo * f) {
    assert(f != NULL);

    return f->size == EMPTY_SIZE;
}

int fifo_isfull(struct fifo * f) {
    assert(f != NULL);

    return f->size + 1 == f->capacity;
}

void fifo_push(struct fifo * f, int element) {
    assert(!fifo_isfull(f));

    f->size += 1;
    stack_push(f->s[0], element);
}

int fifo_pop(struct fifo * f) {
    assert(!fifo_isempty(f));

    if (stack_isempty(f->s[1])) {
        while (!stack_isempty(f->s[0])) {
            int e = stack_topandpop(f->s[0]);
            stack_push(f->s[1], e);
        }
    }
    f->size -= 1;
    return stack_topandpop(f->s[1]);
}
