#include "TCPServer.h"

TCPServer::TCPServer() {

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
        printf("[TCPServer] Socket bind failed...\n");
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
}

void TCPServer::readReady() {
    m_len = sizeof(m_cli);
    // Accept the data packet from client and verification
    m_connfd = accept(m_sockfd, reinterpret_cast<sockaddr *>(&m_cli), &m_len);

    if (m_connfd < 0) {
        printf(("[TCPServer] Server accept failed...\n"));
        return;
    } else {
        printf("[TCPServer] Server accept the client...\n");
    }
}

