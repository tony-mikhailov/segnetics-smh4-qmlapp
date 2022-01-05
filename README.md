# Пример проекта с системой сборки в docker-контейнере для контроллеров Segnetics SMH4 (Trim5, Matrix) 


## Как пользоваться

Для *NIX

1. Клонируем репозиторий:
> $ git clone https://github.com/tony-mikhailov/segnetics-smh4-qmlapp.git

> $ cd segnetics-smh4-qmlapp/

собираем docker-образ:
> $ docker build -t tonymikhailov/smh4_build_qt5.9.4 .

запускаем сборку приложения в докере:
> $ docker run --rm -it --mount type=bind,source="$(pwd)",target=/host tonymikhailov/smh4_build_qt5.9.4:latest /make.sh

Для удобства установки результат сборки будет сразу упакован в deb-пакет   ** qmlapp_smh4_1.0.0.deb ** 


> $ scp qmlapp_smh4_1.0.0.deb root@192.168.0.213:~/

