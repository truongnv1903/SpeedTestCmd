#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

class TCPClient
{
public:
    TCPClient();
    ~TCPClient();

    bool connectToHost(std::string host, int port);
    void sendData(std::vector<unsigned char> buff);
    std::vector<unsigned char> receivedData();
    void closeConnection();

private:
    struct sockaddr_in  m_servaddr;
    int                 m_sockfd;
};

#endif // TCPCLIENT_H
