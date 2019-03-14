#include "XHttpClient.h"

void XHttpClient::Main()
{
    while(1)
    {
        char buf[10240]={0};
        int len=client.Revc(buf,sizeof(buf)-1);
        if(len<=0)
        {
            std::cout<<"接收数据失败"<<std::endl;
            break;
        }

        buf[len]='\0';//使其成为字符串

        if(!res.SetRequest(buf))
        {
            std::cout<<"解析数据失败"<<std::endl;
            break;
        }

        std::string head=res.GetHead();
        if(client.Send(head.c_str(),head.size())<0)
        {
            break;
        }


        //读取正文
        int size=sizeof(buf);
        int error=false;
        while(1)
        {
            int len= res.Read(buf,size);
            if(len<0) 
            {
                error=true;
                break;
            }
            if(client.Send(buf,len)<=0) 
            {
                error=true;
                break;
            }
            if(len==0)//正常到文件末尾
            {
                break;
            }      
        }
    }
    
    client.CloseSocket();
    delete this;
};

bool XHttpClient::Start(XTcp client)//用于处理多个客户端连接
{
    this->client=client;
    std::thread th(&XHttpClient::Main,this);
    th.detach();
    return true;
}


