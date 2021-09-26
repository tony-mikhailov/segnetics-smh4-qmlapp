#!/bin/bash


docker run --rm --mount type=bind,source=/Users/tony/Workspace/smh4,target=/host  -it test_smh4 /make.sh
