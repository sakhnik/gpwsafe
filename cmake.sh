#!/bin/bash

set -e

cd `dirname ${BASH_SOURCE[0]}`
[[ -d BUILD ]] || mkdir BUILD

mkdir /tmp/gpwsafe-build
sudo mount -o bind /tmp/gpwsafe-build BUILD
cd BUILD
cmake ..
ln -sf .ycm_extra_conf.py ..
