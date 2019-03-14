#include "XHttpResponse.h"
using namespace std;

bool XHttpResponse::SetRequest(std::string request)
{
    //回应HTTP请求 GET请求
    //消息头
    //. /index.html /ff/index.php /ff //index.php?id=1&name=awu
    string src=request;
    string pattern("([[:upper:]]+) /([[:alnum:]]*).?([[:alpha:]]+)?[?]?(.*) (HTTP/1)") ;
    regex r(pattern);//传递进去规则
    smatch mas;//结果集
    regex_search(src,mas,r);
    
    if(mas.size()==0)//匹配失败
    {
        printf("%s failed\n",pattern.c_str());
        return false;
    }
    
    string path("/");
    string type=mas[1];
    path+=mas[2];
    string filetype=mas[3];
    string query=mas[4];
    
    cout<<endl<<"type:"<<type<<endl;
    cout<<"path:"<<path<<endl;
    cout<<"filetype:"<<filetype<<endl;
    cout<<"query:"<<query<<endl;

            
    if(type!="GET")
    {
        cout<<"Not GET command!"<<endl;
        return false;
    }
    
    string filename="";
    if(path=="/")
    {
        filename="/index.html";//默认首页文件,假定没有参数
    }
    else if(path=="/favicon")
    {
        filename="/index.html";
        path="/index";
        filetype="html";
    }
    else
    {
        filename=path;
        filename+=".";
        filename+=filetype;
        cout<<"not / and /index.html but"<<" path is "<<filename<<endl;
    }
    
    string filepath="www";//一般放在网站配置上
    filepath+=filename;
    
    if(filetype=="php")
    {
        for(int i=0;i<query.size();i++)
        {
            if(query[i]=='&') 
                query[i]=' ';
        }
        //执行 php-cgi www/index.php 参数1 参数2 > www/index.php.html(临时变量)
        string cmd ="php-cgi ";
        cmd+=filepath;
        cmd+=" "; 
        cmd+=query;
        cmd+=" > ";
        cmd+=filepath;
        cmd+=".html";
        printf("excuting the command: %s\n",cmd.c_str());
        system(cmd.c_str());
        filepath+=".html";
    }
    
    fp=fopen(filepath.c_str(),"rb");
    if(fp==NULL)
    {
        printf("cannot open file: %s",filepath.c_str());
        return false;//一般而言应该返回404错误，课后自己做
    }
    //获取文件大小
    fseek(fp,0,SEEK_END);//移到结尾处
    filesize =ftell(fp);//4G
    printf("file size is %d",filesize);
    fseek(fp,0,0);//回到开始位置

    if(filetype=="php")
    {
        char c=0;
        int HeadSzie=0;
        while(fread(&c,1,1,fp)>0)
        {
            HeadSzie++;
            if(c=='\r')
            {
                fseek(fp,3,SEEK_CUR);
                HeadSzie+=3;
                break;
            }
        }   
        filesize=filesize-HeadSzie;
    }    
    return true;
}

std::string XHttpResponse::GetHead()
{
    string rmsg="";
    rmsg="HTTP/1.1 200 OK\r\n";
    rmsg+="Server: XHttp\r\n";
    rmsg+="Content-Type: text/html\r\n";
    rmsg+="Content-Length: ";
    char bsize[128]={0};
    sprintf(bsize,"%d",filesize);
    rmsg+=bsize;
    rmsg+="\r\n\r\n";
    
    return rmsg;
}

int XHttpResponse::Read(char * buf,int bufsize)
{
    return fread(buf,1,sizeof(buf),fp);
}