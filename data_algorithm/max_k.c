/**
 * 2013-02-01
 * 找出一组N个数第k大个数
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * 冒泡排序
 * type 1 升序 2 降序
 */
void bubble(int * arr, size_t arr_size, int type) {
    size_t i, j;

    for (i = 0; i < arr_size; ++i) {
        for (j = i + 1; j < arr_size; ++j) {
            int tmp = 0;
            if (type == 1) {
                if (arr[i] > arr[j]) {
                    tmp = 1;
                }
            } else if (type == 2) {
                if (arr[i] < arr[j]) {
                    tmp = 1;
                }
            }
            if (tmp != 0) {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}

/**
 * 冒泡排序取第K个数
 */
int max_k_1(int * arr, size_t arr_size, size_t k) {
    assert(k <= arr_size);
    bubble(arr, arr_size, 2);

    return arr[k-1];
}

/**
 * 创建大小为K的数组，拷贝arr的前K个数，排序;逐个读取剩余元素，将K数组中不合的元素挤出
 */
int max_k_2(int * arr, size_t arr_size, size_t k) {
    int * arr_k = malloc(sizeof(int) * (k+1));
    assert(arr_k != NULL);

    memcpy(arr_k, arr, k * sizeof(int));
    bubble(arr_k, k, 2);

    size_t i;
    for (i = k; i < arr_size; i++) {
        arr_k[k] = arr[i];
        size_t j;
        for (j = k; j > 0; --j) {
            if (arr_k[j] > arr_k[j-1]) {
                int tmp = arr_k[j];
                arr_k[j] = arr_k[j-1];
                arr_k[j-1] = tmp;
            }
        }
    }

    int k_num = arr_k[k-1];
    free(arr_k);

    return k_num;
}



void arr_print(int * arr, size_t arr_size) {
    size_t i;
    for (i = 0; i < arr_size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {

    int arr[] = {5, 2, 1, 3, 6};
    int arr_2[] = {5, 2, 1, 3, 6};
    arr_print(arr, sizeof(arr)/sizeof(int));
    printf("k=%d\n", max_k_1(arr, sizeof(arr)/sizeof(int), 1));
    printf("k=%d\n", max_k_2(arr_2, sizeof(arr_2)/sizeof(int), 1));
    arr_print(arr, sizeof(arr)/sizeof(int));
    
    return 0;
}
