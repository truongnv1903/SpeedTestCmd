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
#include <vector>

class TCPServer {
  public:
    TCPServer();
    ~TCPServer();

    void openServer(int port);
    void sendData(int sock, std::vector<unsigned char> buff);
    std::vector<unsigned char> receivedData(int sock, int &res);
    bool isConnectionAccepted();
    void *newSocket();
    void closeServer();

  private:
    static int          MAX_CONNECTIONS;
    static int          MAX_BUFF_SIZE;
    int                 m_sockfd;
    int                 *m_connfd;
    int                 m_conn;
    std::vector<int>    m_clients;
    socklen_t           m_len;
    struct sockaddr_in  m_servaddr, m_cli;
};

#endif // SERVER_H
