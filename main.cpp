#include "server.h"

int main(int argc, char *argv[])
{
    WebServer server;
    int PORT = 9190;
    server.init(PORT);

    server.eventListen();
}