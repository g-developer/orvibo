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
#include<net/if.h>
#include<sys/socket.h>
#include<linux/sockios.h>
#include<sys/ioctl.h>

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

//{{{int gen_orvibo_udp_socket(_Orvibo_Udp_Server_Info_T *udp_info, const char *ip, const int port, const int mode)
/**
 * @brief 
 *
 * @param [in/out] udp_info   : _Orvibo_Udp_Server_Info_T*
 * @param [in/out] ip   : const char*
 * @param [in/out] port   : const int
 * @param [in/out] mode   : const int	0,client udp广播;1,client 非广播;2,server
 * @return  int 
 * @retval   
 * @see 
 * @author work
 * @date 2014/09/25 10:45:47
 **/
int gen_orvibo_udp_socket(_Orvibo_Udp_Server_Info_T *udp_info, const char *ip, const int port, const int mode){
	if(NULL == udp_info || NULL == ip || 0 == port){
		return -1;
	}
	int ip_len = strlen(ip) + 1;
	udp_info->ip = (unsigned char*)malloc((ip_len) * sizeof(unsigned char));
	memset(udp_info->ip, 0, ip_len);
	snprintf(udp_info->ip, ip_len, "%s", ip);

	if(0 == port){
		udp_info->port = ORVIBO_UDP_BROADCAST_PORT;
	}else{
		udp_info->port = port;
	}
	udp_info->fd = socket(PF_INET, SOCK_DGRAM, 0);
	if(0 > udp_info->fd){
		return -2;
	}
	memset(&udp_info->addr, 0, sizeof(udp_info->addr));
	udp_info->addr.sin_family = AF_INET;
	udp_info->addr.sin_addr.s_addr = inet_addr(udp_info->ip);
	udp_info->addr.sin_port = htons(udp_info->port);

	if(2 == mode){
		printf("bind info [%s] [%d] [fd]\n", udp_info->ip, udp_info->port, udp_info->fd);
		if(0 > bind(udp_info->fd, (struct sockaddr*)&(udp_info->addr), sizeof(udp_info->addr))){
			perror("bind error!\n");
			return -3;
		}
	}else{
		if(0 == mode){
			int tmp;
			if(0 > setsockopt(udp_info->fd, SOL_SOCKET, SO_BROADCAST, &tmp, sizeof(tmp))){
				return -4;
			}
		}

	}
	return udp_info->fd;
}//}}}//}}}

int send_cmd_2_orvibo_udp_server(_Orvibo_Udp_Server_Info_T *udp_send_info, _Orvibo_Udp_Server_Info_T *udp_recv_info, const _Orvibo_Cmd_Info_T *cmd_info, _Orvibo_Msg_Info_T *talk){
	if(NULL == udp_send_info){
		return -1;
	}
	if(NULL == udp_recv_info){
		return -1;	
	}
	if(NULL == cmd_info){
		return -1;
	}
	if(NULL == talk){
		return -1;
	}

	unsigned char recvBuf[ORVIBO_UDP_MAX_RECV_BYTS] = {'\0'};
	int retry = ORVIBO_RETRY_TIME;
	talk->ask = udp_send_info->addr;
	talk->answer = udp_recv_info->addr;
	talk->requset = (_Orvibo_Cmd_Info_T*)malloc(sizeof(_Orvibo_Cmd_Info_T));
	memset(talk->requset, 0, sizeof(_Orvibo_Cmd_Info_T));
	int cmd_name_len = strlen(cmd_info->cmd_name) + 1;
	//talk->requset->cmd_name = (unsigned char*)malloc(cmd_name_len);
	snprintf(talk->requset->cmd_name, cmd_name_len, "%s", cmd_info->cmd_name);
	//talk->requset->cmd_msg = (unsigned char*)malloc(sizeof(unsigned char) * cmd_info->cmd_len);
	int i = 0;
	for(i=0; i<cmd_info->cmd_len; i++){
		talk->requset->cmd_msg[i] = cmd_info->cmd_msg[i];
	}
	talk->requset->cmd_len = cmd_info->cmd_len;
	//talk->requset = cmd_info;
	//while(retry--){
	while(1){
		if(0 < sendto(udp_send_info->fd, (char*)cmd_info->cmd_msg, cmd_info->cmd_len, 0, (struct sockaddr*)&(udp_send_info->addr), sizeof(udp_send_info->addr))){
			break;
		}
	}
	int recvByts = 0;
	retry = ORVIBO_RETRY_TIME;
	printf("send : [%s]:[%d]\n", inet_ntoa(udp_send_info->addr.sin_addr), udp_send_info->addr.sin_port);
	for(i=0; i<cmd_info->cmd_len; i++){
		printf("socket send : cmd_msg[%d] = %x\n", cmd_info->cmd_msg[i]);
	}
	printf("recv : [%s]:[%d]\n", inet_ntoa(udp_recv_info->addr.sin_addr), udp_recv_info->addr.sin_port);
	memset(recvBuf, '\0', sizeof(recvBuf));
	//while(retry--){
	while(1){
		recvByts = recvfrom(udp_recv_info->fd, recvBuf, ORVIBO_UDP_MAX_RECV_BYTS, 0, (struct sockaddr*)&(talk->answer), &(talk->answer));
		//recvByts = recvfrom(udp_recv_info->fd, recvBuf, ORVIBO_UDP_MAX_RECV_BYTS, 0, NULL, NULL);
		for(i=0; i<recvByts; i++){
			printf("get---[%s]---recvBuf[%d] = [%x]\n", talk->requset->cmd_name, i, recvBuf[i]);
		}
		if(0 < recvByts){
			break;
		}
	}
	talk->response = (_Orvibo_Cmd_Info_T*)malloc(sizeof(_Orvibo_Cmd_Info_T));
	memset(talk->response, 0, sizeof(_Orvibo_Cmd_Info_T));
	//talk->response->cmd_name = (unsigned char*)malloc(sizeof(unsigned char) * cmd_name_len);
	snprintf(talk->response->cmd_name, cmd_name_len, "%s", talk->requset->cmd_name);

	//talk->response->cmd_msg = (unsigned char*)malloc(sizeof(unsigned char) * recvByts);
	//snprintf(talk->response->cmd_msg, recvByts, "%x", recvBuf);

	printf("answer : [%s]:[%d]\n", inet_ntoa(talk->answer.sin_addr), talk->answer.sin_port);

	for(i=0; i<recvByts;i++){
		talk->response->cmd_msg[i] = recvBuf[i];
	}

	talk->response->cmd_len = recvByts;

	return 0;
}


unsigned const int get_local_ip(socketfd fd, unsigned char *ip){
	if(NULL == ip || 0 > fd){
		return -1;
	}
	struct ifconf conf;
	struct ifreq *ifr;
	char buff[BUFSIZ];
	int num;
	int i;
	conf.ifc_len = BUFSIZ;
	conf.ifc_buf = buff;
	ioctl(fd, SIOCGIFCONF, &conf);
	num = conf.ifc_len / sizeof(struct ifreq);
	unsigned char *local_ip = NULL;
	ifr = conf.ifc_req;
	for(i=0;i < num;i++){
		struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);
		ioctl(fd, SIOCGIFFLAGS, ifr);
		if(((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP)){
			local_ip = inet_ntoa(sin->sin_addr);
			if(strstr(local_ip, "192.168")){
				snprintf(ip, strlen(local_ip) + 1, "%s", local_ip);
				return 0;
			}
		}
		ifr++;
	}
	return -1;
}

void close_orvibo_udp_socket(_Orvibo_Udp_Server_Info_T *udp_info){
	if(NULL != udp_info){
		if(0 > udp_info->fd){
			close(udp_info->fd);
		}
		if(NULL != udp_info->ip){
			free(udp_info->ip);
		}
		free(udp_info);
	}
}














/* vim: set ts=4 sw=4 sts=4 tw=100 */
