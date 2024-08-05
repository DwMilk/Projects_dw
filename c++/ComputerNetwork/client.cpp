#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
using namespace std;
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080

int main()
{
    /* 创建套接字 */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error");
        _exit(-1);
    }
    cout << "sockfd:" << sockfd << endl;

    /* 连接服务器 */
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);

    const char *ip = "192.168.23.128";
    inet_pton(AF_INET, ip, &(serverAddress.sin_addr.s_addr));

    int ret = connect(sockfd, reinterpret_cast<const sockaddr *>(&serverAddress), sizeof(serverAddress));
    if (ret != 0)
    {
        perror("connect error:");
        _exit(-1);
    }
    char writebuffer[128] = {0};
    char readbuffer[128] = {0};
    while (1)
    {
        cout << "请输入要发送的信息:";
        cin >> writebuffer;
        write(sockfd, writebuffer, strlen(writebuffer) + 1);

        /* 休眠 */
        sleep(2);

        read(sockfd,readbuffer,sizeof(readbuffer));
        cout << "server:" << readbuffer << endl;
    }
    return 0;
}