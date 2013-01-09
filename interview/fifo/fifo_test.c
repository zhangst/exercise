#include "fifo.h"

#include <stdio.h>
#include <assert.h>

int main(void) {

    struct fifo * f = fifo_create(32);
    assert(f != NULL);

    int i = 0;
    for (i = 0; i < 10; i++) {
        fifo_push(f, i);
        printf("fifo_push: %d\n", i);
    }

    while (!fifo_isempty(f)) {
        printf("%d ", fifo_pop(f));
    }
    printf("\n");

    fifo_dispose(f);
    
    return 0;
}
