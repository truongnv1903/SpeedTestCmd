#ifndef SPEEDCHECKING_H
#define SPEEDCHECKING_H

#include <sstream>
#include <thread>
#include <chrono>
#include <sys/time.h>
#include "json.hpp"
#include "COBData.h"
#include "TCPClient.h"

//#define TEST

using namespace std;
using json = nlohmann::json;

class SpeedChecking {
  public:
    SpeedChecking();
    ~SpeedChecking();

    void doCheck();
    void startCheckThread();
    void setHostParams(std::string ip, int port);
    void setConnection();

  private:
    bool checkSpeed(std::string &details);
    bool checkPing(const std::string &address,
                   const int   &max_attempts,
                   std::string      &details);
    int commandExec(const std::string  &command,
                    std::string        &output,
                    const std::string  &mode = "r");

    TCPClient                   *m_client = nullptr;
    static std::string          INTERNET_DNS;
    std::string                 m_ipHost = "";
    int                         m_portHost = -1;
    std::vector<unsigned char>  m_receivedData;
    bool                        m_start = false;
    std::thread                 m_checkThread;
    json                        m_detail;
};

#endif // SPEEDCHECKING_H
