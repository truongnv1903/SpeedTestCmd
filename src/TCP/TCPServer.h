#ifndef SERVER_H
#define SERVER_H

#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

class TCPServer
{
public:
    TCPServer();
    ~TCPServer();

    void openServer(int port);
    void readReady();
    void disconnected();

private:
    int m_sockfd;
    int m_connfd;
    socklen_t m_len;
    struct sockaddr_in m_servaddr, m_cli;
};

#endif // SERVER_H
