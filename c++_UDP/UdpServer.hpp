#ifndef __UDP_SERVER_HPP__
#define __UDP_SERVER_HPP__ 
#include <iostream>
#include <functional>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef void (*Handler)(const std::string& , const std::string* );

class Sock
{
    private:
        int _sock;          //套接字（网卡）的文件描述符
        std::string _ip;    //ip地址
        int _port;          //端口号
    public:
        Sock() {}
        Sock(const std::string ip,int port):_ip(ip),_port(port)
        {}
        bool Socker()    //创建
        {
            //参数(Ip4协议,支持数据包,缺省为0) 返回套接字
            _sock = socket(AF_INET,SOCK_DGRAM,0);  //创建一个套接字 
            if(_sock < 0){
                std::cerr << "Socker Failed! " << std::endl;       //标准错误
                exit(2);
            }
            return true;
        }
        bool Close()
        {
            close(_sock);
            return true;
        }
        bool Bind()      //绑定
        {
            struct sockaddr_in local;       //Ip4地址
            local.sin_family = AF_INET;     //Ip4的协议族
            local.sin_port = htons(_port);  //主机字节序转成网络字节序(端口号),返回一个2字节的数
            local.sin_addr.s_addr = inet_addr(_ip.c_str());  //ip字符串转成32位的整形
            //参数(套接字,Ip4地址,长度)
            int ret = bind(_sock,(struct sockaddr*)&local,sizeof(local));  //绑定套接字
            if(ret < 0){
                std::cerr<< "Bind Failed!" << std::endl;
                exit(3);
            }
            return true;
        }
        bool RecvFrom(std::string& str,std::string* ip=NULL,int* port=NULL)  //str为输出型参数
        {
            char buf[1024] = {0};
            sockaddr_in peer;
            socklen_t len = sizeof(peer);
            ssize_t sz = recvfrom(_sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&peer,&len);
            if(sz > 0){
                str = buf;   //将buf中的内容放到str中
                if(ip != NULL)
                    *ip = inet_ntoa(peer.sin_addr);
                if(port != NULL)
                    *port = ntohs(peer.sin_port);
                return true;
            }
            return false;
        }
        bool SendTo(std::string& str,const std::string& ip,int port)
        {
            sockaddr_in peer;
            peer.sin_family = AF_INET;
            peer.sin_addr.s_addr = inet_addr(ip.c_str());
            peer.sin_port = htons(port);
            ssize_t sz = sendto(_sock,str.c_str(),str.size(),0,(struct sockaddr*)&peer,sizeof(peer));
            if(sz < 0){
                perror("Sendto Faild!");
                return false;
            }
            return true;
        }
};
//UdpServer通用服务器端
class UdpServer
{
    private:
        Sock _sock;        
    public:
        UdpServer(const std::string& ip, int port)   //传进来ip地址和端口
            :_sock(ip,port)
        {}
        bool InitServer()
        {   
            _sock.Socker();      //创建套接字
            _sock.Bind();       //绑定
             return true;
        }
        bool StartServer(Handler handler)
        {
            while(1)
            {
                std::string str;
                std::string remote_ip;
                int remote_port;
                bool ret = _sock.RecvFrom(str,&remote_ip,&remote_port);  //收到client的信息
                if(!ret){       //接收错误则继续
                    continue;
                }
                std::string resp;
                handler(str,&resp);   //str是接收的内容,resp是响应内容
                _sock.SendTo(resp,remote_ip,remote_port);
             //   std::cout << inet_ntoa(client.sin_addr)<<":"<<ntohs(client.sin_port)<<" "<< str <<std::endl;
            }
            return true;
        }
        ~UdpServer()
        {
            _sock.Close();
        }
};
//UdpClient通用客户端
class UdpClient
{
    private:
        Sock _sock;
        std::string _ip;   //服务器端的ip
        uint16_t _port;    //服务器端的端口号
    public:
        UdpClient(const std::string ip,int port):_sock(),_ip(ip),_port(port)
        {}
        ~UdpClient()
        {
            _sock.Close();
        }
        bool InitClient()
        {
            return _sock.Socker();
        }
        bool RecvFrom(std::string& str)
        {
            return _sock.RecvFrom(str);
        }
        bool SendTo(std::string& buf)
        {
            return _sock.SendTo(buf,_ip,_port);
        }
};
#endif 
