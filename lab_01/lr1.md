# Цель работы
Ознакомление и установка дистрибутива Linux на виртуальную машину.

# Ход работы
## Настройка VirtualBox
Был выбран дистрибутив Arch Linux. Ниже показана первичная настройка виртуальной машины в VirtualBox
![oops](https://bmstu.codes/iu8/bsbd/2021/63/evula/lab_01/-/raw/master/img/VBsets.png)

## Работа с дисками
### Разбиение диска
```bash
/dev/sda1 - 1GB		// EFI 		// EFI system 		// FAT32
/dev/sda2 - 1GB		// swap 	// Linux swap 		// SWAP
/dev/sda3 - 6GB		// home 	// Linux filesystem	// EXT4
/dev/sda4 - 10GB	// var 		// Linux filesystem	// EXT4
/dev/sda5 - 6GB		// opt  	// Linux filesystem	// EXT4
/dev/sda6 - 20GB	// root 	// Linux filesystem	// EXT4
```
### Форматирование диска
```bash
cfdisk		// переход в меню разбиения диска 
mkfs.fat -F32 /dev/sda1
mkswap /dev/sda2
mkfs.ext4 /dev/sda3
mkfs.ext4 /dev/sda4
mkfs.ext4 /dev/sda5
mkfs.ext4 /dev/sda6
```
### Монтирование разделов
```bash
mount /dev/sda6 /mnt
mkdir -p -v /mnt/boot/efi
mount /dev/sda1 /mnt/boot/efi
mkdir -p -v /mnt/{home,var,opt}
mount /dev/sda3 /mnt/home
mount /dev/sda4 /mnt/var
mount /dev/sda5 /mnt/opt
swapon /dev/sda2
```
### Результат
![oops](https://bmstu.codes/iu8/bsbd/2021/63/evula/lab_01/-/raw/master/img/lsblk.png)

## Установка ОС
```bash
pacstrap /mnt base linux linux-firmware		// установка ядра, базовой ОС и прошивок 
genfstab -U /mnt >> /mnt/etc/fstab		// генерация файла параметров разделов диска
arch-chroot /mnt				// переход в рут новой системы 
```

## Первичная настройка системы
### Установка текстового редактора
```bash
pacman -S nano
```
### Настройка локали
```bash
ln -sf /usr/share/zoneinfo/Europe/Moscow /etc/localtime
hwclock --systohc
```
Раскоментированы строки в `/etc/locale.gen`
<pre>en_US.UTF-8 UTF-8
ru_RU.UTF-8 UTF-8</pre>
`locale-gen` - генерация локалей

Добавлена строка в `/etc/hostname`
<pre>shadowdedulet</pre>
Добавлены строки в `/etc/hosts`
<pre>127.0.0.1	localhost
::1		localhost
127.0.1.1	shadowdedulet.localdomain 	shadowdedulet </pre>
```bash
mkinitcpio -P 	// создание начального загрузочного диска
passwd		// задаем пароль для рута
```

## Установка загрузчика
```bash
mkdir -p -v /boot/efi/EFI/arch
cp /boot/vmlinuz-linux /boot/efi/EFI/arch/
cp /boot/initramfs-linux.img /boot/efi/EFI/arch/
cp /boot/initramfs-linux-fallback.img /boot/efi/EFI/arch/

pacman -S grub efibootmgr
grub-install --target=x86_64-efi --efi-directory=/boot/efi --bootloader-id=GRUB --recheck
grub-mkconfig -o /boot/grub/grub.cfg
```
## Установка необходимых перед перезагрузкой пакетов
```bash
pacman -S networkmanager net-tools
```

## Перезапуск системы
```bash
exit
umount -R /mnt 		// отмонтирование всех разделов
reboot
```

## Вторичная настройка системы
### Сеть
```bash
systemctl enable NetworkManager
systemctl start NetworkManager
ping google.com 		// проверка работы сети
```
### Настройка пользователя
```bash
groupadd ic8-63
useradd -g ic8-63 -m evula
passwd evula
usermod -aG wheel,optaical,video,audio,storage evula
echo "evula ALL=(ALL) ALL" >> /etc/sudoers
```
### Extra
```bash
pacman -S chromium git sudo
// пакеты для улучшения работы VirtualBox
pacman -S virtualbox-guest-dkms virtualbox-guest-utils
sudo systemctl start vboxservice
sudo systemctl enable vboxservice
```
### Настройка ssh
Изменена строка в `/etc/ssh/ssh_config`
<pre>PasswordAuthentication no</pre>
Изменены строки в `/etc/ssh/sshd_config`
<pre>PasswordAuthentication no
ChallengeResponseAuthentication no
AllowUsers evula</pre>
```bash
sudo systemctl enable ssh
sudo systemctl start ssh
```
Для проверки работы ssh былш сгенерирован и скопирован на сервер паблик ключ:
```bash
ssh-keygen
ssh-copy-id username@serveraddress
``` 
![oops](http://bmstu.codes/iu8/bsbd/2021/63/evula/lab_01/-/raw/master/img/ssh.PNG)

### Установка XORG
```bash
pacman -S xf86-video-vesa xorg xorg-xinit xorg-server
```
### Установка Plasma KDE
```bash
pacman -S plasma kde-applications sddm
// при первой установке были ошибки '404'
pacman -S plasma kde-applications sddm
systemctl enable sddm
reboot
```
### Установка CLion
```bash
// был скачан .tar.gz архив с оф. сайта CLion
sudo tar xvzf CLion-2021.1.tar.gz -C /opt 	// распаковка
cd /opt/clion-2021.1/bin
./clion.sh 					// запуск CLion'a
```
### Extra
Проверка - правильно ли смонтирована директория /tmp
```bash
df -T /tmp 		// проверка монтирования папки /tmp
// filesystem 	Type 	1K-blocks 	Used 	Available 	Use% 	Mounted on
// tmpfs 	tmpfs 	3034604 	4 	3034600 	1% 	/tmp
```

# Результат
![oops](https://bmstu.codes/iu8/bsbd/2021/63/evula/lab_01/-/raw/master/img/WorkingThingy.png)

# Выводы
В результате выполнения лабораторной работы был установлен дистрибутив Arch Linux с рабочим окружением KDE на виртуальную машину. Был установлен и настроен ssh, а также установлены необходимые пакеты (NetworkManager, git, ..) и среда разработки кода (CLion).

# Используемые ресурсы
- [Arch Linux](https://wiki.archlinux.org/index.php/installation_guide)
- [OpenSSH](https://wiki.archlinux.org/index.php/OpenSSH)
- [Markdown](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)
- [CLion](https://www.jetbrains.com/clion/)
