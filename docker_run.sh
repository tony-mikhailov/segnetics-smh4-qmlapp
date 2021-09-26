#!/bin/bash

docker run --rm -it \
--mount type=bind,source="$(pwd)",target=/host \
test_smh4:latest \
/make.sh


