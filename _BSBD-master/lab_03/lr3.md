# Цель работы
Необходимо реализовать POSIX-совместимый shell-скрипт для получения потоков запускаемого приложения

# Ход работы
## Теоретическая часть
POSIX (Portable Operating System Interface) - "Портативный Интерфейс Операционной Системы". Набор стандартов, описывающих интерфейсы между ОС и прикладной программой, библиотеку языка C и набор приложений и их интерфейсов. Необходим для обеспечения совместимости UNIX-ОС и переносимости прикладных программ.<br>
POSIX нужен для:
- облегчения переноса кодов на иные платформы
- унификации интерфейсов 
- определения минимума интерфейсов программ
- развития стандартов в направлениях коммуникационные сети, обработка данных, защита информации.


## Практическая часть
Создадим скрипт `nano hello.sh`, который будет приветствовать. Также выдадим права на него `chmod 755 hello.sh`.<br>
_hello.sh_:
```bash
#!/bin/sh
# greeting script


if [ $# -gt 0 ]; then
        echo "hello, $1";
        exit 0;
fi

echo "hello";
echo "name not provided" 1>&2;
exit 1;
```
Вызовем скрипт напрямую 2 раза (с/без аргумента):<br>
![hello.sh](http://bmstu.codes/iu8/bsbd/2021/63/evula/lab_03/-/raw/master/img/hello.png)<br>

Создадим скрипт `nano open.sh`, который будет запускать приложения.<br>
Поток вывода приложения переходит в stdout.log 
Поток ошибок приложения переходит в stderr.log 
Код завершение приложения записывается в return.log 
```bash                                      
#!/bin/bash
# open script


if [ $# -gt 0 ]; then
        app_path=$1;
        $app_path 1>stdout.log 2>stderr.log;
        echo return $?>return.log;
        cat stdout.log stderr.log return.log;
        exit 0;
fi
echo "path not provided">&2;
exit 1;
```
Использование скрипта:<br>
![open.sh](http://bmstu.codes/iu8/bsbd/2021/63/evula/lab_03/-/raw/master/img/open.png)<br>

#### Коды скриптов - [sources](https://bmstu.codes/iu8/bsbd/2021/63/evula/lab_03/-/tree/master/src)

# Выводы
Были получены знания о стандартах POSIX, получении потоков приложения, написании и использовании shell скриптов. 