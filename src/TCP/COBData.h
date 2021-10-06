#pragma once
#include <stdint.h>
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

class COBData
{
public:
    uint8_t data[128];
    COBData(uint8_t addr);

    std::vector<unsigned char> speedData(double download, double upload);
    void calcChecksum();
};
