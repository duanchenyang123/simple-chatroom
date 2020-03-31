/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月29日 星期日 18时16分00秒
 ************************************************************************/

#include "../common/chatroom.h"
#include "../common/tcp_client.h"
#include "../common/common.h"
#include "../common/color.h"

char *conf = "./client.conf";
int sockfd;

char logfile[50]  = {0};
void logout(int signalnum) {
    close(sockfd);
    exit(1);
    printf("recv a signal");
}

int main() {
    int port;
    struct Msg msg;
    char ip[20] = {0};
    port = atoi(get_value(conf,"SERVER_PORT"));
    strcpy(ip, get_value(conf, "SERVER_IP"));
    strcpy(logfile,get_value(conf,"LOG_FILE"));
    printf("ip = %s, port = %d\n",ip, port);
    
    
    if((sockfd = socket_connect(ip, port)) < 0) {
        perror("socket_connect");
        return 1;
    }
    strcpy(msg.from, get_value(conf, "MY_NAME"));
    printf("%s\n",msg.from);
    msg.flag = 2; //2是空开消息 1私聊 3是系统通知
    if(chat_send(msg, sockfd) < 0) {
        return 2;
    }
    
    struct RecvMsg rmsg = chat_recv(sockfd);

    if(rmsg.retval < 0) {
        fprintf(stderr, "Error!\n");
        return 1;
    }

    printf(GREEN"Server" NONE": %s", rmsg.msg.message);

    if(rmsg.msg.flag == 3){
        close(sockfd);
        return 1;
    }
    
    pid_t pid;
    if((pid = fork()) < 0) {
        perror("fork");
    }
    if(pid == 0) {
        sleep(2);
    signal(2, logout);
        system("clear");
        char c = 'a';
        while(c != EOF) {
            printf(L_PINK"Please Tnput Message:"NONE"\n");
            scanf("%[^\n]s", msg.message);
            c = getchar();
            msg.flag = 0;
            if (msg.message[0] == '@') {
                msg.flag = 1;
            }
            chat_send(msg,sockfd);
            memset(msg.message, 0, sizeof(msg.message));
            system("clear");
        }
        close(sockfd);

    }else{
        signal(2, logout);
        freopen(logfile,"r", stdout);
        //FILE* log_fp = fopen(logfile,"w");
        while(1) {
            struct RecvMsg rmsg;
            rmsg = chat_recv(sockfd);
            if (rmsg.msg.flag == 0) 
            //    fprintf(log_fp,BLUE"%s"NONE" : %s\n",rmsg.msg.from,rmsg.msg.message );

            printf(BLUE"%s"NONE ": %s \n", rmsg.msg.from, rmsg.msg.message);
        else if(rmsg.msg.flag == 2) {
            printf(YELLOW"通知信息: "NONE "%s\n", rmsg.msg.message);
        } else if (rmsg.msg.flag == 1){
            printf("私聊 %s: %s\n", rmsg.msg.from, rmsg.msg.message);
        } else {
            printf("Error\n");
        }
        fflush(stdout);
        }
        fflush(stdout);
       // fflush(log_fp);
        wait(NULL);
        close(sockfd);
    }

    return 0;

}

