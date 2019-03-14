#include <string>
#include <regex>
#include <iostream>
class XHttpResponse
{
public:
    bool SetRequest(std::string request);//用户请求信息
    std::string GetHead();//获取HTTP协议头信息
    int Read(char * buf,int bufsize);
    int filesize;
    FILE * fp=NULL;
    std::string rmsg;
};