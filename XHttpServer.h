#include "XTcp.hpp"
#include "XHttpClient.h"
#include <thread>

class XHttpServer
{
public:
    bool Start(unsigned short port);
    void Stop();
    void Main();
    bool isexit=true;
    XTcp server;
};