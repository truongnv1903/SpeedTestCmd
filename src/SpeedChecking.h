#ifndef SPEEDCHECKING_H
#define SPEEDCHECKING_H

#include <sstream>
#include <thread>
#include <pthread.h>
#include <chrono>
#include <sys/time.h>
#include "json.hpp"
#include "COBData.h"
#include "TCPServer.h"

//#define TEST

using namespace std;
using json = nlohmann::json;

class SpeedChecking {
  public:
    SpeedChecking();
    ~SpeedChecking();

    void startCheckThread();
    void startServer(int port);
    static void *wrapperConnectionHandler(void *pointer);

  private:
    bool checkSpeed(std::string &details);
    bool checkPing(const std::string &address,
                   const int   &max_attempts,
                   std::string      &details);
    int commandExec(const std::string  &command,
                    std::string        &output,
                    const std::string  &mode = "r");
    void *connectionHandler();

    TCPServer *m_server = nullptr;
    static std::string          INTERNET_DNS;
    std::string                 m_ipHost = "";
    int                         m_portHost = -1;
    std::vector<unsigned char>  m_receivedData;
    bool                        m_start = false;
    json                        m_detail;
};

#endif // SPEEDCHECKING_H
