#include "UdpServer.hpp"
#include <iostream>

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        std::cerr << "argument error!" << std::endl;
        exit(1);
    }
    UdpClient client(argv[1],atoi(argv[2]));
    client.InitClient();
    while(1){
        std::string word;
        std::cout << "请输入要查询的单词：";
        std::cin >> word;
        if(word == "q"){
            std::cout << "Good Bye!" << std::endl;
            break;
        }
        client.SendTo(word);
        std::string result;
        client.RecvFrom(result);
        std::cout << word <<"的意思是："<< result << std::endl;
    }
    return 0;
}
