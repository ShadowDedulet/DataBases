# Цель работы
Разработать приложения для работы с UNIX-сокетами

# Ход работы
## Теоретическая часть
UNIX-сокет - механизм обмена данными, не использующая сетевого протокола для взаимодействия (обмен данными происходит локально). Используется в операционных системах, поддерживающих стандарт POSIX, для межпроцессного взаимодействия.<br>
В дополнение к отсылаемым данным процессы могут отсылать файловые дескрипторы через соединение на основе UDS


## Практическая часть
Релизованы сервер и клиент:
### Сервер
- создает локальный сокет
- соединяет его с именем сокета
- ожидает подключения клиентов
- устанавливает подключение с ним
- запрашиват имя (идентификация)
- получает и отправляет данные клиенту  
- ведет отчет передаваемых данных
- на вопрос клиента отвечает случайной фразой ("8-ball")

#### Создание и настройка сокета сервера
```cpp
char* SERVER_NAME = "ShadowDedulet";

unlink(SERVER_NAME);

int listenFd = socket(AF_UNIX, SOCK_STREAM, 0);
if (listenFd < 0) {
    fprintf(stdout, "ERROR: socket creation failure\n");
    fprintf(log, "ERROR: socket creation failure\n");
    exit(1);
}

struct sockaddr_un serverAddr{};
serverAddr.sun_family = AF_UNIX;
strncpy(serverAddr.sun_path, SERVER_NAME, sizeof(serverAddr.sun_path) - 1);

if (bind(listenFd, (const struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
    fprintf(stdout, "ERROR: bind failure\n");
    fprintf(log, "ERROR: bind failure\n");
    exit(2);
}
```

#### Функция отправки сообщения клиенту с зщаписью в лог 
```cpp
void send_to_client(int &client, char* msg, FILE* log) {
    char buffer[1024] = {};

    fprintf(stdout, "[ShadowDedulet] %s\n", msg);
    fprintf(log, "[ShadowDedulet] %s\n", msg);

    sprintf(buffer, "%s\n", msg);
    write(client, buffer, strlen(buffer));
}
```

### Клиент
- создает локальный сокет
- соединяет его с именем сокета
- устанавливает подключение с сервером
- может отправлять данные серверу
- при вводе "exit" отключается от сервера

#### Создание сокета клиента, и подключение к серверу
```cpp
char* SERVER_NAME = "ShadowDedulet";

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
```

Все события (подключение/отключение клиента, обмен данными между клиентом и сервером) записываются в файл 'server.log'


### Пример взаимодействия клиента и сервера
![](http://bmstu.codes/iu8/bsbd/2021/63/evula/lab_05/-/raw/master/img/example.jpg)
### Исходные коды приложения - [sources](https://bmstu.codes/iu8/bsbd/2021/63/evula/lab_05/-/tree/master/src)

# Выводы
Были получены знания о Unix-сокетах и принципах взаимодействия сервер-клиент.