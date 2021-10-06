#ifndef SPEEDCHECKING_H
#define SPEEDCHECKING_H

#include <sstream>
#include <thread>
#include "json.hpp"
#include "COBData.h"
#include "TCPClient.h"

using namespace std;
using json = nlohmann::json;

class SpeedChecking
{
    public:
        SpeedChecking();
        ~SpeedChecking();

        void doCheck();
        void startCheckThread();
        void isChecked();

    private:
        bool checkSpeed(std::string &details);
        bool checkPing( const std::string &address,
                        const int   &max_attempts,
                        std::string      &details );
        int commandExec(const std::string&  command,
                        std::string&        output,
                        const std::string&  mode = "r");

        TCPClient           *m_client = nullptr;
        static std::string  INTERNET_DNS;
        bool                m_start = false;
        std::thread         checkThread;
};

#endif // SPEEDCHECKING_H
