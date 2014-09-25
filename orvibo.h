/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file orvibo.h
 * @author work(zhangyouchang@baidu.com)
 * @date 2014/09/24 15:18:45
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __ORVIBO_H_
#define  __ORVIBO_H_


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdarg.h>

#include "orvibo_cmd.h"
#include "orvibo_socket.h"
#include "orvibo_util.h"
#include "orvibo_remote_control.h"


int orvibo_init(_Orvibo_Schema_Config_Map_T schema_config[], _Orvibo_Cmd_Config_T *cmd_conf, unsigned char **cmd_tpl, const unsigned int bucket_num);

int assign_cmd_tpl(unsigned char **cmd_tpl, const unsigned int bucket_num, unsigned const char *cmd_name, unsigned const char *str_cmd, unsigned const cmd_len);

int assign_cmd_info(_Orvibo_Cmd_Info_T *cmd_info, unsigned const char *cmd_name, unsigned int cmd_len, unsigned const char *cmd_str);







#endif  //__ORVIBO_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
