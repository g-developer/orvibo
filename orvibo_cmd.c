/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/



/**
 * @file orvibo_cmd.c
 * @author work(zhangyouchang@baidu.com)
 * @date 2014/09/19 12:00:22
 * @version $Revision$ 
 * @brief 
 *  
 **/


#include "orvibo_cmd.h"
#include "orvibo_socket.h"

/*
orviboCmd cmd[] = {
	{"qa", {0x68, 0x64, 0x00, 0x06, 0x71}, 0},
	{"qb", {0x68, 0x64, 0x00, 0x06, 0x71, 0x12}, 0},
	{"qc", {0x68, 0x64, 0x00, 0x06, 0x71, 0x13}, 0},
	{"qd", {0x68, 0x64, 0x00, 0x06, 0x71},0},
	{"qf", {0x68, 0x64, 0x00, 0x06, 0x71}, 0},
	{NULL, NULL, -1},
};
*/

int getCmd(_Orvibo_Cmd_List_T *cmdList, char *cmdqa, _Orvibo_Cmd_Info_T *cmd_info){
	unsigned char t[] = {0x68, 0x64, 0x00, 0x06, 0x71, 0x61};
	char *cmd = "qa";
	memset(cmd_info, 0, sizeof(_Orvibo_Cmd_Info_T));
	int cmd_len = strlen(cmd) + 1;
	cmd_info->cmd_name = (char*)malloc(sizeof(char) * cmd_len);
	snprintf(cmd_info->cmd_name, cmd_len, "%s", cmd);
	int cmd_msg_len = sizeof(t);
	cmd_info->cmd_msg = (unsigned char*)malloc(sizeof(char) * cmd_msg_len);
	//snprintf(cmd_info->cmd_msg, cmd_msg_len, "%u", t);
	cmd_info->cmd_len = cmd_msg_len;
	int i = 0;
	for(i=0; i<sizeof(t); i++){
		printf("%d--1--%x\t", i, t[i]);
		cmd_info->cmd_msg[i] = t[i];
	}
	printf("\n-----sizeof(t)[%d]\n",cmd_msg_len);
	for(i=0; i<sizeof(t); i++){
		printf("%d--2--%x\t", i, cmd_info->cmd_msg[i]);
	}
	printf("\n");
	return 0;
}

void destroy_cmd_info(_Orvibo_Cmd_Info_T *cmd_info){
	if(NULL != cmd_info){
		if(NULL != cmd_info->cmd_name){
			free(cmd_info->cmd_name);
		}
		if(NULL != cmd_info->cmd_msg){
			free(cmd_info->cmd_msg);
		}
		free(cmd_info);
	}
}

void destroy_msg_info(_Orvibo_Msg_Info_T *msg_info){
	if(NULL != msg_info){
		if(NULL != msg_info->requset){
			destroy_cmd_info(msg_info->requset);
		}
		if(NULL != msg_info->response){
			destroy_cmd_info(msg_info->response);
		}
		free(msg_info);
	}
}

void display_msg(_Orvibo_Msg_Info_T* talk){
	if(NULL == talk){
		printf("\n");
	}else{
		printf("request id[%ld]\n", talk->req_id);
		int i = 0;
		printf("%s:%d send		cmd[%s]:\n", inet_ntoa(talk->ask.sin_addr), talk->ask.sin_port, talk->requset->cmd_name);
		char* tmp = talk->requset->cmd_msg;
		for(i=0; i<talk->requset->cmd_len; i++){
			printf("\t%x", tmp[i]);
		}
		printf("\n");
		printf("%s:%d answer	cmd[%s]:\n", inet_ntoa(talk->answer.sin_addr), talk->answer.sin_port, talk->response->cmd_name);
		tmp = talk->response->cmd_msg;
		for(i=0; i<talk->response->cmd_len; i++){
			printf("\t%x", tmp[i]);
		}
		printf("\n");
	}
}

/*
int main(){
	_Orvibo_Cmd_Info_T *cmd_info = (_Orvibo_Cmd_Info_T *)malloc(sizeof(_Orvibo_Cmd_Info_T));
	memset(cmd_info, 0, sizeof(_Orvibo_Cmd_Info_T));
	getCmd(cmd, "qa", cmd_info);

	free(cmd_info);
}
*/















/* vim: set ts=4 sw=4 sts=4 tw=100 */
