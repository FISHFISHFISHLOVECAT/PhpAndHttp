#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "XTcp.hpp"

class TcpTread
{
public:
    void Main()
    {
        char buf[1024] = "";
        while (1)
        {
            t.Revc(buf, sizeof(buf));
            if (strstr(buf, "quit") != NULL)
            {
                char re[] = "Server: quit sucessfully!";
                send(t.fd, re, strlen(re) + 1, 0);
                break;
            }

            std::cout << "Rec from clinet:" << buf;

            char ok[] = "Server: ok\n";
            t.Send(ok, sizeof(ok));
        }
        t.CloseSocket();
        delete this;    
    }
    XTcp t;
};

int main()
{
    XTcp Server;
    Server.CreateSocket();
    Server.BindSocket(8080);
    Server.ListenSocket();
    while(1)
    {
        XTcp t = Server.AcceptSocket();
        TcpTread * ptr=new TcpTread();
        ptr->t=t;
        std::thread th(&TcpTread::Main, ptr);
        th.detach();
    }

    return 0;
}


