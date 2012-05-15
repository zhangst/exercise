#include <stdio.h>
#include <assert.h>

void bubble(int *arr, size_t arr_num)
{
	size_t i, j;
	int tmp;
	assert(arr != NULL);

	for (i = 0; i < arr_num; i++) {
		for (j = i+1; j < arr_num; j++) {
			if (arr[i] > arr[j]) {
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

int main(void)
{
	int t[] = {4, 7, 1, 9, 8};

	bubble(t, sizeof(t)/sizeof(int));

	int i = 0;
	for (i = 0; i < sizeof(t)/sizeof(int); i++) {
		printf("%d,", t[i]);
	}
}
