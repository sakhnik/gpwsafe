#!/bin/bash

set -e

cd `dirname ${BASH_SOURCE[0]}`
[[ -d BUILD ]] || mkdir BUILD

mkdir -p /tmp/gpwsafe-build
if which bindfs >/dev/null 2>&1; then
   bindfs -n /tmp/gpwsafe-build BUILD
else
   sudo mount -o bind /tmp/gpwsafe-build BUILD
fi
(
cd BUILD
CC=clang CXX=clang++ cmake ..
)
