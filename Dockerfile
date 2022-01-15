from ubuntu:18.04 as base_build

COPY ./apt.packages.txt /home/build/apt.packages.txt

RUN dpkg --add-architecture i386 \
    && apt-get -qq -y update \
    && xargs apt-get -qq install -y --no-install-recommends < /home/build/apt.packages.txt 
    
RUN cd / && wget https://mikhailov.center/archives/compiler.tar.gz && tar xf compiler.tar.gz
RUN mkdir -p /home/build && cd /home/build && wget https://mikhailov.center/archives/qt_5.9.4_arm_smh4.tar.gz && tar xf qt_5.9.4_arm_smh4.tar.gz 
RUN mkdir -p /home/build && cd /home/build && wget https://mikhailov.center/archives/deb.tar.gz && tar xf deb.tar.gz 

#ADD ./archives/compiler.tar.gz /
#ADD ./archives/qt_5.9.4_arm_smh4.tar.gz /home/build
#ADD ./archives/deb.tar.gz /home/build

FROM base_build

#COPY ./qmlapp /home/build/qmlapp

COPY ./make.sh /make.sh
RUN chmod +x /make.sh


