#include "COBData.h"

COBData::COBData() {
    data[0] = 'C';
    data[1] = 'O';
    data[2] = 'B';
    data[3] = 'V';
    data[4] = 'M';
}

std::vector<unsigned char> COBData::queryData() {
    data[5] = static_cast<unsigned char>(COBDataKey::SpeedQuery);
    data[6] = 1;
    data[7] = '\r';
    vector<unsigned char> result(data, data + 8);
    return result;
}

std::vector<unsigned char> COBData::downloadData(double download) {
    data[5] = static_cast<unsigned char>(COBDataKey::DownloadData);
    char cmd[128];
    sprintf(cmd, "%.03f", download);

    for (int i = 0; i < strlen(cmd); i++) {
        printf("%02X", cmd[i]);

        if (cmd[i] == ',') {
            cmd[i] = '.';
        }

        data[6 + i] = cmd[i];
    }

    printf("\n");

    data[6 + strlen(cmd)] = '\r';
    printf("download = %s\r\n", cmd);
    vector<unsigned char> result(data, data + strlen(cmd) + 7);
    return result;
}

std::vector<unsigned char> COBData::uploadData(double upload) {
    data[5] = static_cast<unsigned char>(COBDataKey::UploadData);
    char cmd[128];
    sprintf(cmd, "%.03f", upload);

    for (int i = 0; i < strlen(cmd); i++) {
        printf("%02X", cmd[i]);

        if (cmd[i] == ',') {
            cmd[i] = '.';
        }

        data[6 + i] = cmd[i];
    }

    printf("\n");

    data[6 + strlen(cmd)] = '\r';
    printf("upload = %s\r\n", cmd);
    vector<unsigned char> result(data, data + strlen(cmd) + 7);
    return result;
}

std::vector<unsigned char> COBData::pingData(bool ping) {
    data[5] = static_cast<unsigned char>(COBDataKey::PingData);
    data[6] = static_cast<unsigned char>(ping);
    data[7] = '\r';
    vector<unsigned char> result(data, data + 8);
    return result;
}
