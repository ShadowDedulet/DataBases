#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    char* SERVER_NAME = "ShadowDedulet";

    fprintf(stdout, "Connecting to [%s]...\n", SERVER_NAME);

    int clientFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (clientFd < 0){
        fprintf(stdout, "ERROR: socket creation failure\n");
        exit(1);
    }

    struct sockaddr_un serverAddr{};
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, SERVER_NAME, sizeof(serverAddr.sun_path) - 1);

    if (connect(clientFd, (const struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        fprintf(stdout, "ERROR: connection failure\n");
        exit(2);
    }

    fprintf(stdout, "Connected to [%s]\n", SERVER_NAME);

    ssize_t len;
    while (true) {
        char read_buf[1024] = {}, write_buf[1024] = {};
        len = read(clientFd, read_buf, 1024);
        if (len <= 0){
            fprintf(stdout, "Disconnecting...\n");
            break;
        }

        fprintf(stdout, "[%s] %s>>", SERVER_NAME, read_buf);

        fscanf(stdin, "%s", write_buf);
        write(clientFd, write_buf, strlen(write_buf));
    }

    fprintf(stdout, "Disconnected from server\n");
    close(clientFd);
    exit(0);
}
