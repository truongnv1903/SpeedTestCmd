#pragma once
#include <stdint.h>
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

class COBData {
  public:

    enum class COBDataKey : unsigned char {
        SpeedQuery      = 0x00,
        PingData        = 0x01,
        DownloadData    = 0x02,
        UploadData      = 0x03
    };

    unsigned char data[128];
    COBData();

    std::vector<unsigned char> queryData();
    std::vector<unsigned char> downloadData(double download);
    std::vector<unsigned char> uploadData(double upload);
    std::vector<unsigned char> pingData(bool ping);
};
