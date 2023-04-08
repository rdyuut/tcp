#include<iostream>
#include<cstring>
#include<unistd.h>
using namespace std;
#include<arpa/inet.h>

int main(){
    //1. 建立套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1){
        perror("socket");
        return -1;
    }
    //将文件描述符与本地ip端口进行绑定
    
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(10000);
    saddr.sin_addr.s_addr=INADDR_ANY;

    int ret = bind(fd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret==-1){
        perror("bind");
        return -1;
    }

    //监听
    ret = listen(fd,128);
    if(ret==-1){
        perror("listen");
        return -1;
    }

    //等待客户端的连接请求，建立新的连接，会得到一个用于通信的文件描述符
    struct sockaddr_in caddr;
    socklen_t clen = sizeof(caddr);

    int cfd = accept(fd,(struct sockaddr*)&caddr,&clen);
    if(cfd==-1){
        perror("accept");
        return -1;
    }
    //打印客户端的地址信息
    char ip[32]={0};
    printf("客户端ip地址：%s,端口：%d\n",
            inet_ntop(AF_INET,&caddr.sin_addr.s_addr,ip,sizeof(ip)),
            ntohs(caddr.sin_port));

    //5. 与客户端进行通信
    while(1){

       // 申请内存块，当作缓冲区
        char buff[1024];
       //把buff内存块置0
        memset(buff,0,sizeof(buff));
        //读缓冲区中的内容
        int re = recv(cfd,buff,sizeof(buff),0);
        if(re>0){
            printf("客户端说：%s\n",buff);
            send(cfd,buff,re,0);
        }
        else if(re==0){
            printf("客户端已经断开连接");
                break;
        }else{
            perror("recv");
            break;
        }
       
    }
    //断开连接，关闭套接字
   
    close(cfd);
    close(fd);


    return 0;
}
