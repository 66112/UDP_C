#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
int main(int argc,char* argv[])
{
    if(argc != 3){
        perror("argument error!");
        return 1;
    }
    int sock = socket(AF_INET,SOCK_DGRAM,0);  //创建套接字
    if(sock < 0){
        perror("sock failed!");
        return 2;
    }
    struct sockaddr_in local;      //Ip4的地址空间
    local.sin_family = AF_INET;    //Ip4协议族
    local.sin_port = htons(atoi(argv[2]));   //Ip4端口号(字符串转成16位整型)
    local.sin_addr.s_addr = inet_addr(argv[1]);  //Ip地址转成32为整型
    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){
        perror("bind faild!");
        return 3;
    }
    char buf[1024]={0};
    struct sockaddr_in client;     //接收
    while(1){
        socklen_t len = sizeof(client);
        ssize_t sz = recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&client,&len);
        if(sz > 0){
            buf[sz]=0;
            printf("[%s:%d]: %s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),buf);
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&client,sizeof(client));  //发送
        }
    }
    return 0;
}
