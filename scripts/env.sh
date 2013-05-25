#!/bin/bash

mkdir work
sudo mount -t tmpfs temp work
export TEMP=work
export USRTWORK=work
export LD_LIBRARY_PATH=work

make clean
make worker
make example

work/initLog
work/pushTask HelloV -m

