/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file orvibo_cmd.h
 * @author work(zhangyouchang@baidu.com)
 * @date 2014/09/19 11:41:35
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __ORVIBO_CMD_H_
#define  __ORVIBO_CMD_H_
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

typedef struct __Orvibo_Head_Map_S{
	char *head;
	unsigned len;
}_Orvibo_Head_Map_T;

typedef struct __Orvibo_Cmd_S{
	_Orvibo_Head_Map_T **requset_head;
	_Orvibo_Head_Map_T **response_head;
	char *cmd_name;
	void *cmd_info;
}_Orvibo_Cmd_T;

typedef struct __Orvibo_Cmd_List_S{
	char *cmd_name;
	_Orvibo_Cmd_T cmd;
	struct __Orvibo_Cmd_List *next;
}_Orvibo_Cmd_List_T;

typedef struct __Orvibo_Cmd_Info_S{
	char *cmd_name;
	unsigned char *cmd_msg;
	unsigned int cmd_len;
}_Orvibo_Cmd_Info_T;

typedef struct __Orvibo_Msg_Info_S{
	unsigned long req_id;
	_Orvibo_Cmd_Info_T *requset;
	_Orvibo_Cmd_Info_T *response;
	struct sockaddr_in ask;
	struct sockaddr_in answer;
}_Orvibo_Msg_Info_T;

int getCmd(_Orvibo_Cmd_List_T *cmdList, char *cmdqa, _Orvibo_Cmd_Info_T *cmd_info);

//int complate_cmd_list();

void display_msg(_Orvibo_Msg_Info_T* talk);
void destroy_cmd_info(_Orvibo_Cmd_Info_T *cmd_info);
void destroy_msg_info(_Orvibo_Msg_Info_T *msg_info);






#endif  //__ORVIBO_CMD_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
