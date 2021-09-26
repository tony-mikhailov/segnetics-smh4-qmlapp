#!/bin/bash

cd /home/build/qmlapp && 
/home/build/qt/5.9.4/armhf/bin/qmake qmlapp.pro && 
make && 
cp qmlapp /home/build/DEB/qmlapp/ &&
cd /home/build/ &&
dpkg-deb --build DEB qmlapp_smh4_1.0.0.deb &&
cp qmlapp_smh4_1.0.0.deb /host/ 
