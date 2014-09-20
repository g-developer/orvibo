/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/



/**
 * @file orvibo_socket.c
 * @author work(zhangyouchang@baidu.com)
 * @date 2014/09/20 12:44:20
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

#include "orvibo_remote_control.h"
#include "orvibo_socket.h"



//{{{int set_socketfd_info(_Orvibo_Udp_Server_Info_T *udp_info, const char *local_udp_ip, const int local_udp_port, const char *server_udp_ip, const int server_udp_port)
/**
 * @brief 
 *
 * @param [in/out] udp_info   : _Orvibo_Udp_Server_Info_T*
 * @param [in/out] local_udp_ip   : const char*
 * @param [in/out] local_udp_port   : const int
 * @param [in/out] server_udp_ip   : const char*
 * @param [in/out] server_udp_port   : const int
 * @return  int 
 * @retval   
 * @see 
 * @author work
 * @date 2014/09/20 12:57:22
 **/
int set_socketfd_info(_Orvibo_Udp_Server_Info_T *udp_info, const char *local_udp_ip, const int local_udp_port, const char *server_udp_ip, const int server_udp_port, const int mode){
	if(NULL == local_udp_ip && NULL == server_udp_ip){
		return -1;
	}

	// create recvfd first, it's local udp server
	if(NULL != local_udp_ip){
		int local_udp_ip_len = strlen(local_udp_ip) + 1;
		udp_info->local_udp_ip = (char*)malloc((local_udp_ip_len) * sizeof(char));
		memset(udp_info->local_udp_ip, 0, local_udp_ip_len);
		snprintf(udp_info->local_udp_ip, local_udp_ip_len, "%s", local_udp_ip);

		if(0 == local_udp_port){
			udp_info->local_udp_port = ORVIBO_UDP_BROADCAST_PORT;
		}else{
			udp_info->local_udp_port = local_udp_port;
		}
		udp_info->local_recv_fd = socket(PF_INET, SOCK_DGRAM, 0);
		if(0 > udp_info->local_recv_fd){
			return 1;
		}
		memset(&udp_info->local_addr, 0, sizeof(udp_info->local_addr));
		udp_info->local_addr.sin_family = AF_INET;
		udp_info->local_addr.sin_addr.s_addr = inet_addr(udp_info->local_udp_ip);
		udp_info->local_addr.sin_port = htons(udp_info->local_udp_port);
		if(0 > bind(udp_info->local_recv_fd, (struct sockaddr*)&(udp_info->local_addr), sizeof(udp_info->local_addr))){
			return 2;
		}
	}

	if(NULL != server_udp_ip){
		int server_udp_ip_len = strlen(server_udp_ip) + 1;
		udp_info->server_udp_ip = (char*)malloc((server_udp_ip_len) * sizeof(char));
		//memset(udp_info->server_udp_ip, 0, server_udp_ip_len);
		snprintf(udp_info->server_udp_ip, server_udp_ip_len, "%s", server_udp_ip);
		if(0 == server_udp_port){
			udp_info->server_udp_port = ORVIBO_UDP_BROADCAST_PORT;
		}else{
			udp_info->server_udp_port = server_udp_port;
		}

		udp_info->local_send_fd = socket(PF_INET, SOCK_DGRAM, 0);
		if(0 > udp_info->local_send_fd){
			return 3;
		}
		memset(&udp_info->remote_addr, 0, sizeof(udp_info->remote_addr));
		udp_info->remote_addr.sin_family = AF_INET;
		udp_info->remote_addr.sin_addr.s_addr = inet_addr(server_udp_ip);
		udp_info->remote_addr.sin_port = htons(server_udp_port);
		if(1 == mode){
			int tmp;
			if(0 > setsockopt(udp_info->local_send_fd, SOL_SOCKET, SO_BROADCAST, &tmp, sizeof(tmp))){
				return 4;
			}
		}

	}
	return 0;
}//}}}

int send_cmd_2_orvibo_udp_server(_Orvibo_Udp_Server_Info_T *udp_info, const _Orvibo_Cmd_Info_T *cmd_info, _Orvibo_Msg_Info_T *talk){
	unsigned char recvBuf[ORVIBO_UDP_MAX_RECV_BYTS] = {'\0'};
	if(NULL == udp_info){
		return -1;
	}
	if(NULL == cmd_info){
		return -1;
	}
	if(NULL == talk){
		return -1;
	}

	int retry = ORVIBO_RETRY_TIME;
	talk->ask = udp_info->local_addr;
	talk->requset = cmd_info;
	//while(retry--){
	while(1){
		if(0 < sendto(udp_info->local_send_fd, (char*)cmd_info->cmd_msg, cmd_info->cmd_len, 0, (struct sockaddr*)&(udp_info->remote_addr), sizeof(udp_info->remote_addr))){
			break;
		}
	}
	int recvByts = 0;
	retry = ORVIBO_RETRY_TIME;
	printf("send : [%s]:[%d]\n", inet_ntoa(udp_info->local_addr.sin_addr), udp_info->local_addr.sin_port);
	printf("recv : [%s]:[%d]\n", inet_ntoa(udp_info->remote_addr.sin_addr), udp_info->remote_addr.sin_port);
	//while(retry--){
	while(1){
		//recvByts = recvfrom(udp_info->local_recv_fd, recvBuf, ORVIBO_UDP_MAX_RECV_BYTS, 0, (struct sockaddr*)&(talk->answer), sizeof(talk->answer));
		recvByts = recvfrom(udp_info->local_recv_fd, recvBuf, ORVIBO_UDP_MAX_RECV_BYTS, 0, NULL, NULL);

		if(0 < recvByts){
			break;
		}
	}
	talk->response->cmd_name = talk->requset->cmd_name;
	talk->response->cmd_msg = (char*)malloc(sizeof(char) * recvByts);
	snprintf(talk->response->cmd_msg, recvByts, "%x", recvBuf);
	talk->response->cmd_len = recvByts;
	return 0;
}


void close_orvibo_udp_socket(_Orvibo_Udp_Server_Info_T *udp_info){
	if(NULL != udp_info){
		if(0 > udp_info->local_recv_fd){
			close(udp_info->local_recv_fd);
		}
		if(0 > udp_info->local_send_fd){
			close(udp_info->local_send_fd);
		}
		if(NULL != udp_info->server_udp_ip){
			free(udp_info->server_udp_ip);
		}
		if(NULL != udp_info->local_udp_ip){
			free(udp_info->local_udp_ip);
		}
		free(udp_info);
	}
}














/* vim: set ts=4 sw=4 sts=4 tw=100 */
