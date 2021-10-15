#include "SpeedChecking.h"

std::string SpeedChecking::INTERNET_DNS = "8.8.8.8";

SpeedChecking::SpeedChecking() {
    m_server = new TCPServer();
}

SpeedChecking::~SpeedChecking() {
    if (nullptr != m_server) {
        m_server->closeServer();
        delete m_server;
    }
}

void SpeedChecking::startCheckThread() {
    printf("[SpeedChecking] startCheckThread..\n");

    while (true) {
        while (m_server->isConnectionAccepted()) {
            pthread_t sniffer_thread;

            if (pthread_create(&sniffer_thread, nullptr,  wrapperConnectionHandler,
                               this) < 0) {
                perror("could not create thread");
                break;
            }

            //Now join the thread , so that we dont terminate before the thread
            //        pthread_join(sniffer_thread, nullptr);
            puts("Handler assigned");
        }

        while (m_server->isConnectionAccepted() == false) {
            printf("[SpeedChecking] wait for client..\n");
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
}

void SpeedChecking::startServer(int port) {
    printf("[SpeedChecking] start server..\n");
    m_server->openServer(port);
}

void *SpeedChecking::wrapperConnectionHandler(void *pointer) {
    SpeedChecking *itseft = static_cast<SpeedChecking *>(pointer);
    return itseft->connectionHandler();
}

/*
 * This will handle connection for each client
 * */
void *SpeedChecking::connectionHandler() {
    //Get the socket descriptor
    printf("[SpeedChecking] connectionHandler..\n");
    void *p = m_server->newSocket();
    int sock = *(int *)p;
//    std::vector<unsigned char> m_receivedData;
    char buff[32];
    int result;
    std::vector<unsigned char> tmpData;

    //Receive a message from client
    while ((tmpData = m_server->receivedData(sock, result)).size() > 0) {
        m_receivedData.insert(m_receivedData.end(), tmpData.begin(), tmpData.end());

        while (m_receivedData.size()) {
            if (m_receivedData.size() >= 8 &&
                    m_receivedData.at(0) == 'C' &&
                    m_receivedData.at(1) == 'O' &&
                    m_receivedData.at(2) == 'B' &&
                    m_receivedData.at(3) == 'V' &&
                    m_receivedData.at(4) == 'M' &&
                    m_receivedData.at(5) ==
                    static_cast<unsigned char>(COBData::COBDataKey::SpeedQuery)) {
                m_start = true;
                m_receivedData.erase(m_receivedData.begin(), m_receivedData.begin() + 8);
            } else {
                m_start = false;
                m_receivedData.erase(m_receivedData.begin());
            }

            if (m_start == true) {
                m_start = false;
#ifdef TEST
                COBData pingData;
                std::vector<unsigned char> pingDataSent = pingData.pingData(true);
                m_server->sendData(pingDataSent);
                COBData resData;
                std::vector<unsigned char> resDataSent = resData.responseState(true);
                m_server->sendData(resDataSent);
                COBData downData, upData;
                std::vector<unsigned char> downDataSent = downData.downloadData(139.391);
                m_server->sendData(downDataSent);
                std::vector<unsigned char> upDataSent = upData.uploadData(117.451);
                m_server->sendData(upDataSent);
#else
                std::string details, details2;
                bool result2 = this->checkPing(INTERNET_DNS, 1, details2);
                printf("[SpeedChecking] Internet Connection = %s\r\n", result2 ? "OK" : "Fail");

                COBData pingData;
                std::vector<unsigned char> pingDataSent = pingData.pingData(result2);
                m_server->sendData(sock, pingDataSent);

                if (!result2) {
                    break;
                }

                bool result = this->checkSpeed(details);
                printf("[SpeedChecking] Check Speed = %s\r\n", result ? "OK" : "Fail");

                COBData resData;
                std::vector<unsigned char> resDataSent = resData.responseState(result2);
                m_server->sendData(sock, resDataSent);

                if (!result) {
                    break;
                }

                m_detail = json::parse(details);
                json jdown, jup;
                jdown = m_detail.value("download", json::object());
                jup = m_detail.value("upload", json::object());
                double down, up;
                down = jdown.value("bytes", 0.0) / (1024.0 * 1024.0);
                up = jup.value("bytes", 0.0) / (1024.0 * 1024.0);
                printf("==> Download: %f Mbps|| <== Upload: %f Mbps\r\n", down, up);
                COBData downData, upData;
                std::vector<unsigned char> downDataSent = downData.downloadData(down);
                m_server->sendData(sock, downDataSent);
                std::vector<unsigned char> upDataSent = upData.uploadData(up);
                m_server->sendData(sock, upDataSent);
#endif
            }
        }
    }

    if (result == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (result == -1) {
        perror("recv failed");
    }

    //Free the socket pointer
    free(p);

    return nullptr;
}

bool SpeedChecking::checkSpeed(std::string &details) {
    // Format a command std::string
    std::string command;
    command = "speedtest -f json";
    int code = commandExec(command, details);
    return (code == 0);
}

bool SpeedChecking::checkPing(const std::string &address,
                              const int &max_attempts, std::string &details) {
    // Format a command std::string
    std::string command;
    command = "ping -w 1 -c " + std::to_string(max_attempts) + " " + address +
              " 2>&1";
    int code = commandExec(command, details);
    return (code == 0);
}

int SpeedChecking::commandExec(const std::string  &command,
                               std::string        &output,
                               const std::string  &mode) {
    // Create the std::stringstream
    std::stringstream sout;
    // Run Popen
    FILE *in;
    char buff[512];

    // Test output
    if (!(in = popen(command.c_str(), mode.c_str()))) {
        return 1;
    }

    // Parse output
    while (fgets(buff, sizeof(buff), in) != nullptr) {
        sout << buff;
    }

    int exit_code = pclose(in);
    // set output
    output = sout.str();
    // Return exit code
    return exit_code;
}
