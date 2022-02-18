#include <cstdio>

int main() {
    FILE * file;
    file = fopen("../lab_02.txt", "w");
    if (file != nullptr) {
        int n;
        fprintf(stdout, "Enter num:\n>>");
        fscanf(stdin, "%d", &n);
        fprintf(file, "entered %d", n);
        fclose(file);
    }
    fprintf(stderr, "(stderr)");
    return 0;
}
