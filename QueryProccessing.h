#ifndef QUERYPROCCESSING_H
#define QUERYPROCCESSING_H

#include <iostream>
#include <vector>
#include <cstdio>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#ifdef __linux__
    //linux code goes here
    #include <sys/time.h>
#elif _WIN32
    // windows code goes here
#else

#endif

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class QueryProccessing
{
    public:
        QueryProccessing();
        ~QueryProccessing();

        void start();
        void stopChecking();

    private:
        bool    checkSpeed(std::string &details );
        void    msleep(int ms);

    private:
        bool    m_stop = false;
};

#endif // QUERYPROCCESSING_H
