#include "COBData.h"

COBData::COBData(uint8_t addr) {
    data[0] = 'C';
    data[1] = 'O';
    data[2] = 'B';
    data[3] = 'V';
    data[4] = 'M';
}

std::vector<unsigned char> COBData::speedData(double download, double upload) {
    char cmd[128];
    sprintf(cmd, "%.03f", download);

    for (int i = 0; i < strlen(cmd); i++) {
        if (cmd[i] == ',') {
            cmd[i] = '.';
        }

        data[5 + i] = cmd[i];
    }

    data[5 + strlen(cmd)] = '\r';
    printf("download = %s\r\n", cmd);
    vector<unsigned char> result(data, data + strlen(cmd) + 7);
    return result;
}

void COBData::calcChecksum()
{
	uint16_t sum = 0;
	for(int i = 1; i < 6; ++i)
	{
		sum += data[i];
	}
	sum = sum % 256;
	data[6] = (uint8_t)sum;
}
