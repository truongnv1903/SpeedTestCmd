#include "TCPServer.h"

int TCPServer::MAX_CONNECTIONS = 5;
int TCPServer::MAX_BUFF_SIZE = 32;

void *connectionHandler(void *socket_desc);

TCPServer::TCPServer()
    : m_sockfd(-1)
    , m_conn(-1) {
}

TCPServer::~TCPServer() {
}

void TCPServer::openServer(int port) {
    // socket create and verification
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (m_sockfd == -1) {
        printf("[TCPServer] Socket creation failed...\n");
        exit(0);
    } else {
        printf("[TCPServer] Socket successfully created..\n");
    }

    bzero(&m_servaddr, sizeof(m_servaddr));

    // assign IP, PORT
    m_servaddr.sin_family = AF_INET;
    m_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_servaddr.sin_port = htons(static_cast<uint16_t>(port));

    // Binding newly  created socket to given IP and verification
    if (bind(m_sockfd, reinterpret_cast<sockaddr *>(&m_servaddr),
             sizeof(m_servaddr)) != 0) {
        printf("[TCPServer] Socket bind failed... %d\n", m_sockfd);
        return;
    } else {
        printf("[TCPServer] Socket successfully binded..\n");
    }

    // Now server is ready to listen and verification
    if ((listen(m_sockfd, 5)) != 0) {
        printf("[TCPServer] Listen failed...\n");
        return;
    } else {
        printf("[TCPServer] Server listening..\n");
    }

    m_len = sizeof(m_cli);
}

void TCPServer::sendData(int sock, std::vector<unsigned char> buff) {
    printf("Send %d bytes: ", static_cast<int>(buff.size()));

    for (int i = 0; i < static_cast<int>(buff.size()); i++) {
        printf(" %02X", buff.at(static_cast<ulong>(i)));
    }

    printf("\n");

    send(sock, buff.data(), sizeof(buff), 0);
}

std::vector<unsigned char> TCPServer::receivedData(int sock, uint16_t &res) {
    printf("[TCPServer] readData.\n");
    char buff[MAX_BUFF_SIZE];
    bzero(buff, sizeof(buff));
    res = read(sock, buff, sizeof(buff));

    std::vector<unsigned char> buffer;

    if (res > 0) {

        for (int i = 0; i < sizeof(buff); i++) {
            buffer.push_back(buff[i]);
        }

        printf("Received %d bytes: ", buffer.size());

        for (int i = 0; i < buffer.size(); i++) {
            printf(" %02X", buffer.at(i));
        }

        printf("\n");
        return buffer;
    }

    return buffer;
}

bool TCPServer::isConnectionAccepted() {
    // Accept the data packet from client and verification
//    m_conn = accept(m_sockfd, reinterpret_cast<sockaddr *>(&m_cli), &m_len);

    m_conn = accept(m_sockfd, (struct sockaddr *)&m_cli, (socklen_t *)&m_len);

    m_connfd = static_cast<int *>(malloc(1));
    *m_connfd = m_conn;

    if (m_conn < 0) {
        printf("[TCPServer] Server accept failed... %d\n", m_len);
        return false;
    } else {
        printf("[TCPServer] Server accept the client... %d\n", m_len);
        return true;
    }
}

void *TCPServer::newSocket() {
    return reinterpret_cast<void *>(m_connfd);
}

void TCPServer::closeServer() {
    close(m_sockfd);
    close(m_conn);
//    free(m_connfd);
}



