/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file orvibo_remote_control.c
 * @author work(zhangyouchang@baidu.com)
 * @date 2014/09/23 15:29:41
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include "orvibo_remote_control.h"
#include "orvibo_cmd.h"

/*
unsigned char *gen_orvibo_query_all_device(unsigned int *cmd_len){
	unsigned const char qa_cmd[] = {0x68, 0x64, 0x00, 0x06, 0x71, 0x61};
	*cmd_len = sizeof(qa_cmd);
	return qa_cmd;
}
*/

//dc_cmd = gen_orvibo_cmd(cmd_conf, 32, "cl", &len, 6, "sessionId", "xxx", "Uid", "000000", "Order", "xxx1");
//需要手动free
unsigned char *gen_orvibo_cmd(_Orvibo_Cmd_Config_T *cmd_conf, const int bucket_num, const unsigned char *cmd_name, unsigned int *cmd_len, int argc, ...){
	//unsigned char cmd[64] = {0x68, 0x64, 0x01, 0x02, 0x71, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	*cmd_len = 0;
	if(NULL == cmd_conf || NULL == cmd_name || 0 == bucket_num || NULL == cmd_len){
		return NULL; 
	}
	int i = 0;
	if(0 == strcmp("qa", cmd_name)){
		unsigned char *cmd = (unsigned char *)malloc(sizeof(unsigned char) * 6);
		memset(cmd, 0, sizeof(unsigned char) * 6);
		unsigned const char qa_cmd[] = {0x68, 0x64, 0x00, 0x06, 0x71, 0x61};
		for(i=0; i<6; i++){
			cmd[i] = qa_cmd[i];
		}
		*cmd_len = 6;
		return cmd;
	}
	if(0 == argc){
		return NULL;
	}
	_Orvibo_Cmd_Config_T *cmd_conf_tmp = get_orvibo_cmd_conf(cmd_conf, bucket_num, cmd_name);
	*cmd_len = get_orvibo_cmd_len(cmd_conf_tmp, "request");
	unsigned char *cmd = (unsigned char *)malloc(sizeof(unsigned char) * (*cmd_len));
	memset(cmd, 0, sizeof(unsigned char) * (*cmd_len));
	i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, argc);
	unsigned char *segment_key[argc/2];
	unsigned char *segment_value[argc/2];
	_Orvibo_Schema_Unit_T *qa_head_unit[argc/2];
	int index = 0;
	while(i < argc){
		index = (int)((i+1) / 2);
		if(0 == i % 2){
			segment_key[index] = va_arg(arg_ptr, unsigned char*);
			qa_head_unit[index] = get_orvibo_schema_segment(cmd_conf_tmp->cmd->requset_schema, segment_key[index]);
		}else{
			segment_value[index - 1] = va_arg(arg_ptr, unsigned char*);
		}
		i++;
	}
	i = 0;
	unsigned char *cmd_tmp = NULL; 
	int j = 0;
	for(i=0; i<index; i++){
		cmd_tmp = set_orvibo_cmd_segment(cmd, qa_head_unit[i], segment_value[i]);
		for(j = 0; j < *cmd_len; j++){
		printf("fuck--1--i[%d]---j[%d]---%x\n", i, j, cmd_tmp[j]);
			cmd[j] = cmd_tmp[j];
		printf("fuck--2--i[%d]---j[%d]---%x\n", i, j, cmd[j]);
		}
	}
	for(j = 0; j < *cmd_len; j++){
		printf("fuck--3--i[%d]---j[%d]---%x\n", i, j, cmd[j]);
	}
	return cmd;
}

unsigned char *modify_orvibo_cmd_segment(_Orvibo_Cmd_Config_T *cmd_conf, const unsigned int bucket_num, unsigned char **cmd_tpl, const unsigned char *cmd_name, const unsigned argc, ...){
	if(NULL == cmd_tpl || NULL == cmd_name){
		return NULL;
	}
	int hash_num = 0;
	hash_num = get_schema_hash_num(cmd_name, bucket_num);
	if(0 == argc){
		return cmd_tpl[hash_num];
	}
	int i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, argc);
	unsigned char *segment_key[argc/2];
	unsigned char *segment_value[argc/2];
	_Orvibo_Schema_Unit_T *qa_head_unit[argc/2];
	_Orvibo_Cmd_Config_T *cmd_conf_tmp = get_orvibo_cmd_conf(cmd_conf, ORVIBO_CMD_HASH_BUCKET_NUM, cmd_name);
	int index = 0;
	while(i < argc){
		index = (int)((i+1) / 2);
		if(0 == i % 2){
			segment_key[index] = va_arg(arg_ptr, unsigned char*);
			qa_head_unit[index] = get_orvibo_schema_segment(cmd_conf_tmp->cmd->requset_schema, segment_key[index]);
		}else{
			segment_value[index - 1] = va_arg(arg_ptr, unsigned char*);
		}
		i++;
	}
	i = 0;
	unsigned char *cmd_tmp = NULL; 
	int j = 0;
	for(i=0; i<index; i++){
		cmd_tmp = set_orvibo_cmd_segment(cmd_tpl[hash_num], qa_head_unit[i], segment_value[i]);
		for(j = (qa_head_unit[i])->start; j < (qa_head_unit[i])->len; j++){
		printf("fuck--1111--i[%d]---j[%d]---%x\n", i, j, cmd_tmp[j]);
			cmd_tpl[hash_num][j] = cmd_tmp[j];
		printf("fuck--2222--i[%d]---j[%d]---%x\n", i, j, cmd_tpl[hash_num][j]);
		}
	}
	return cmd_tpl[hash_num];
}













/* vim: set ts=4 sw=4 sts=4 tw=100 */
