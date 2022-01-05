#!/bin/bash

docker run --rm -it \
--mount type=bind,source="$(pwd)",target=/host \
tonymikhailov/smh4_build_qt5.9.4:latest \
/make.sh


