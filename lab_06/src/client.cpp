#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>


int main(int argc, char *argv[]) {
    char* IP = "192.168.1.64";
    int PORT = 6969;

    fprintf(stdout, "Connecting to [%s] %d...\n", IP, PORT);

    int clientFd = socket(AF_INET, SOCK_STREAM , 0);
    if (clientFd < 0) {
        fprintf(stdout, "ERROR: socket creation failure\n");
        exit(1);
    }

    int opt = 1;
    if (setsockopt(clientFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        fprintf(stdout, "ERROR: socket opt failure\n");
        exit(2);
    }
    struct sockaddr_in serverAddr{};
    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, IP, &serverAddr.sin_addr) <= 0) {
        fprintf(stdout, "ERROR: server not found\n");
        exit(3);
    }

    if (connect (clientFd, (const struct sockaddr *) &serverAddr, sizeof(struct sockaddr_in)) < 0) {
        fprintf(stdout, "ERROR: connection failure\n");
        exit(4);
    }

    fprintf(stdout, "Connected to [%s] %d\n", IP, PORT);

    ssize_t len;
    while (true) {
        char read_buf[1024] = {}, write_buf[1024] = {};
        len = read(clientFd, read_buf, 1024);
        if (len <= 0) {
            fprintf(stdout, "Disconnecting...\n");
            break;
        }

        fprintf(stdout, "[%s] %s>>", IP, read_buf);

        fscanf(stdin, "%s", write_buf);
        write(clientFd, write_buf, strlen(write_buf));
    }

    fprintf(stdout, "Disconnected from server\n");
    close(clientFd);
    exit(0);
}
