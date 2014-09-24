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


#include "orvibo.h"

_Orvibo_Schema_Config_Map_T orvibo_cmd_schema[] = {
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
	{   "cl", 
		"Head:2,Len:2,Cmd:2,Uid:12,LocalPassword:12", 
		"Head:2,Len:2,Cmd:2,Uid:12,SessionId:4,ErrorCode:1,Status:1",
		0,
	},
	{   "dc", 
		"Head:2,Len:2,Cmd:2,Uid:12,SessionId:4,Order:1", 
		"Head:2,Len:2,Cmd:2,Uid:12,SessionId:4,Status:1",
		0,
	},
	{   "hb", 
		"Head:2,Len:2,Cmd:2,Uid:12,SessionId:4", 
		"Head:2,Len:2,Cmd:2,Uid:12,SessionId:4,Status:1",
		0,
	},
};

int main(){

	_Orvibo_Cmd_Config_T *cmd_conf = NULL;
	unsigned char **cmd_tpl = NULL;
	const unsigned bucket_num = ORVIBO_CMD_HASH_BUCKET_NUM;
	orvibo_init(orvibo_cmd_schema, cmd_conf, cmd_tpl, bucket_num);
	return 0;
}














/* vim: set ts=4 sw=4 sts=4 tw=100 */
