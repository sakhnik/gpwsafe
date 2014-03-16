#!/bin/bash

srcdir=`dirname ${BASH_SOURCE[0]}`

pushd $srcdir

autopoint || exit 1

aclocal || exit 1

autoheader || exit 1

autoconf || exit 1

automake --add-missing || exit 1

ln -s /usr/share/gettext/gettext.h
ln -s /usr/share/gettext/config.rpath
find ./src -name "*.cc" -o -name "*.hh" > po/POTFILES.in

popd

$srcdir/configure $@
