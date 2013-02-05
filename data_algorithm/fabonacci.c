/**
 * Fabonacci斐波那契数列的递归和非递归实现
 * 2012-02-05
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

long int fib_r(int n) {
    if (n <= 1) {
        return 1L;
    }

    return fib_r(n - 1) + fib_r(n - 2);
}

/**
 * 用包含两个元素的数组，降低代码复杂度
 * arr[i%2] 会不断的覆盖数组中的值
 */
long int fib(int n) {
    if (n <= 1) {
        return 1L;
    }

    int i;
    long int arr[2] = {1L, 1L};
    for (i = 2; i <= n; ++i) {
        arr[i%2] = arr[0] + arr[1];
    }

    return arr[(i-1)%2];
}

void fib_print(int max, long int (* fib_func)(int n)) {
    int i;
    for (i = 0; i < max; ++i) {
        printf("%ld ", fib_func(i));
    }
    printf("\n");
}


int main(int argc, char * argv[]) {
    if (argc != 2) {
        return 1;
    }

    int max = atoi(argv[1]);
    fib_print(max, fib);
    fib_print(max, fib_r);
    /* 虚拟机中, max=40时，递归明显很慢 */
    
    return 0;
}
