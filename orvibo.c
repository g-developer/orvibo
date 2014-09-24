/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/



/**
 * @file orvibo.c
 * @author work(zhangyouchang@baidu.com)
 * @date 2014/09/24 15:19:32
 * @version $Revision$ 
 * @brief 
 *  
 **/


#include "orvibo.h"

int orvibo_init(_Orvibo_Schema_Config_Map_T schema_config[], _Orvibo_Cmd_Config_T *cmd_conf, unsigned char **cmd_tpl, const unsigned int bucket_num){
	if(NULL == schema_config || NULL != cmd_conf || NULL != cmd_tpl || 0 == bucket_num){
		return -1;
	}

	int ret = 0;
	//初始化全局表头数组
	cmd_conf = (_Orvibo_Cmd_Config_T *)malloc(sizeof(_Orvibo_Cmd_Config_T) * bucket_num);
	memset(cmd_conf, 0, sizeof(_Orvibo_Cmd_Config_T) * bucket_num);
	ret = read_orvibo_schema(schema_config, cmd_conf, bucket_num);
	if(0 > ret){
		return -1;
	}

	//初始化全局cmd命令内容数组
	cmd_tpl = (unsigned char **)malloc(sizeof(unsigned char *) * bucket_num);
	memset(cmd_tpl, 0, sizeof(unsigned char *) * bucket_num);

	int index = 0, i = 0;
	int cmd_len = 0;
	const unsigned char *qa_cmd = gen_orvibo_cmd(cmd_conf, bucket_num, "qa", &cmd_len, 0);
	for(i=0; i<cmd_len; i++){
		printf("wacalie---------qa_cmd[%d]=%x\n", i, qa_cmd[i]);
	}
	
	//为填充发送消息卡槽
	_Orvibo_Cmd_Info_T *cmd_info = (_Orvibo_Cmd_Info_T *)malloc(sizeof(_Orvibo_Cmd_Info_T));
	memset(cmd_info, 0, sizeof(_Orvibo_Cmd_Info_T));
	cmd_info->cmd_name = (unsigned char *)malloc(sizeof(unsigned const char) * 3);
	snprintf(cmd_info->cmd_name, 3, "%s", "qa");
	cmd_info->cmd_len = cmd_len;
	cmd_info->cmd_msg = (unsigned char *)malloc(sizeof(unsigned const char) * cmd_len);
	for(i=0; i<cmd_len; i++){
		cmd_info->cmd_msg[i] = qa_cmd[i];
	}

	//创建udp广播udp socket
	_Orvibo_Udp_Server_Info_T *udp_broadcast_info = (_Orvibo_Udp_Server_Info_T *)malloc(sizeof(_Orvibo_Udp_Server_Info_T));
	memset(udp_broadcast_info, 0, sizeof(_Orvibo_Udp_Server_Info_T));

	int udp_broadcast_fd = set_socketfd_info(udp_broadcast_info, "192.168.99.215", 10000, "255.255.255.255", 10000, 1);

	//定义接收消息结构体
	_Orvibo_Msg_Info_T *qa_msg_info = (_Orvibo_Msg_Info_T *)malloc(sizeof(_Orvibo_Msg_Info_T));
	memset(qa_msg_info, 0, sizeof(_Orvibo_Msg_Info_T));

	//发送 qa 查询命令
	send_cmd_2_orvibo_udp_server(udp_broadcast_info, cmd_info, qa_msg_info);

	
	for(i=0; i<qa_msg_info->response->cmd_len;i++){
		printf("wacalie---2-----qa_msg_inf[%d] = %x\n", i, qa_msg_info->response->cmd_msg[i]);
	}

	return 0;
}


















/* vim: set ts=4 sw=4 sts=4 tw=100 */
