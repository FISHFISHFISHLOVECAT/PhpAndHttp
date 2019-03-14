#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <string>
#include <regex>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "XTcp.hpp"
#include "XHttpServer.h"

using namespace std;

int main(int argc,char * argv[])//arg[0]包含完整路径程序名 arg[1]字符串1
{
    XTcp Server;
    Server.CreateSocket();
    unsigned short port=8080;
    if(argc>1)
    {
        port=atoi(argv[1]);
    }

    XHttpServer server;
    if(!server.Start(port))
    {
        cout<<"服务端启动失败"<<endl;
    }
 
    getchar();
    
    return 0;
}


