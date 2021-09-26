from ubuntu:18.04

COPY ./apt.packages.txt /home/build/apt.packages.txt

RUN echo "## Start building" \
    && echo "## Update and install packages" \
    && dpkg --add-architecture i386 \
    && apt-get -qq -y update \
    && xargs apt-get -qq install -y --no-install-recommends < /home/build/apt.packages.txt \
    && echo "## Done"

ADD ./archives/compiler.tar.gz /
ADD ./archives/qt_5.9.4_arm_smh4.tar.gz /home/build
ADD ./archives/deb.tar.gz /home/build

COPY ./qmlapp /home/build/qmlapp

COPY ./make.sh /make.sh
RUN chmod +x /make.sh


