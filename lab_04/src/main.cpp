#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {
    FILE *log;
    log = fopen("sys.log", "w");

    // no path to script
    if (argc < 2) {
        fprintf(log, "ERROR: app name not given\n");
        printf("ERROR: app name not given\n");
        exit(1);
    }

    // concatenate all arguments to 'path-string'
    // ./ + app_name + [args]
    std::string temp = "./";
    for (unsigned int i = 0; i < argc-1; ++i)
        temp += std::string(argv[i+1]) + " ";
    // remove extra space from end
    temp.erase(temp.length()-1);
    const char *path = temp.c_str();

    // separate pipes for stdout and stderr
    int out_pipe[2], err_pipe[2];
    if (pipe(out_pipe) < 0 || pipe(err_pipe) < 0) {
        fprintf(log, "ERROR: FD\n");
        printf("ERROR: FD\n");
        exit(1);
    }
    
    // fork the process
    int STATUS;
    pid_t pid = fork();
    // child
    if (pid == 0) {
        close(out_pipe[0]);
        close(err_pipe[0]);
        dup2(out_pipe[1], 1);
        dup2(err_pipe[1], 2);
        close(out_pipe[1]);
        close(err_pipe[1]);

        // run the script
        STATUS = system(path);
        if (WEXITSTATUS(STATUS) == 0)
            return 0;
        else
            return 1;
    }
    // parent
    else {
        waitpid(pid, &STATUS, 0);
    }
    STATUS = WEXITSTATUS(STATUS);
    close(out_pipe[1]);
    close(err_pipe[1]);

    // read collected data
    char buff_out[1024], buff_err[1024];
    buff_out[read(out_pipe[0], buff_out, sizeof(buff_out))] = '\0';
    buff_err[read(err_pipe[0], buff_err, sizeof(buff_err))] = '\0';

    // print app exit code to console
    printf("exit code: %d\n", STATUS);

    FILE *out;
    FILE *err;
    FILE *exc;
    out = fopen("out.log", "w");
    err = fopen("err.log", "w");
    exc = fopen("exc.log", "w");

    // if error occurs
    if (STATUS) {
        // print 'err' data to console
        printf("%s", buff_err);
        // clear 'out' file
        fprintf(out, "");
        // print data to 'err' file
        fprintf(err, "%s", buff_err);
    }
    // if no error
    else {
        // print 'out' data to console
        printf("%s", buff_out);
        // print data to 'out' file
        fprintf(out, "%s", buff_out);
        // clear 'err' file
        fprintf(err, "");
    }
    // print app exit code to 'exc' file
    fprintf(exc, "%d\n", STATUS);

    // close files
    fclose(out);
    fclose(err);
    fclose(exc);

    exit(0);
}