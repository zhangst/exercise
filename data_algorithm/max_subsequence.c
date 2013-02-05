/**
 * 最大自序列和的4种实现，《数据结构与算法分析》书中实现
 * 2012-02-05
 */

#include <stdio.h>

int max_subsequence_sum_1(const int arr[], int n) {
    int this_sum, max_sum, i, j, k;

    max_sum = 0;
    for (i = 0; i < n; ++i) {
        for (j = i; j < n; ++j) {
            this_sum = 0;
            for (k = i; k <= j; ++k) {
                this_sum += arr[k];
            }
            if (this_sum > max_sum) {
                max_sum = this_sum;
            }
        }
    }

    return max_sum;
}

int max_subsequence_sum_2(const int arr[], int n) {
    int this_sum, max_sum, i, j;

    max_sum = 0;
    for (i = 0; i < n; ++i) {
        this_sum = 0;
        for (j = i; j < n; ++j) {
            this_sum += arr[j];
            if (this_sum > max_sum) {
                max_sum = this_sum;
            }
        }
    }

    return max_sum;
}

static int max_3(int n1, int n2, int n3) {
    int max = (n1 > n2)?n1:n2;

    return (max > n3)?max:n3;
}
static int max_sub_sum_3(const int arr[], int left, int right) {
    int max_left_sum, max_right_sum;
    int max_leftborder_sum, max_rightborder_sum;
    int leftborder_sum, rightborder_sum;
    int center, i;

    if (left == right) {
        if (arr[left] > 0) {
            return arr[left];
        } else {
            return 0;
        }
    }

    center = (left + right) / 2;
    max_left_sum = max_sub_sum_3(arr, left, center);
    max_right_sum = max_sub_sum_3(arr, center + 1, right);

    max_leftborder_sum = 0;
    leftborder_sum = 0;
    for (i = center; i >= left; i--) {
        leftborder_sum += arr[i];
        if (leftborder_sum > max_leftborder_sum) {
            max_leftborder_sum = leftborder_sum;
        }
    }

    max_rightborder_sum = 0;
    rightborder_sum = 0;
    for (i = center + 1; i <= right; ++i) {
        rightborder_sum += arr[i];
        if (rightborder_sum > max_rightborder_sum) {
            max_rightborder_sum = rightborder_sum;
        }
    }

    return max_3(max_left_sum, max_right_sum, max_rightborder_sum + max_leftborder_sum);
}
int max_subsequence_sum_3(const int arr[], int n) {
    return max_sub_sum_3(arr, 0, n - 1);
}

int max_subsequence_sum_4(const int arr[], int n) {
    int this_sum, max_sum, j;

    this_sum = max_sum = 0;
    for (j = 0; j < n; ++j) {
        this_sum += arr[j];
        if (this_sum > max_sum) {
            max_sum = this_sum;
        } else if (this_sum < 0) {
            this_sum = 0;
        }
    }

    return max_sum;
}

int main(int argc, char * argv[]) {

    const int arr[] = {4,-3,5,-2,-1,2,6,-2};

    printf("%d\n", max_subsequence_sum_1(arr, sizeof(arr)/sizeof(int)));
    printf("%d\n", max_subsequence_sum_2(arr, sizeof(arr)/sizeof(int)));
    printf("%d\n", max_subsequence_sum_3(arr, sizeof(arr)/sizeof(int)));
    printf("%d\n", max_subsequence_sum_4(arr, sizeof(arr)/sizeof(int)));
    
    return 0;
}
