#include <cstdio>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int p[2];
    if (pipe(p) < 0) {
        return 1;
    }
    char buf[6];
    char* c_first = "1desc\n";
    char* c_second = "2desc\n";
    write(p[1], c_first, 6);
    write(p[1], c_second, 6);

    int first;
    first = open("../lab_02.txt", O_RDWR);
    int second = dup(first);
    if (first == -1 || second == -1) {
        return 1;
    }
    read(p[0], buf, 6);
    write(first, buf, 6);
    read(p[0], buf, 6);
    write(second, buf, 6);
    fork();
    write(first, "forked\n", 7);
    return 0;
}
