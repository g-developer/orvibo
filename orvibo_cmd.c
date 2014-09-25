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
#include "orvibo_util.h"


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

void destroy_cmd_config(_Orvibo_Cmd_Config_T *msg_conf, unsigned long bucket_num){
	unsigned int index = 0;
	_Orvibo_Schema_T *tmp = NULL;
	_Orvibo_Schema_T *tmp2 = NULL;
	printf("aaaa\n");
	while(index < bucket_num){
		if(NULL != msg_conf[index].cmd_name){
			printf("index-----------%d\n", index);
			free(msg_conf[index].cmd_name);
			tmp = msg_conf[index].cmd->requset_schema;
			while(NULL != tmp && NULL != tmp->schema){
				tmp2 = tmp->next;
				printf("aaaa---1--------%s\n", tmp->schema->title);
				free(tmp->schema->title);
				free(tmp->schema);
				free(tmp);
				tmp = tmp2;
			}
			//free((msg_conf[index].cmd)->requset_schema);

			tmp = msg_conf[index].cmd->response_schema;
			while(NULL != tmp && NULL != tmp->schema){
				printf("bbbb----2--------%s\n", tmp->schema->title);
				tmp2 = tmp->next;
				free(tmp->schema->title);
				free(tmp->schema);
				free(tmp);
				tmp = tmp2;
			}
			//free(msg_conf[index].cmd)->response_schema;
			free(msg_conf[index].cmd);
		}
		index++;
	}
}

void display_msg(_Orvibo_Msg_Info_T* talk){
	if(NULL == talk){
		printf("\n");
	}else{
		printf("request id[%ld]\n", talk->req_id);
		int i = 0;
		printf("%s:%d send		cmd[%s]:\n", inet_ntoa(talk->ask.sin_addr), talk->ask.sin_port, talk->requset->cmd_name);
		unsigned char* tmp = talk->requset->cmd_msg;
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

int read_orvibo_schema(_Orvibo_Schema_Config_Map_T schema_config[], _Orvibo_Cmd_Config_T *cmd_conf, unsigned long bucket_num){
	if(NULL == schema_config || NULL == cmd_conf){
		return -1;
	}
	_Orvibo_Schema_Config_Map_T *tmp_schema;
	int i = 0;
	int j = 0;
	int k = 0;
	int unit_start = 0, unit_end = 0;
	int hash_index = 0, cmd_name_len = 0;
	tmp_schema = schema_config;

	unsigned char **sp_tmp1 = NULL;
	unsigned char **sp_tmp2 = NULL;
	//while(NULL != tmp_schema->cmd_name){
	while(NULL != schema_config[i].cmd_name){
		j = 0;
		tmp_schema = &(schema_config[i]);
		cmd_name_len = strlen(tmp_schema->cmd_name) + 1;
		//printf("i---temp_schema[%d][cmd_name]		:	%s\n", i, tmp_schema->cmd_name);
		//printf("i---temp_schema[%d][cmd_send_schema]	:	%s\n", i, tmp_schema->cmd_send_schema);
		//printf("i---temp_schema[%d][cmd_recv_schema]	:	%s\n", i, tmp_schema->cmd_recv_schema);
		//printf("i---temp_schema[%d][is_broadcast]	:	%d\n", i, tmp_schema->is_broadcast);
		hash_index = get_schema_hash_num(tmp_schema->cmd_name, bucket_num);

		//赋值 cmd_name
		cmd_conf[hash_index].cmd_name = (unsigned char*)malloc(sizeof(unsigned char) * cmd_name_len);
		snprintf(cmd_conf[hash_index].cmd_name, cmd_name_len, "%s", tmp_schema->cmd_name);
		//是否是广播
		cmd_conf[hash_index].is_broadcast = tmp_schema->is_broadcast;
		//采用Hash桶，根据实际情况，cmd不会出现重复的情况，所以next为NULL，用不到
		cmd_conf[hash_index].next = NULL;
		//为_Orvibo_Cmd_T cmd申请存储空间
		cmd_conf[hash_index].cmd = (_Orvibo_Cmd_T *)malloc(sizeof(_Orvibo_Cmd_T));

		//发送时的schema配置
		_Orvibo_Schema_T *schema_tmp = NULL, *schema_tmp_1 = NULL, *schema_head = NULL;
		sp_tmp1 = split_str_2_arr(tmp_schema->cmd_send_schema, ',');
		(&cmd_conf[hash_index])->cmd->requset_schema = schema_head;
		schema_tmp = schema_head;
		_Orvibo_Schema_Unit_T *unit_tmp = NULL;
		unit_start = unit_end = 0;
		int s = 1;
		while(NULL != sp_tmp1 && NULL != sp_tmp1[j]){

			schema_tmp_1 = (_Orvibo_Schema_T *)malloc(sizeof(_Orvibo_Schema_T) * 1);
			memset(schema_tmp_1, 0, sizeof(_Orvibo_Schema_T) * 1);
			s++;
			schema_tmp_1->count = s + 1;
			schema_tmp_1->schema = NULL;
			schema_tmp_1->next = NULL;

			sp_tmp2 = split_str_2_arr(sp_tmp1[j], ':');
			//printf("sp_tmp2[0]=%s\n", sp_tmp2[0]);
			//printf("sp_tmp2[1]=%ld\n", strtol(sp_tmp2[1], NULL, 10));
			k = 0; 
			//为单元申请空间
			unit_tmp = (_Orvibo_Schema_Unit_T *)malloc(sizeof(_Orvibo_Schema_Unit_T) * 1);
			//为title赋值
			unit_tmp->title = (unsigned char *)malloc(sizeof(unsigned char) * (strlen(sp_tmp2[0]) + 1));
			snprintf(unit_tmp->title, strlen(sp_tmp2[0]) + 1, "%s", sp_tmp2[0]);
			unit_tmp->len = strtol(sp_tmp2[1], NULL, 10);
			unit_tmp->index = j;
			if(0 == j){
				unit_start = 0;
			}else{
				unit_start = unit_end + 1;
			}
			unit_tmp->start = unit_start;
			unit_end = unit_tmp->len - 1 + unit_start;
			unit_tmp->end = unit_end;

			schema_tmp_1->schema = unit_tmp;
			free_str_2_arr(sp_tmp2);


			//printf("1---[%d]-------name[%s]\n", j, schema_tmp_1->schema->title);
			if(0 == j){
				(&cmd_conf[hash_index])->cmd->requset_schema = schema_tmp_1;
				schema_tmp = schema_tmp_1;
			}else{
				schema_tmp->next = schema_tmp_1;
				//printf("2---[%d]-------name[%s]\n", j, schema_tmp->schema->title);
				schema_tmp = schema_tmp->next;
			}
			j++;
		}
		free_str_2_arr(sp_tmp1);

		//接收时的recv的schema配置
		schema_tmp = schema_tmp_1 = schema_head = NULL;
		sp_tmp1 = split_str_2_arr(tmp_schema->cmd_recv_schema, ',');
		(&cmd_conf[hash_index])->cmd->response_schema = schema_head;
		schema_tmp = schema_head;
		unit_tmp = NULL;
		unit_start = unit_end = 0;
		s = 1;
		j = 0;
		while(NULL != sp_tmp1 && NULL != sp_tmp1[j]){
			schema_tmp_1 = (_Orvibo_Schema_T *)malloc(sizeof(_Orvibo_Schema_T) * 1);
			s++;
			schema_tmp_1->count = s + 1;
			schema_tmp_1->schema = NULL;
			schema_tmp_1->next = NULL;
			sp_tmp2 = split_str_2_arr(sp_tmp1[j], ':');
			k = 0; 
			//为单元申请空间
			unit_tmp = (_Orvibo_Schema_Unit_T *)malloc(sizeof(_Orvibo_Schema_Unit_T) * 1);
			//为title赋值
			unit_tmp->title = (unsigned char *)malloc(sizeof(unsigned char) * (strlen(sp_tmp2[0]) + 1));
			snprintf(unit_tmp->title, strlen(sp_tmp2[0]) + 1, "%s", sp_tmp2[0]);
			unit_tmp->len = strtol(sp_tmp2[1], NULL, 10);
			unit_tmp->index = j;
			if(0 == j){
				unit_start = 0;
			}else{
				unit_start = unit_end + 1;
			}
			unit_tmp->start = unit_start;
			unit_end = unit_tmp->len - 1 + unit_start;
			unit_tmp->end = unit_end;

			schema_tmp_1->schema = unit_tmp;
			free_str_2_arr(sp_tmp2);
			if(0 == j){
				(&cmd_conf[hash_index])->cmd->response_schema = schema_tmp_1;
				schema_tmp = schema_tmp_1;
			}else{
				schema_tmp->next = schema_tmp_1;
				//printf("2---[%d]-------name[%s]\n", j, schema_tmp->schema->title);
				schema_tmp = schema_tmp->next;
			}
			j++;
		}
		free_str_2_arr(sp_tmp1);
		i++;
	}
	display_cmd_config(cmd_conf, bucket_num);
	return 0;
}

void display_cmd_config(_Orvibo_Cmd_Config_T *cmd_conf, unsigned long bucket_num){
	unsigned int i = 0;
	unsigned int k = 0;
	_Orvibo_Schema_T *tmp = NULL;
	for(i=0; i<bucket_num; i++){
		if(NULL == cmd_conf[i].cmd_name){
			continue;
		}
		printf("The cmd_conf[%d] Info Below\n", i);
		printf("\tcmd_name				:	%s\n", cmd_conf[i].cmd_name);
		printf("\tis_broadcast				:	%d\n", cmd_conf[i].is_broadcast);
		printf("\trequest_schema count			:	%d\n", cmd_conf[i].cmd->requset_schema->count);
		tmp = cmd_conf[i].cmd->requset_schema;
		k = 0;
		while(NULL != tmp){
			printf("\t\ti[%d]----k[%d]----Unit TiTile		:	%s\n", i, k, tmp->schema->title);
			printf("\t\ti[%d]----k[%d]----Unit Len		:	%d\n", i, k, tmp->schema->len);
			printf("\t\ti[%d]----k[%d]----Unit Index		:	%d\n", i, k, tmp->schema->index);
			printf("\t\ti[%d]----k[%d]----Unit Start		:	%d\n", i, k, tmp->schema->start);
			printf("\t\ti[%d]----k[%d]----Unit End		:	%d\n", i, k, tmp->schema->end);
			tmp = tmp->next;
			k++;
		}
		printf("\tresponse_schema count			:	%d\n", cmd_conf[i].cmd->response_schema->count);
		tmp = cmd_conf[i].cmd->response_schema;
		k = 0;
		while(NULL != tmp){
			printf("\t\ti[%d]----k[%d]----Unit TiTile		:	%s\n", i, k, tmp->schema->title);
			printf("\t\ti[%d]----k[%d]----Unit Len		:	%d\n", i, k, tmp->schema->len);
			printf("\t\ti[%d]----k[%d]----Unit Index		:	%d\n", i, k, tmp->schema->index);
			printf("\t\ti[%d]----k[%d]----Unit Start		:	%d\n", i, k, tmp->schema->start);
			printf("\t\ti[%d]----k[%d]----Unit End		:	%d\n", i, k, tmp->schema->end);
			tmp = tmp->next;
			k++;
		}
		printf("\n");
	}
}

_Orvibo_Cmd_Config_T *get_orvibo_cmd_conf(_Orvibo_Cmd_Config_T *cmd_conf, unsigned long bucket_num, const unsigned char *cmd_name){

	unsigned long hash_index = 0;
	if(NULL == cmd_conf || 0 == bucket_num || NULL == cmd_name){
		return NULL;
	}
	hash_index = get_schema_hash_num(cmd_name, bucket_num);
	if(NULL == &cmd_conf[hash_index]){
		return NULL;
	}else{
		return &cmd_conf[hash_index];
	}
}


_Orvibo_Schema_Unit_T *get_orvibo_schema_segment(_Orvibo_Schema_T *schema_conf, const char *key){
	if(NULL == schema_conf || NULL == key){
		return NULL;
	}
	_Orvibo_Schema_T *tmp = schema_conf;
	while(NULL != tmp){
		if(0 == strcmp(tmp->schema->title, key)){
			break;
		}
		tmp = tmp->next;
	}
	return tmp->schema;
}

unsigned char *set_orvibo_cmd_segment(unsigned char *cmd_str, _Orvibo_Schema_Unit_T *schema_conf, const unsigned char *value){
	if(NULL == cmd_str || NULL == schema_conf || NULL == value){
		return NULL;
	}
	unsigned int index = 0;
	/*
	//if(strlen(value) != schema_conf->len){
	if(sizeof(value) != schema_conf->len){
		return NULL;
	}
	*/
	for(index = 0; index < schema_conf->len; index++){
		cmd_str[index + schema_conf->start] = value[index];
	}
	return cmd_str;
}

/*
//需要手工free
unsigned char *get_orvibo_cmd_segment(unsigned char *cmd_str, _Orvibo_Schema_Unit_T *schema_conf){
	if(NULL == cmd_str || NULL == schema_conf){
		return NULL;
	}
	unsigned int index = 0;
	unsigned char *seg = NULL;
	seg = (unsigned char *)malloc(sizeof(unsigned char) * (schema_conf->len));
	memset(seg, 0, schema_conf->len);
	for(index = 0; index < schema_conf->len; index++){
		seg[index] = cmd_str[index + schema_conf->start];
	}
	return seg;
}
*/

//get_orvibo_cmd_segment(qa_msg_info->response->cmd_msg, schema_config, "qa", "Uid")
unsigned char *get_orvibo_cmd_segment(_Orvibo_Cmd_Config_T *cmd_conf, unsigned const int bucket_num, const unsigned char *resp, unsigned const char* cmd_name, unsigned const char *type, unsigned const char *segment, unsigned int *len){
	if(NULL == resp || NULL == cmd_conf || NULL == cmd_name || NULL == segment || 0 == bucket_num || NULL == type || NULL == len){
		return NULL;
	}
	*len = 0;
	int hash_index = get_schema_hash_num(cmd_name, bucket_num);
	int cmp = strcmp("response", type);
	_Orvibo_Schema_T *schema_tmp = NULL;
	if(0 == cmp){
		schema_tmp = cmd_conf[hash_index].cmd->response_schema;
	}else{
		schema_tmp = cmd_conf[hash_index].cmd->requset_schema;
	}
	while(NULL != schema_tmp){
		if(0 == strcmp(segment, schema_tmp->schema->title)){
			break;
		}else{
			schema_tmp = schema_tmp->next;
		}
	}
	//没有找到: schema 里面没有找到 segment
	if(NULL == schema_tmp){
		return NULL;
	}

	unsigned char *seg = (unsigned char *)malloc(sizeof(unsigned char) * schema_tmp->schema->len);
	memset(seg, 0, sizeof(unsigned char) * schema_tmp->schema->len);
	int index = 0, i = 0;
	for(i=0; i<schema_tmp->schema->len; i++){
		seg[i] = resp[i + schema_tmp->schema->start]; 
	}
	*len = schema_tmp->schema->len;
	return seg;
}

int get_orvibo_cmd_len(_Orvibo_Cmd_Config_T *cmd_conf, const unsigned char *type){
	if(NULL == cmd_conf || NULL == type){
		return -1;
	}
	_Orvibo_Schema_T *schema = NULL;
	int cmp = strcmp("request", type);
	if(0 == cmp){
		schema = cmd_conf->cmd->requset_schema;
	}else{
		schema = cmd_conf->cmd->response_schema;
	}
	int len = 0;
	while(NULL != schema){
		len += schema->schema->len;
		schema = schema->next;
	}
	return len;
}


/* vim: set ts=4 sw=4 sts=4 tw=100 */
