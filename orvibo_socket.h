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
	char *server_udp_ip;
	int	server_udp_port;
	char *local_udp_ip;
	int *local_udp_port;
	socketfd local_send_fd;
	socketfd local_recv_fd;
	struct sockaddr_in remote_addr;
	struct sockaddr_in local_addr;
}_Orvibo_Udp_Server_Info_T;

int set_socketfd_info(_Orvibo_Udp_Server_Info_T *udp_info, const char *local_udp_ip, const int local_udp_port, const char *server_udp_ip, const int server_udp_port, const int mode);

int send_cmd_2_orvibo_udp_server(_Orvibo_Udp_Server_Info_T *udp_info, const _Orvibo_Cmd_Info_T *cmd_info, _Orvibo_Msg_Info_T *talk);

void close_orvibo_udp_socket(_Orvibo_Udp_Server_Info_T *udp_info);













#endif  //__ORVIBO_SOCKET_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
