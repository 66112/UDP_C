#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
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
    struct sockaddr_in server;      //Ip4的地址空间
    server.sin_family = AF_INET;    //Ip4协议族
    server.sin_port = htons(atoi(argv[2]));   //Ip4端口号(字符串转成16位整型)
    server.sin_addr.s_addr = inet_addr(argv[1]);  //Ip地址转成32为整型
    if(bind(sock,(struct sockaddr*)&server,sizeof(server)) < 0){
        perror("bind faild!");
        return 3;
    }
    printf("bind success!\n");
    char buf[1024]={0};
    struct sockaddr_in peer;     //接收
    while(1){
        socklen_t len = sizeof(peer);
        printf("Please Enter# ");
        fflush(stdout);
        size_t s = read(0,buf,sizeof(buf)-1);
        if(s > 0){
            buf[s-1]=0; 
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&server,sizeof(server));  //发送
            printf("Please Wait...\n");
            ssize_t sz = recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&peer,&len);
            if(sz > 0){
                buf[sz]=0;
                printf("[%s:%d]: %s\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port),buf);
            }
        }
    }
    return 0;
}
