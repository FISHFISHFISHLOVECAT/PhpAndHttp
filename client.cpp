#include "XTcp.hpp"

int main()
{
    XTcp client;
    client.CreateSocket();
    client.SetBlock(false);
    client.ConnectSocket("192.168.1.122",8080,10);


    while(1)
    {
        char Sayhello[] = "Hello I come from client!" ;
        client.Send("Hello I come from client!", sizeof(Sayhello));
    }


    return 0;
}
