#include "SpeedChecking.h"

int main(int argc, char *argv[]) {
//    if (argc < 3) {
//        fprintf(stderr, "usage %s hostname port\n", argv[0]);
//        exit(0);
//    }

//    std::string ip = argv[1];
//    int port = atoi(argv[2]);

//    SpeedChecking *query = new SpeedChecking();
//    query->setHostParams(ip, port);
//    query->setConnection();
//    query->startCheckThread();

    if (argc < 2) {
        fprintf(stderr, "usage %s port\n", argv[0]);
        return 0;
    }

    int port = atoi(argv[1]);

    SpeedChecking *query = new SpeedChecking();
    query->startServer(port);
    query->startCheckThread();

    return 0;
}
