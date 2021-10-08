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

class TCPClient {
  public:
    TCPClient();
    ~TCPClient();

    bool connectToHost(std::string host, int port);
    void sendData(std::vector<unsigned char> buff);
    std::vector<unsigned char> receivedData();
    bool checkConnection();
    void closeConnection();
    bool connectionState();

  private:
    static int          MAX_BUFF_SIZE;
    struct sockaddr_in  m_servaddr;
    int                 m_sockfd = -1;
    bool                m_status = false;
};

#endif // TCPCLIENT_H
