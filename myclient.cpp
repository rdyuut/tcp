#include<stdio.h>  
#include<stdlib.h>                                                                                                                                                                                       
#include<unistd.h>                                                                                                                                                                                       
#include<string.h>                                                                                                                                                                                       
#include<arpa/inet.h>                          

int main(){
    //1. 建立套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("socket");
        return -1;
    }
    //与服务器进行连接
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(10000);
    //把点分十进制IP转为大端，并储存到saddr
    inet_pton(AF_INET,"172.31.119.209",&saddr.sin_addr.s_addr);

    int ret =connect(fd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret==-1){
        perror("connect");
        return -1;
    }



    // 与服务器进行通信
    int num=0;
    while(1){

        // 申请内存块，当作缓冲区
        char buff[1024];
        //发送数据
        sprintf(buff,"你好，hello world,%d...\n",num++);
        send(fd,buff,strlen(buff)+1,0);

        //把buff内存块置0
        memset(buff,0,sizeof(buff));
        //读缓冲区中的内容
        int len = recv(fd,buff,sizeof(buff),0);
        if(len>0){
            printf("server say：%s\n",buff);
        }
        else if(len==0){
            printf("服务器已经断开连接");
            break;
        }else{
            perror("recv");
            break;
        }
        sleep(1);

    }
    //断开连接，关闭套接字

    close(fd);


    return 0;
}
