/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/



/**
 * @file orvibo_util.c
 * @author work(zhangyouchang@baidu.com)
 * @date 2014/09/21 17:59:51
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include "orvibo_util.h"


unsigned char **split_str_2_arr(const unsigned char *src, const unsigned char sp){
	if(NULL == src || '\0' == sp){
		return NULL;
	}
	//printf("src=%s\n",src);
	unsigned long index1 = 0;
	unsigned long index2 = 0;
	unsigned long offset = 0;
	unsigned long count = 1;
	unsigned long i = 0;
	unsigned long j = 0;
	unsigned char **arr, **tmp, *start;
	start = (unsigned char *)src;
	tmp = arr;

	//先计算出需要划分多少个字符串数组， 至少一个	
	while('\0' != start[index1]){		
		if(sp == start[index1]){
			count++;
		}
		index1++;
	}
	
	
	//malloc arr分区大小,多malloc出一个，置标志位NULL
	arr = (unsigned char**)malloc(sizeof(unsigned char*) * (count + 1));
	memset(arr, 0, sizeof(unsigned char*) * (count + 1));
	//printf("count=%ld\n", count);
	index1 = 0;
	i = 0;
	j = 0;
	while('\0' != start[index1]){
		if(sp == start[index1]){
			offset = index1 - index2;
			//printf("index1=%ld, index2=%ld, offset=%ld, start[index1]=%c, start[index2]=%c\n", index1, index2, offset, start[index1], start[index2]);
			if(0 == offset){
				arr[j] = (unsigned char *)malloc(sizeof(unsigned  char) * 1);
				*(*(arr + j) + 0) = '\0';
			}else{
				arr[j] = (unsigned char *)malloc(sizeof(unsigned  char) * (offset + 1));
				for(i=0; i<offset; i++){
					arr[j][i] = start[index2 + i];
				}
				arr[j][offset] = '\0';
			}
			j++;
			index2 = index1 + 1;
		}
		index1++;
	}
	
	//printf("j=%ld, count=%ld\n", j, count);
	
	count--;			//下标减一
	if(index1 != index2){
		offset = index1 - index2;
		//printf("index1=%ld, index2=%ld, offset=%ld, start[index1]=%c, start[index2]=%c\n", index1, index2, offset, start[index1], start[index2]);
		if(0 == offset){
			arr[count] = (unsigned char *)malloc(sizeof(unsigned  char) * 1); 
			*(*(arr + count) + 0) = '\0';
		}else{
			arr[count] = (unsigned char *)malloc(sizeof(unsigned  char) * (offset + 1));
			for(i=0; i<offset; i++){
				arr[count][i] = start[index2 + i]; 
			} 
			arr[count][offset] = '\0';
		} 
	}
	count++;			//标志位置为NULL
	//printf("index1=%ld\n", index1);
	arr[count] = (unsigned char *)malloc(sizeof(unsigned  char) * 1);
	free(arr[count]);
	arr[count] = NULL;
	tmp = arr;
	i = 0;
	while(NULL != tmp[i]){
		//printf("arr[%ld] = %s\n", i, tmp[i]);
		i++;
	}
	return arr;
}

int free_str_2_arr(unsigned char **arr){
	unsigned long i = 0;
	unsigned char **tmp = arr;
	if(NULL == arr){
		return 0;
	}
	while(NULL != tmp[i]){
		free(tmp[i]);
		i++;
	}
	if(NULL != tmp){
		free(tmp);
	}
	return 0;
}

unsigned long hash_time33(const unsigned char *str){
	if(NULL == str){
		return -1;
	}
	unsigned long hash_num = 0;
	while('\0' != *str){
		hash_num = 33 * hash_num + (unsigned long)(*str);
		str++;
	}
	return hash_num;
}

int get_schema_hash_num(const unsigned char* cmd_name, const unsigned int bucket_num){
	if(0 == bucket_num || NULL == cmd_name){
		return -1;
	}
	unsigned long hash_num = hash_time33(cmd_name);
	return (hash_num % bucket_num);
}













/* vim: set ts=4 sw=4 sts=4 tw=100 */
