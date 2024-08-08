#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>

#define UDP_PORT 9090
#define BUFFER_SIZE 128
using namespace std;
int main()
{
    /* 创建套接字 */
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("socket error:");
        exit(-1);
    }

    /* 连接服务器的ip和端口 */
    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(UDP_PORT);
    const char * ip = "192.168.23.134";
    inet_pton(AF_INET,ip,&(serverAddr.sin_addr.s_addr));
    char buffer[BUFFER_SIZE] = "hello world.";
    int writeBytes = sendto(sockfd,buffer,sizeof(buffer),0,reinterpret_cast<const sockaddr*>(&serverAddr),sizeof(serverAddr));
    cout << "wirteBytes:"<< writeBytes << endl;

    return 0;
}