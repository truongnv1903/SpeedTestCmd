#include "SpeedChecking.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    std::string ip = argv[1];
    int port = atoi(argv[2]);

    SpeedChecking *query = new SpeedChecking();
    query->setHostParams(ip, port);
    query->setConnection();
    query->startCheckThread();
    return 0;
}
