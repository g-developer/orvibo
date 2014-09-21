/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/



/**
 * @file main.c
 * @author work(zhangyouchang@baidu.com)
 * @date 2014/09/20 15:14:57
 * @version $Revision$ 
 * @brief 
 *  
 **/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include "orvibo_cmd.h"
#include "orvibo_socket.h"
#include "orvibo_util.h"

_Orvibo_Schema_Config_Map_T cmd[] = {
	{   "qa",						 
		"Head:2,Len:2,Cmd:2", 
		"Head:2,Len:2,Cmd:2,Status:1,Uid:12,localPassword:12,Model:6,Time:4,Status:1",
		1,
	},
	{   "qg", 
		"Head:2,Len:2,Cmd:2,Uid:12", 
		"Head:2,Len:2,Cmd:2,Status:1,Uid:12,localPassword:12,Model:6,Time:4,Status:1",
		1,
	},
	{   "qc", 
		"Head:2,Len:2,Cmd:2,Uid:12", 
		"Head:2,Len:2,Cmd:2,Status:1,Uid:12,localPassword:12",
		0,
	},
	{   "qf", 
		"Head:2,Len:2,Cmd:2,Uid:12", 
		"Head:2,Len:2,Cmd:2,Status:1,Uid:12,localPassword:12,Status:1",
		0,
	},
	{   "qf2", 
		"Head:2,Len:2,Cmd:2,Uid:12", 
		"Head:2,Len:2,Cmd:2,Status:1,Uid:12,localPassword:12,Model:6,Time:4",
		0,
	},
};


int main(){
	
	_Orvibo_Cmd_Config_T cmd_conf[32];

	read_orvibo_schema(cmd, cmd_conf);

	/*
	_Orvibo_Cmd_Info_T *cmd_info = (_Orvibo_Cmd_Info_T *)malloc(sizeof(_Orvibo_Cmd_Info_T));
	memset(cmd_info, 0, sizeof(_Orvibo_Cmd_Info_T));

	_Orvibo_Udp_Server_Info_T *udp_info = (_Orvibo_Udp_Server_Info_T *)malloc(sizeof(_Orvibo_Udp_Server_Info_T));
	memset(udp_info, 0, sizeof(_Orvibo_Udp_Server_Info_T));

	_Orvibo_Msg_Info_T *msg_info = (_Orvibo_Msg_Info_T *)malloc(sizeof(_Orvibo_Msg_Info_T));
	memset(msg_info, 0, sizeof(_Orvibo_Msg_Info_T));

	getCmd(NULL, "qa", cmd_info);

	int setRet = set_socketfd_info(udp_info, "192.168.99.215", 10000, "255.255.255.255", 10000, 1);
	printf("set socket fd Ret[%d]\n",setRet);	

	send_cmd_2_orvibo_udp_server(udp_info, cmd_info, msg_info);

	display_msg(msg_info);

	destroy_cmd_info(cmd_info);
	destroy_msg_info(msg_info);
	close_orvibo_udp_socket(udp_info);
	*/

	return 0;
}














/* vim: set ts=4 sw=4 sts=4 tw=100 */
