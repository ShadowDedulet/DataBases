#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>


void send_to_client(int &client, char* msg, FILE* log) {
    char buffer[1024] = {};

    fprintf(stdout, "[192.168.1.64] %s\n", msg);
    fprintf(log, "[192.168.1.64] %s\n", msg);

    sprintf(buffer, "%s\n", msg);
    write(client, buffer, strlen(buffer));
}

int main(int argc, char *argv[]) {
    char* IP = "192.168.1.64";
    int PORT = 6969;

    FILE* log;
    log = fopen("./server.log", "w");

    fprintf(stdout, "[%s] %s %d...\n", IP, "starting on", PORT);
    fprintf(log, "[%s] %s %d...\n", IP, "starting on", PORT);

    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd < 0) {
        fprintf(stdout, "ERROR: socket creation failure\n");
        fprintf(log, "ERROR: socket creation failure\n");
        exit(1);
    }

    int opt = 1;
    if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        fprintf(stdout, "ERROR: socket opt failure\n");
        fprintf(log, "ERROR: socket opt failure\n");
        exit(2);
    }

    struct sockaddr_in serverAddr{};
    int serverAddrLen = sizeof(serverAddr);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT );

    if (bind(listenFd, (const struct sockaddr *) &serverAddr, sizeof(struct sockaddr_in)) < 0) {
        fprintf(stdout, "ERROR: bind failure\n");
        fprintf(log, "ERROR: bind failure\n");
        exit(3);
    }

    if (listen(listenFd, 20) < 0) {
        fprintf(stdout, "ERROR: listening failure\n");
        fprintf(log, "ERROR: listening failure\n");
        exit(4);
    }

    fprintf(stdout, "[%s] %s %d\n", IP, "started on", PORT);
    fprintf(log, "[%s] %s %d\n", IP, "started on", PORT);
    fprintf(stdout, "Waiting for connections...\n");
    fprintf(log, "Waiting for connections...\n");

    char clientName[1024] = {};
    while(true) {
        int connectFd = accept(listenFd, (struct sockaddr *)&serverAddr, (socklen_t*)&serverAddrLen);
        if (connectFd < 0) {
            fprintf(stdout, "ERROR: client connection failure\n");
            fprintf(log, "ERROR: client connection failure\n");
        }

        fprintf(stdout, "Connecting...\n");
        fprintf(log, "Connecting...\n");

        send_to_client(connectFd, "Enter your name", log);
        read(connectFd, clientName, 1024);
        fprintf(stdout, "%s connected\n", clientName);
        fprintf(log, "%s connected\n", clientName);
        fprintf(stdout, "[%s] %s\n", clientName, clientName);
        fprintf(log, "[%s] %s\n", clientName, clientName);

        send_to_client(connectFd, "Hi, I'm ShadowDedulet Server", log);
        send_to_client(connectFd, "\t\tI can count length of your message", log);
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
    exit(0);
}
