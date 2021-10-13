#include "SpeedChecking.h"

std::string SpeedChecking::INTERNET_DNS = "8.8.8.8";

SpeedChecking::SpeedChecking() {
    m_client = new TCPClient();
}

SpeedChecking::~SpeedChecking() {
    if (nullptr != m_client) {
        m_client->closeConnection();
        delete m_client;
    }
}

void SpeedChecking::doCheck() {
    while (true) {
        long mtime, seconds, useconds;
        struct timeval stop, start;
        gettimeofday(&start, nullptr);

        while (m_client->connectionState() == false) {
            printf("[SpeedChecking] doCheck [FAILED]: connect to server\n");
            this->setConnection();
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        printf("[SpeedChecking] doCheck..\n");
        std::vector<unsigned char> tmpData = m_client->receivedData();
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
                COBData downData, upData;
                std::vector<unsigned char> downDataSent = downData.downloadData(139.391);
                m_client->sendData(downDataSent);
                std::vector<unsigned char> upDataSent = upData.uploadData(117.451);
                m_client->sendData(upDataSent);
#else
                std::string details, details2;
                bool result2 = this->checkPing(INTERNET_DNS, 1, details2);
                printf("Internet Connection = %s\r\n", result2 ? "OK" : "Fail");

                COBData pingData;
                std::vector<unsigned char> pingDataSent = pingData.pingData(result2);
                m_client->sendData(pingDataSent);

                if (!result2) {
                    break;
                }

                bool result = this->checkSpeed(details);
                printf("Check Speed = %s\r\n", result ? "OK" : "Fail");

                COBData resData;
                std::vector<unsigned char> resDataSent = resData.responseState(result2);
                m_client->sendData(resDataSent);

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
                m_client->sendData(downDataSent);
                std::vector<unsigned char> upDataSent = upData.uploadData(up);
                m_client->sendData(upDataSent);
#endif
            }
        }

        gettimeofday(&stop, nullptr);
        seconds  = stop.tv_sec  - start.tv_sec;
        useconds = stop.tv_usec - start.tv_usec;
        mtime = static_cast<long>((seconds) * 1000.0 + useconds / 1000.0);

        if (mtime < 1000) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 - mtime));
        }
    }
}

void SpeedChecking::startCheckThread() {
    printf("[SpeedChecking] startCheckThread..\n");
    m_checkThread = std::thread(&SpeedChecking::doCheck, this);
    m_checkThread.join();
}

void SpeedChecking::setHostParams(std::string ip, int port) {
    m_ipHost = ip;
    m_portHost = port;
}

void SpeedChecking::setConnection() {
    if (nullptr != m_client) {
        while (m_client->connectionState() == false) {
            long mtime, seconds, useconds;
            struct timeval stop, start;
            gettimeofday(&start, nullptr);
            printf("[SpeedChecking] connect to host [%s] port [%d]\n", m_ipHost.c_str(),
                   m_portHost);

            std::string details;

            while (this->checkPing(m_ipHost.c_str(), 1, details) == false) {
                printf("[SpeedChecking] can not ping to host [%s]\n", m_ipHost.c_str());
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }

            m_client->connectToHost(m_ipHost, m_portHost);

            gettimeofday(&stop, nullptr);
            seconds  = stop.tv_sec  - start.tv_sec;
            useconds = stop.tv_usec - start.tv_usec;
            mtime = static_cast<long>((seconds) * 1000.0 + useconds / 1000.0);

            if (mtime < 1000) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 - mtime));
            }
        }
    }
}

bool SpeedChecking::checkSpeed(std::string &details) {
    // Format a command std::string
    std::string command;
    std::string output;
    command = "speedtest -f json";
    int code = commandExec(command, details);
    return (code == 0);
}

bool SpeedChecking::checkPing(const std::string &address,
                              const int &max_attempts, std::string &details) {
    // Format a command std::string
    std::string command;
    std::string output;
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
