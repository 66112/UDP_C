#include "UdpServer.hpp"
#include <iostream>
#include <unordered_map>

std::unordered_map<std::string,std::string> g_dict; //创建一个哈希表

void Usage(char* proc)
{
    std::cout << "Usage: " << proc << "ip port" << std::endl;
}

void Translate(std::string& req,std::string* resp)
{
    auto it = g_dict.find(req);
    if(it == g_dict.end()){
        *resp = "未查到！";
        return;
    }
    *resp = it->second;  //it对象的value参数
}
//参数(命令行参数的个数,命令行的每一个参数)
int main(int argc,char* argv[])
{
    if(argc != 3){
        Usage(argv[0]);
        exit(1);
    } 
    g_dict.insert(std::make_pair("hello", "你好")); 
    g_dict.insert(std::make_pair("world", "世界"));  
    g_dict.insert(std::make_pair("c++", "最好的编程语言"));  
    g_dict.insert(std::make_pair("bit", "特别NB")); 
    g_dict.insert(std::make_pair("朱小裕","是猪！哈哈哈"));
    
    UdpServer* sp = new UdpServer(argv[1],atoi(argv[2]));  //argv[1]是ip , argv[2]是端口号
    sp->InitServer();
    sp->StartServer(Translate);
    delete sp;
    return 0;
}
