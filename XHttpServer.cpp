#include "XHttpServer.h"

void XHttpServer::Main()
{
    while(!isexit)//无限循环注意停止
    {
        XTcp client=server.AcceptSocket();//阻塞式接受一个HTTP请求
        if(client.fd <= 0) 
        {
            continue;
        }
        XHttpClient * th = new XHttpClient();//服务器创建HTTP客户端连接
        
        th->Start(client);
     
    }
    std::cout<<"有一个服务器退出"<<std::endl;
}

bool XHttpServer::Start(unsigned short port)
{
    isexit=false;
    server.CreateSocket();
    if(!server.BindSocket(port))
    {
        return false;
    } 
    server.ListenSocket();
    std::thread th(&XHttpServer::Main,this);
    th.detach();

    return true;
}

void XHttpServer::Stop()
{
    isexit=true;
}

