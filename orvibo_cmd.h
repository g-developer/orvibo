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

typedef struct __Orvibo_Schema_Config_Map_S{
	char *cmd_name;								//cmd命令名称
	char **cmd_send_schema;						//发送的表头格式
	char **cmd_recv_schema;						//接收到的表头格式
}_Orvibo_Schema_Config_Map_T;

/*
//cmd名称---全局命令配置表所在节点的地址映射， 方便查找速度
typedef struct __Orvibo_Cmd_Name_2_Addr_S{
	char *cmd_name;								//cmd名称
	unsigned int addr;							//全局配置列表中的地址
}_Orvibo_Cmd_Name_2_Addr_T;
*/

typedef struct __Orvibo_Schema_S{
	char *title;								//表头
	unsigned len;								//长度
}_Orvibo_Schema_T;


//schema 列表，每条cmd的发送表头、接收表头均有一个schema列表分开记录
typedef struct __Orvibo_Schema_List_S{			
	_Orvibo_Schema_T *schema;					
	struct __Orvibo_Schema_List_S *next;
}_Orvibo_Schema_List_T;

//每条cmd的结构信息
typedef struct __Orvibo_Cmd_S{
	_Orvibo_Schema_List_T *requset_schema;
	_Orvibo_Schema_List_T *response_schema;
}_Orvibo_Cmd_T;

//全局cmd 信息列表
typedef struct __Orvibo_Cmd_List_S{
	char *cmd_name;
	int is_broadcast;							//是否是UDP广播方式
	_Orvibo_Cmd_T cmd;
	struct __Orvibo_Cmd_List *next;
}_Orvibo_Cmd_List_T;


//消息定义
typedef struct __Orvibo_Cmd_Info_S{
	char *cmd_name;
	unsigned char *cmd_msg;
	unsigned int cmd_len;
}_Orvibo_Cmd_Info_T;

//会话信息
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
