#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>


void send_to_client(int &client, char* msg, FILE* log) {
    char buffer[1024] = {};

    fprintf(stdout, "[ShadowDedulet] %s\n", msg);
    fprintf(log, "[ShadowDedulet] %s\n", msg);

    sprintf(buffer, "%s\n", msg);
    write(client, buffer, strlen(buffer));
}

int main(int argc, char *argv[]) {
    char* SERVER_NAME = "ShadowDedulet";

    // log out all events
    FILE *log;
    log = fopen("./server.log", "w");

    fprintf(stdout, "starting server %s...\n", SERVER_NAME);
    fprintf(log, "starting server %s...\n", SERVER_NAME);

    unlink(SERVER_NAME);

    int listenFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listenFd < 0) {
        fprintf(stdout, "ERROR: socket creation failure\n");
        fprintf(log, "ERROR: socket creation failure\n");
        exit(1);
    }

    struct sockaddr_un serverAddr{};
    serverAddr.sun_family = AF_UNIX;
    /// 'sizeof(sockaddr_un.sun_path)-1' fixes warning when using 'sockaddr_un.sun_path - 1'
    strncpy(serverAddr.sun_path, SERVER_NAME, sizeof(serverAddr.sun_path) - 1);

    if (bind(listenFd, (const struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        fprintf(stdout, "ERROR: bind failure\n");
        fprintf(log, "ERROR: bind failure\n");
        exit(2);
    }

    if (listen(listenFd, 5) < 0) {
        fprintf(stdout, "ERROR: listening failure\n");
        fprintf(log, "ERROR: listening failure\n");
        exit(3);
    }

    fprintf(stdout, "server %s %s\n", SERVER_NAME, "started");
    fprintf(log, "server %s %s\n", SERVER_NAME, "started");
    fprintf(stdout, "Waiting for connections...\n");
    fprintf(log, "Waiting for connections...\n");

    char clientName[1024] = {};
    while (true) {
        int connectFd = accept(listenFd, nullptr, nullptr);
        if (connectFd < 0) {
            fprintf(stdout, "ERROR: client connection failure\n");
            fprintf(log, "ERROR: client connection failure\n");
        }

        // Authentication part
        fprintf(stdout, "Connecting...\n");
        fprintf(log, "Connecting...\n");

        send_to_client(connectFd, "Enter your name", log);
        read(connectFd, clientName, 1024);
        fprintf(stdout, "%s connected\n", clientName);
        fprintf(log, "%s connected\n", clientName);
        fprintf(stdout, "[%s] %s\n", clientName, clientName);
        fprintf(log, "[%s] %s\n", clientName, clientName);

        send_to_client(connectFd, "Hi, I'm ShadowDedulet Server", log);
        send_to_client(connectFd, "\t\tI'm simple echo server", log);
        send_to_client(connectFd, "\t\tPrint \"exit\" to quit", log);

        while (true) {
            char buffer[1024] = {};
            read(connectFd, buffer, 1024);

            fprintf(stdout, "[%s] %s\n", clientName, buffer);
            fprintf(log, "[%s] %s\n", clientName, buffer);

            if (std::string(buffer) == "exit") {
                fprintf(stdout, "%s disconnected\n", clientName);
                fprintf(log, "%s disconnected\n", clientName);
                close(connectFd);
                break;
            }
            send_to_client(connectFd, buffer, log);
        }
        break;
    }
    close(listenFd);
    unlink(SERVER_NAME);
    exit(0);
}
