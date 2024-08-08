#include <iostream>
using namespace std;
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#define UDP_PORT 9090
#define BUFFER_SIZE 128
int main()
{

    /* 创建套接字 */
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd == -1)
    {
        perror("socket error:");
        _exit(-1);
    }

    int optVal = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    if (ret != 0)
    {
        perror("bind error:");
        throw std::runtime_error("setsocket error.");
    }
    /* 绑定IP和端口 */
    struct sockaddr_in localAddress;
    memset(&localAddress, 0, sizeof(localAddress));
    /* 地址族 */
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(UDP_PORT);
    localAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(sockfd, reinterpret_cast<const sockaddr *>(&localAddress), sizeof(localAddress));
    if (ret != 0)
    {
        perror("bind error:");
        throw std::runtime_error("bind error.");
    }
    char buffer[BUFFER_SIZE] = {0};
    int readBytes = read(sockfd,buffer,sizeof(buffer));
    if(readBytes < 0)
    {
        perror("read error:");
        _exit(-1);
    }
    else if(readBytes == 0)
    {
        cout << "readBytes == 0" << endl;
    }
    else 
    {
        cout << "buffer:" << buffer << endl;
    }
   
    /* 关闭套接字 */
    close(sockfd);
    
    return 0;
}