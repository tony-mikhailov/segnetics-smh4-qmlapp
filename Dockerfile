from ubuntu:18.04 as base_build

COPY ./apt.packages.txt /home/build/apt.packages.txt

RUN dpkg --add-architecture i386 \
    && apt-get -qq -y update \
    && xargs apt-get -qq install -y --no-install-recommends < /home/build/apt.packages.txt 
    

ADD ./archives/compiler.tar.gz /
ADD ./archives/qt_5.9.4_arm_smh4.tar.gz /home/build
ADD ./archives/deb.tar.gz /home/build

FROM base_build

COPY ./qmlapp /home/build/qmlapp

COPY ./make.sh /make.sh
RUN chmod +x /make.sh


