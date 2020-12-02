#include <stdio.h>

void arr_printf(int * arr, size_t arr_size) {
    if (arr != NULL) {
        printf("arr:");
        for (int i = 0; i < arr_size; ++i) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
}

int sum_loop(int * arr, size_t arr_size) {
    int sum = 0;
    
    if (arr == NULL) {
        return sum;
    }
    
    for (int i = 0; i < arr_size; ++i) {
        sum += arr[i];
    }

    return sum;
}

int sum_recursion(int * arr, size_t arr_size) {
    if (arr == NULL || arr_size == 0) {
        return 0;
    } else if (arr_size == 1) {
        return arr[0];
    } else {
        // 递归过程
        printf("%d + sum_recursion(%d, %d)\n", arr[0], arr+1, arr_size-1);
        
        return arr[0] + sum_recursion(arr+1, arr_size-1);
    }
}


int main(void) {
    int arr[] = {1,2,3,4,5,6};

    arr_printf(arr, sizeof(arr)/sizeof(int));
    printf("sum_loop:%d\n", sum_loop(arr, sizeof(arr)/sizeof(int)));
    printf("sum_recursion:%d\n", sum_recursion(arr, sizeof(arr)/sizeof(int)));
    return 0;
}
