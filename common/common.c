/*************************************************************************
	> File Name: common.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月29日 星期日 11时57分06秒
 ************************************************************************/
#include"head.h" 

char *get_value(char *path, char *key) {
    FILE *fp = NULL;
    ssize_t nrd;//读到多少字符
    char *line = NULL, *sub = NULL;
    extern char conf_ans[50];
    size_t linecap;
    if (path == NULL || key == NULL) {
        fprintf(stderr, "Error in argument!\n");
        return NULL;
    }
    if((fp = fopen(path, "r")) == NULL) {
        perror("fopen");
        return NULL;
    } 
    
    while((nrd = getline(&line, &linecap,fp)) != -1) {//第一个参数是char**型，从第三个FILE型指针处读读到line这个二维指针指向的数组，然后通过第二个参数指向的大小动态开辟第一个参数指向的数组的空间。
        if((sub = strstr(line, key)) == NULL) //sub 是偏移地址
            continue;
        else {
            if(line[strlen(key)] == '=') {
                strncpy(conf_ans, sub + strlen(key) + 1, nrd - strlen(key) - 2);
                *(conf_ans + nrd - strlen(key) -2) = '\0';
                break;
            }
        }
    }
    free(line);
    fclose(fp);
    if(sub == NULL) {
        return NULL;
    }
    return conf_ans;
}



void make_nonblock_ioctl(int fd) {
    unsigned long ul = 1;
    ioctl(fd,FIONBIO, &ul);
}
void make_block_ioctl(int fd) {
    unsigned long ul = 0;
    ioctl(fd,FIONBIO, &ul);
}

void make_nonblock(int fd) {
    int flags;
    if((flags = fcntl(fd, F_GETFL, 0)) < 0){
        return ;
    }
    flags |= O_NONBLOCK;
    fcntl(fd,F_SETFL, flags);
}

void make_block(int fd) {
    int flags;
    if((flags = fcntl(fd, F_GETFL, 0)) < 0) {
        return ;
    }
    flags &= ~O_NONBLOCK;
    fcntl(fd,F_SETFL, flags);
}

