/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file orvibo_socket.h
 * @author work(zhangyouchang@baidu.com)
 * @date 2014/09/19 18:32:27
 * @version $Revision$ 
 * @brief 
 *  
 **/


#ifndef  __ORVIBO_SOCKET_H_
#define  __ORVIBO_SOCKET_H_

#include "orvibo_cmd.h"

typedef int socketfd;

#define		ORVIBO_UDP_BROADCAST_IP		"255.255.255.255"
#define		ORVIBO_UDP_BROADCAST_PORT	10000
#define		ORVIBO_UDP_MAX_RECV_BYTS	1024


typedef struct __Orvibo_Udp_Server_Info_S{
	unsigned char *ip;
	unsigned int port;
	socketfd fd;
	struct sockaddr_in addr;
}_Orvibo_Udp_Server_Info_T;

int gen_orvibo_udp_socket(_Orvibo_Udp_Server_Info_T *udp_info, const char *ip, const int port, const int mode);

int send_cmd_2_orvibo_udp_server(_Orvibo_Udp_Server_Info_T *udp_send_info, _Orvibo_Udp_Server_Info_T *udp_recv_info, const _Orvibo_Cmd_Info_T *cmd_info, _Orvibo_Msg_Info_T *talk);

void close_orvibo_udp_socket(_Orvibo_Udp_Server_Info_T *udp_info);

unsigned const int get_local_ip(socketfd fd, unsigned char *ip);











#endif  //__ORVIBO_SOCKET_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
