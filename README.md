# Пример проекта с системой сборки в docker-контейнере для контроллеров Segnetics SMH4 (Trim5, Matrix) 


## Как пользоваться

### Windows

1. Установите docker -> https://www.docker.com
2. Установите git -> https://git-scm.com/download/win
3. Клонируйте репозиторий
> git clone https://github.com/tony-mikhailov/segnetics-smh4-qmlapp.git

3. Запустите в склонированной папке segnetics-smh4-qmlapp пакетные файлы:

**docker_build.bat** 

**docker_run.bat** 

Для удобства установки результат сборки будет сразу упакован в deb-пакет   **qmlapp_smh4_1.0.0.deb** 
Установите его с помошью smlogix: установка deb-пакета или скопируйте каким-либо на устройство, например по ssh
scp qmlapp_smh4_1.0.0.deb root@192.168.0.213:~/

и установите с помощью команды:
dpkg -i qmlapp_smh4_1.0.0.deb 

### Для *NIX

1. Установите docker, git
2. Клонируем репозиторий:
> $ git clone https://github.com/tony-mikhailov/segnetics-smh4-qmlapp.git

> $ cd segnetics-smh4-qmlapp/

собираем docker-образ:
> $ docker build -t tonymikhailov/smh4_build_qt5.9.4 .

запускаем сборку приложения в докере:
> $ docker run --rm -it --mount type=bind,source="$(pwd)",target=/host tonymikhailov/smh4_build_qt5.9.4:latest /make.sh

Для удобства установки результат сборки будет сразу упакован в deb-пакет   **qmlapp_smh4_1.0.0.deb** 

Скопируйте  **qmlapp_smh4_1.0.0.deb**  по ssh:
> $ scp qmlapp_smh4_1.0.0.deb root@192.168.0.213:~/

и установите его с помощью команды:
> $ dpkg -i qmlapp_smh4_1.0.0.deb 

