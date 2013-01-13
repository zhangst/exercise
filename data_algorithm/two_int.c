#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "hashmap/hashmap.h"

/**
 * 找出一堆int数中相加等于某个数的数对
 */

int main(void) {

    int num_max = 1000;
    int num_add = 155;

    int * arr = malloc(sizeof(int) * num_max);
    assert(arr != NULL);
    struct hashmap * m = map_new(num_max * 3 / 2);
    assert(m != NULL);

    srand(time(NULL));
    int i, t = 0;
    for (i = 0; i < num_max; i++,t = (t+1)%2) {
        if (t == 1) {
            arr[i] = rand();
        } else {
            arr[i] = rand() * -1;
        }
        map_insert(m, arr[i]);
    }
    map_dump(m);
    printf("-------\n");

    for (i = 0; i < num_max; i++) {
        if (map_search(m, arr[i]) == 1) {
            if (map_search(m, num_add - arr[i]) == 1) {
                map_earse(m, arr[i]);
                map_earse(m, num_add - arr[i]);
                printf("%d,%d\n", arr[i], num_add - arr[i]);
            }
        }
    }

    map_delete(m);
    free(arr);
        
    return 0;
}
