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

int assign_cmd_tpl(unsigned char **cmd_tpl, const unsigned int bucket_num, unsigned const char *cmd_name, unsigned const char *str_cmd, unsigned const cmd_len){
	if(NULL == cmd_tpl || 0 == bucket_num || NULL == cmd_name || NULL == str_cmd || 0 == cmd_len){
		return -1;
	}
	int hash_num = 0, i = 0;
	hash_num = get_schema_hash_num(cmd_name, bucket_num);
	cmd_tpl[hash_num] = (unsigned char*)malloc(sizeof(unsigned char) * cmd_len);
	memset(cmd_tpl[hash_num], 0, sizeof(unsigned char) * cmd_len);
	for(i=0; i<cmd_len; i++){
		cmd_tpl[hash_num][i] = str_cmd[i];
	}
	free(str_cmd);
	return 0;
}

int assign_cmd_info(_Orvibo_Cmd_Info_T *cmd_info, unsigned const char *cmd_name, unsigned int cmd_len, unsigned const char *cmd_str){
	//为填充发送消息卡槽
	int i = 0;
	memset(cmd_info, 0, sizeof(_Orvibo_Cmd_Info_T));
	int cmd_name_len = strlen(cmd_name) + 1;
	snprintf(cmd_info->cmd_name, cmd_name_len, "%s", cmd_name);
	cmd_info->cmd_len = cmd_len;
	for(i=0; i<cmd_len; i++){
		cmd_info->cmd_msg[i] = cmd_str[i];
	}
	return 0;
}

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

	int i = 0;
	int cmd_len = 0;
	const unsigned char *qa_cmd = gen_orvibo_cmd(cmd_conf, bucket_num, "qa", &cmd_len, 0);

	//为填充发送消息卡槽
	_Orvibo_Cmd_Info_T cmd_info;
	if(0 >  assign_cmd_info(&cmd_info, "qa", cmd_len, qa_cmd)){
		printf("assign cmd info error !\n");
		exit(0);
	}

	//创建udp广播udp socket
	_Orvibo_Udp_Server_Info_T *udp_broadcast_info = (_Orvibo_Udp_Server_Info_T *)malloc(sizeof(_Orvibo_Udp_Server_Info_T));
	memset(udp_broadcast_info, 0, sizeof(_Orvibo_Udp_Server_Info_T));
	int udp_broadcast_fd = gen_orvibo_udp_socket(udp_broadcast_info, "255.255.255.255", 10000, 0);
	if(0 > udp_broadcast_fd){
		printf("1, create udp socket error[%d]!\n", udp_broadcast_fd);
		return -1;
	}

	//获取本地Ip
	unsigned char local_ip[32] = {'\0'};
	int get_local_ip_ret = get_local_ip(udp_broadcast_fd, local_ip);
	if(0 > get_local_ip_ret){
		printf("get local Ip error!\n");
		exit(0);
	}
	printf("Local_Ip is [%s]\n", local_ip);	

	//创建接收命令的udp Server 
	_Orvibo_Udp_Server_Info_T *udp_server_info = (_Orvibo_Udp_Server_Info_T *)malloc(sizeof(_Orvibo_Udp_Server_Info_T));
	memset(udp_server_info, 0, sizeof(_Orvibo_Udp_Server_Info_T));
	int udp_server_fd = gen_orvibo_udp_socket(udp_server_info, local_ip, 10000, 2);
	if(0 > udp_server_fd){
		printf("2, create udp socket error![%d]\n", udp_server_fd);
		return -1;
	}


	//定义接收消息结构体
	_Orvibo_Msg_Info_T *qa_msg_info = (_Orvibo_Msg_Info_T *)malloc(sizeof(_Orvibo_Msg_Info_T));
	memset(qa_msg_info, 0, sizeof(_Orvibo_Msg_Info_T));

	//发送 qa 查询命令
	send_cmd_2_orvibo_udp_server(udp_broadcast_info, udp_server_info, &cmd_info, qa_msg_info);
	for(i=0; i<cmd_len; i++){
		printf("wacalie----1111-----cl_cmd[%d]=%x\n", i, qa_msg_info->response->cmd_msg[i]);
	}
	//qa命令用完之后，赋值给全局cmd str 配置，然后free掉
	if(0 > assign_cmd_tpl(cmd_tpl, bucket_num, "qa", qa_cmd, cmd_len)){
		printf("Assign cmd 2 tpl\n");
		exit(0);
	}

	exit(0);
	//创建非广播 udp 发送的clinet
	_Orvibo_Udp_Server_Info_T *udp_client_info = (_Orvibo_Udp_Server_Info_T *)malloc(sizeof(_Orvibo_Udp_Server_Info_T));
	memset(udp_client_info, 0, sizeof(_Orvibo_Udp_Server_Info_T));
	//通过qa_msg_info->answer->addr 获取ip地址
	//int udp_client_fd = gen_orvibo_udp_socket(udp_client_info, "192.168.99.106", 10000, 1);
	unsigned char *orvibo_server_ip = inet_ntoa(qa_msg_info->answer.sin_addr);
	printf("orvibo_server_ip = %s\n", orvibo_server_ip);
	int udp_client_fd = gen_orvibo_udp_socket(udp_client_info, orvibo_server_ip, 10000, 1);
	if(0 > udp_client_fd){
		printf("3, create udp socket error![%d]\n", udp_client_fd);
		return -1;
	}
	//先获取session schema配置信息，然后保存session， 用完需要free
	//unsigned char *get_orvibo_cmd_segment(_Orvibo_Cmd_Config_T *cmd_conf, unsigned const int bucket_num, const unsigned char *resp, unsigned const char* cmd_name, unsigned const char *type, unsigned const char *segment)
	unsigned int uid_len = 0;
	unsigned char *uid = get_orvibo_cmd_segment(cmd_conf, bucket_num, qa_msg_info->response->cmd_msg, "qa", "response", "Uid", &uid_len);

	//获取 local_passwd，用完需要free
	unsigned int local_passwd_len = 0;
	unsigned char *local_password = get_orvibo_cmd_segment(cmd_conf, bucket_num, qa_msg_info->response->cmd_msg, "qa", "response", "LocalPassword", &local_passwd_len);

	unsigned const char head[] = {0x68, 0x64};
	unsigned const char cl_len[] = {0x00, 0x2e};
	unsigned const char cl_name[] = {0x63, 0x6c};

	//登录客户端,获取session_id
	const unsigned char *cl_cmd = gen_orvibo_cmd(cmd_conf, bucket_num, "cl", &cmd_len, 10, "Head", head, "Len", cl_len, "Cmd", cl_name, "Uid", uid, "LocalPassword", local_password);
	for(i=0; i<cmd_len; i++){
		printf("send cl_cmd[%d] = %x\n", cl_cmd[i]);
	}

	if(0 >  assign_cmd_info(&cmd_info, "cl", cmd_len, cl_cmd)){
		printf("assign cmd info error !\n");
		exit(0);
	}
	
	send_cmd_2_orvibo_udp_server(udp_client_info, udp_server_info, &cmd_info, qa_msg_info);

	for(i=0; i<cmd_len; i++){
		printf("wacalie----2222-----cl_cmd[%d]=%x\n", i, qa_msg_info->response->cmd_msg[i]);
	}
	//cl 命令用完之后，赋值给全局cmd str 配置，然后free掉
	if(0 > assign_cmd_tpl(cmd_tpl, bucket_num, "cl", qa_cmd, cmd_len)){
		printf("Assign cmd 2 tpl\n");
		exit(0);
	}
	
	return 0;
}


















/* vim: set ts=4 sw=4 sts=4 tw=100 */
