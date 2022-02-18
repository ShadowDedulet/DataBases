# Цель работы
Разработать приложения для работы с IP-сокетами

# Ход работы
## Теоретическая часть
IP-сокет - механизм обмена данными между процессами, исполняеиыми как на одной машине, так и на разных(передача происходит по сети).<br>
Для взаимодействия с помощью стека протоколов TCP/IP используются адреса и порты. Такая пара определяет сокет.<br>
Адрес - 32-битная структура (IPv4) или 128-битная (IPv6).<br>
Номер порта - целое число в диапазоне от 0 до 65535 (для TCP).

## Практическая часть
Релизованы сервер и клиент:
### Сервер
- создает локальный сокет
- соединяет его с именем сокета
- ожидает подключения клиентов
- устанавливает подключение с ним
- запрашиват имя (идентификация)
- при получении данных от клиента пересылает их обратно (связь между сервером и клиентом)

#### Функция отправки сообщения клиенту (с записью в лог)
```cpp
void send_to_client(int &client, char* msg, FILE* log) {
    char buffer[1024] = {};

    fprintf(stdout, "[192.168.1.64] %s\n", msg);
    fprintf(log, "[192.168.1.64] %s\n", msg);

    sprintf(buffer, "%s\n", msg);
    write(client, buffer, strlen(buffer));
}
```
#### Создание сокета и его настройка 
```cpp
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
```
#### Связывание сокета с лок. адресом и установка в режим прослушивания
```cpp
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
```

### Клиент
- создает локальный сокет
- соединяет его с именем сокета
- устанавливает подключение с сервером
- может отправлять данные серверу
- при вводе "exit" отключается от сервера

#### Создание сокета
```cpp
int clientFd = socket(AF_INET, SOCK_STREAM , 0);
if (clientFd < 0) {
    fprintf(stdout, "ERROR: socket creation failure\n");
    exit(1);
}
```
#### Настройка сокета и подключение к серверу
```cpp
int opt = 1;
if (setsockopt(clientFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    fprintf(stdout, "ERROR: socket opt failure\n");
    exit(2);
}

if(inet_pton(AF_INET, IP, &serverAddr.sin_addr) <= 0) {
    fprintf(stdout, "ERROR: server not found\n");
    exit(3);
}

if (connect (clientFd, (const struct sockaddr *) &serverAddr, sizeof(struct sockaddr_in)) < 0) {
    fprintf(stdout, "ERROR: connection failure\n");
    exit(4);
}
```

Все события (подключение/отелючение клиента, обмен данными между клиентом и сервером) записываются в файл 'server.log'

### Пример взаимодействия клиента и сервера
![](http://bmstu.codes/iu8/bsbd/2021/63/evula/lab_06/-/raw/master/img/example.jpg)
### Исходные коды приложения - [sources](https://bmstu.codes/iu8/bsbd/2021/63/evula/lab_06/-/tree/master/src)

# Выводы
Были получены знания о IP-сокетах.