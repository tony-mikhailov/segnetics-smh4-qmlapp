#!/bin/bash

docker run --rm --mount type=bind,source="$(pwd)",target=/host  -it test_smh4 /make.sh
