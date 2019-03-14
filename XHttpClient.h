//客户端与进行Http通信的类
#include "XTcp.hpp"
#include "XHttpResponse.h"
#include <thread>

class XHttpClient
{
public:
    XTcp client;
    bool Start(XTcp client);
    void Main();//线程函数s
    XHttpResponse res;
};