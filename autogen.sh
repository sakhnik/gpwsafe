#!/bin/bash

srcdir=`dirname ${BASH_SOURCE[0]}`

pushd $srcdir

aclocal || exit 1

autoheader || exit 1

autoconf || exit 1

automake --add-missing || exit 1

popd

$srcdir/configure $@
