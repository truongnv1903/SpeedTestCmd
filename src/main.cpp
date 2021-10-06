#include "QueryProccessing.h"

int main(int argc, char *argv[])
{
    TCPClient *client = new TCPClient();
    client->connectToHost("127.0.0.1", 2323);
//    COBData cobdata(0);
//    std::vector<unsigned char> data = cobdata.speedData(32.56, 22.23);
//    client->sendData(data);
    QueryProccessing *query = new QueryProccessing();
    client->startReadData();
    query->startCheck();
    return 0;
}
