/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file orvibo_util.h
 * @author work(zhangyouchang@baidu.com)
 * @date 2014/09/21 17:56:48
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __ORVIBO_UTIL_H_
#define  __ORVIBO_UTIL_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>


unsigned char **split_str_2_arr(const unsigned char *src, const unsigned char sp);

int free_str_2_arr(unsigned char **arr);

unsigned long hash_time33(const unsigned char *str);

int get_schema_hash_num(const unsigned char* cmd_name, const unsigned int bucket_num);









#endif  //__ORVIBO_UTIL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
