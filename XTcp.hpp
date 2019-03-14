#ifndef XTCP_HPP
#define XTCP_HPP

#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class XTcp
{
  public:
    int fd = 0;
    int errorFlag = 0;
    std::string ip = "192.168.1.122";
    unsigned short port = 808; //unsigned short 无符号的8位，0~255
    bool CreateSocket();
    bool BindSocket(unsigned int port);
    bool ConnectSocket(const char *ip, unsigned int port, int seconds);
    bool SetBlock(bool isblock);
    bool ListenSocket();
    XTcp AcceptSocket();
    void CloseSocket();
    int Revc(char *buf, int bufSize);
    int Send(const char *buf, int bufSize);
};
#endif