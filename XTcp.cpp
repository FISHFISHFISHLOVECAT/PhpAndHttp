#include "XTcp.hpp"

bool XTcp::SetBlock(bool isblock)
{
    if(fd<=0) return false;
    int flags=fcntl(fd,F_GETFL,0);//flags标记成功失败
    if (flags < 0)
        return false;
    if(isblock)
    {
        flags=flags&~O_NONBLOCK;//置0
    }
    else
    {
        flags=flags|O_NONBLOCK;//置1
    }

    if (fcntl(fd, F_SETFL, flags) != 0)
        return false;
    return true;
}

void XTcp::CreateSocket()
{
    fd = socket(AF_INET, SOCK_STREAM, 0); //根据前两个参数推断
    if (fd == -1)
        std::cout << "Create socket failed" << std::endl;
    else
        std::cout << "Create socket successfully" << std::endl;
}

void XTcp::BindSocket(unsigned int port)
{
    struct sockaddr_in addr; //定义一个更加方便的结构体

    bzero(&addr, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.1.122"); //由arpa/inet.h支持,字符串转换为长整型
    addr.sin_port = htons(port);                       ///</inet.h>转换成符合标准的数据

    errorFlag = bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
    if (errorFlag == -1)
        std::cout << "Bind socket failed" << std::endl;
    else
        std::cout << "Bind socket successfully" << std::endl;
}

bool XTcp::ConnectSocket(const char *ip, unsigned int port, int seconds)
{
    if (fd <= 0)
        CreateSocket();
    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port); //host 主机 s：short
    saddr.sin_addr.s_addr = inet_addr(ip);
    fd_set set;//fd_set是一个32位的int 型的数组

    if (connect(fd, (sockaddr *)&saddr, sizeof(saddr)) != 0)
    {
        FD_ZERO(&set);
        FD_SET(fd,&set);
        timeval tm;
        tm.tv_sec=seconds;
        tm.tv_usec=0;

        if(select(fd + 1, 0, &set, 0, &tm)<=0)//监听文件描述符的状态改变，如果select满足设置要求，select就返回了文件描述符，超时则返回零，错误-1
        {
            printf("connect timeout or error!\n");
            printf("connect %s:%d failed:%s\n", ip, port, strerror(errno));
            return false;
        }
    }
    
    printf("connect %s:%d successfully\n", ip, port);
    return true;
}

void XTcp::ListenSocket()
{
    errorFlag = listen(fd, 10);
    if (errorFlag == -1)
        std::cout << "Listen socket failed" << std::endl;
    else
        std::cout << "Listen socket successfully" << std::endl;
}

XTcp XTcp::AcceptSocket()
{
    XTcp client_tcp;
    struct sockaddr_in client_addr; //用于建立连接后返回客户端端口号和地址
    bzero(&client_addr, sizeof(sockaddr_in));
    socklen_t len = sizeof(client_addr);

    client_tcp.fd = accept(fd, (sockaddr *)&client_addr, &len); //这结构体都是网络字节序

    if (client_tcp.fd > 0)
    {
        std::cout << "A clinet connected to the server" << std::endl;
        client_tcp.ip = inet_ntoa(client_addr.sin_addr);
        client_tcp.port = ntohs(client_addr.sin_port);
        printf("ip = %s,Port =%d\n", ip.c_str(), port);
    }
    return client_tcp;
}

int XTcp::Send(const char *buf, int bufSize)
{
    int SendLen = 0;
    while (SendLen != bufSize) //若数据发送缺失，尝试重新发送
    {
        int len = send(fd, buf + SendLen, bufSize - SendLen, 0);
        if (len < 0)
            break;      //send出错
        SendLen += len; //数据发送完整，等于退出循环
    }

    return SendLen;
}
int XTcp::Revc(char *buf, int bufSize)
{
    int rec_len = recv(fd, buf, bufSize, 0);
    buf[rec_len] = '\0';
    return rec_len;
}

void XTcp::CloseSocket()
{
    close(fd);
}