#include "TCPClient.h"

TCPClient::TCPClient() {
}

TCPClient::~TCPClient() {
}

bool TCPClient::connectToHost(std::string host, int port) {
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(m_sockfd == -1) {
        printf("[TCPClient] Socket creation failed...\n");
        return false;
    } else {
        printf("[TCPClient] Socket successfully created..\n");
    }
    bzero(&m_servaddr, sizeof (m_servaddr));

    // assign IP, PORT
    m_servaddr.sin_family = AF_INET;
    m_servaddr.sin_addr.s_addr = inet_addr(host.c_str());
    m_servaddr.sin_port = htons(static_cast<uint16_t>(port));

    // connect the client socket to server socket
    if(connect(m_sockfd, reinterpret_cast<sockaddr*>(&m_servaddr), sizeof (m_servaddr)) != 0) {
        printf("[TCPClient] Connection with the server failed...\n");
        return false;
    }
    else
        printf("[TCPClient] Connected to the server..\n");
    return true;
}

void TCPClient::sendData(std::vector<unsigned char> buff) {
    write(m_sockfd, buff.data(), sizeof(buff));
}

std::vector<unsigned char> TCPClient::receivedData() {
    printf("[TCPClient] readData..\n");
    unsigned char *buff = nullptr;
//        bzero(buff, sizeof (buff));
    read(m_sockfd, buff, sizeof(buff));
//        printf("From Server: %s", buff);
    std::vector<unsigned char> buffer[sizeof(buff)];
    for (int i=0; i<sizeof(buff); i++)
        buffer->push_back(buff[i]);
    printf("\nReceived %d bytes: ", buffer->size());
    for (int i = 0; i < buffer->size(); i++) {
        printf(" %02X", buffer->at(i));
    }
    printf("\n");
    return buffer[buffer->size()];
}

void TCPClient::closeConnection() {
    close(m_sockfd);
}
