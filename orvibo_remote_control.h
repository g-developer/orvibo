/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file orvibo_remote_control.h
 * @author work(zhangyouchang@baidu.com)
 * @date 2014/09/19 18:34:48
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __ORVIBO_REMOTE_CONTROL_H_
#define  __ORVIBO_REMOTE_CONTROL_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>
#include <stdarg.h>

#include "orvibo_cmd.h"
#include "orvibo_util.h"
#define ORVIBO_RETRY_TIME		3

/*
typedef struct __Orvibo_Remote_Cmd_S{
	void *gen_cmd_str(const unsigned char *cmd_name, const char *cmd);
	void *send_cmd(const);
}_Orvibo_Remote_Cmd_T
*/


//发送远程命令给插座
unsigned char *orvibo_send_remote_cmd(const unsigned char *cmd_name, const unsigned char *cmd_content, const int cmd_content_len, const int socketfd);

//need free
unsigned char *gen_orvibo_cmd(_Orvibo_Cmd_Config_T *cmd_conf, const int bucket_num, const unsigned char *cmd_name, unsigned int *cmd_len, int argc, ...);

//no need to free
unsigned char *modify_orvibo_cmd_segment(_Orvibo_Cmd_Config_T *cmd_conf, const unsigned int bucket_num, unsigned char **cmd_tpl, const unsigned char *cmd_name, const unsigned argc, ...);

//3.2.1	 查询所有设备的广播
unsigned char *gen_orvibo_query_all_device(unsigned int *cmd_len);

//3.2.2	 查询指定插座的广播
unsigned char *gen_orvibo_query_device(unsigned int *cmd_len, const unsigned char *uid);

//3.2.3	 客户端登录接口
unsigned char *gen_orvibo_client_login(unsigned int *cmd_len, const unsigned char *uid, const unsigned char *local_passwd);

//3.2.6	 设备控制接口
unsigned char *gen_orvibo_device_control(unsigned int *cmd_len, const unsigned char *uid, const unsigned char *session_id, const unsigned char *status);

//3.2.11 心跳包接口
unsigned char *gen_orvibo_heart_beat(unsigned int *cmd_len, const unsigned char *uid, const unsigned char *session_id);


#endif  //__ORVIBO_REMOTE_CONTROL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
