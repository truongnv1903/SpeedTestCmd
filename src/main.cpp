#include "SpeedChecking.h"

int main(int argc, char *argv[]) {
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
