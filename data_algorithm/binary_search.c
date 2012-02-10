/*********************************************************************
 * Filename:      binary_search.c
 *                
 * Copyright (C) 2012,  zhangst
 * Version:       
 * Author:        zhangst <zhangst@leadsec.com.cn>
 * Created at:    Fri Feb  3 11:57:03 2012
 *                
 * Description:   
 *                
 ********************************************************************/
#include <stdio.h>


/**
 * Function binary_search (number, array, array_len)
 *
 *  Returns
 *    -1,if not found.index of array,if found.
 *
 *  Parameters
 *    @number:     the nubmer waiting for search
 *    @array:      search array pointer
 *    @array_len:  esarch array len
 * 
 *  Description
 *    premise that array is orderly.
 *
 **/
static int binary_search(int number, int *array, int array_len)
{
	int mid, start = 0, end = array_len - 1;

	while (start <= end) {
		mid = (start + end) / 2;
		if (array[mid] < number)
			start = mid + 1;
		else if (array[mid] > number)
			end = mid -1;
		else
			return mid;
	}

	return -1;
}


/**
 * Function binary_search_1 (number, array, array_len)
 *
 *  Returns
 *    
 *
 *  Parameters
 *    @number:     
 *    @array:      
 *    @array_len:  
 * 
 *  Description
 *    存在多个，返回第一个
 *
 **/
static int binary_search_1(int number, int *array, int array_len)
{
	int mid, start = 0, end = array_len - 1;

	while (start <= end) {
		mid = (start + end) / 2;
		if (array[mid] < number) {
			start = mid + 1;
		} else if (array[mid] > number) {
			end = mid -1;
		} else {
			while (mid && array[mid-1] == number) --mid;
			return mid;
		}
	}

	return -1;
}




int main(void)
{
	int arr[8] = {1,2,3,3,5,8,9,10};

	printf("array_len:%zu\n", sizeof(arr)/sizeof(int));
	printf("%d\n", binary_search(3, arr, sizeof(arr)/sizeof(int)));
	printf("_1:%d\n", binary_search_1(3, arr, sizeof(arr)/sizeof(int)));
	
	return 0;
}
