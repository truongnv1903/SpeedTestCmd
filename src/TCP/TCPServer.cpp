#include "TCPServer.h"

int TCPServer::MAX_BUFF_SIZE = 32;

TCPServer::TCPServer()
    : m_sockfd(-1)
    , m_connfd(-1) {
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

    this->readReady();
}

void TCPServer::sendData(std::vector<unsigned char> buff) {
    printf("Send %d bytes: ", buff.size());

    for (int i = 0; i < buff.size(); i++) {
        printf(" %02X", buff.at(i));
    }

    printf("\n");
    write(m_connfd, buff.data(), sizeof(buff));
}

std::vector<unsigned char> TCPServer::receivedData() {
    printf("[TCPServer] readData.\n");
    char buff[MAX_BUFF_SIZE];
    bzero(buff, sizeof(buff));
    uint16_t res = read(m_connfd, buff, sizeof(buff));
    printf("====> m_connfd: %d || res: %d\n", m_connfd || res);

    if (res > 0) {
        m_status = true;
    } else {
        m_status = false;
    }

    std::vector<unsigned char> buffer;

    if (res > 0) {

        for (int i = 0; i < sizeof(buff); i++) {
            buffer.push_back(buff[i]);
        }

        printf("Received %d bytes :", buffer.size());

        for (int i = 0; i < buffer.size(); i++) {
            printf(" %02X", buffer.at(i));
        }

        printf("\n");
        return buffer;
    }

    return buffer;
}

void TCPServer::readReady() {
    m_len = sizeof(m_cli);
    // Accept the data packet from client and verification
    m_connfd = accept(m_sockfd, reinterpret_cast<sockaddr *>(&m_cli), &m_len);

    if (m_connfd < 0) {
        printf(("[TCPServer] Server accept failed...\n"));
        return;
    } else {
        printf("[TCPServer] Server accepted the client...\n");
    }
}

bool TCPServer::checkToClientExist() {
//    this->readReady();

    if (m_connfd > 0) {
        return true;
    } else {
        return false;
    }
}

void TCPServer::closeServer() {
    close(m_sockfd);
}

