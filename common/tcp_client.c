/*************************************************************************
	> File Name: tcp_clinet.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月28日 星期六 17时54分06秒
 ************************************************************************/
#include "head.h"

int socket_connect(char *host, int port) {
	int sockfd;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);


    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    printf("socket creat\n");
    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        return -1;
    }
	return sockfd; 

}
