# Цель работы
Реализовать приложение, запускающее произвольное приложение с получением его потоков на C++

# Ход работы
## Практическая часть

Напишем скрипт:
- выводит сумму длин аргументов
- при отсутствии аргументов выводит ошибку "arguments not given"<br>
_Будем считать, что отсутствие аргументов (т.е. сумма длин аргументов равна нулю) - ошибка._

```bash
#!/bin/sh
# greeting script

if [ $# -gt 0 ]; then
  SUM=0;
  for word in "$@"
  do
    SUM=$((SUM + ${#word}));
  done

  echo "sum of lengths = $SUM";
  exit 0;
fi

echo "arguments not given" 1>&2;
exit 1;
```


Дальше напишем приложение, запускающий данный скрипт.<br>
Оно получает потоки запускаемого приложения - `stdout` и `stderr`, выводит в консоль и записывает в файл (`out.log` и `err.log` соответсвенно)<br>
Также получает код завершения. выводит его в консоль и записывает в файл (`exc.log`).<br>
При возникновении ошибки в запускающем приложении, она выводится в файл `sys.log`

```cpp
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
```

### Пример вызова sh скрипта 
![](http://bmstu.codes/iu8/bsbd/2021/63/evula/lab_04/-/raw/master/img/example.jpg)
### Исходные коды приложения - [sources](https://bmstu.codes/iu8/bsbd/2021/63/evula/lab_04/-/tree/master/src)

# Выводы
Были получены знания о потоках и реализовано приложение для работы с потоками запускаемого приложения.