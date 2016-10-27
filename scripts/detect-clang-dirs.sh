#!/bin/bash

echo \
	| clang -v -E -x c++ - 2>&1 \
	| awk '/#include <...>/,/End of search/' \
	| grep '/usr' \
	| while read -r l; do
		echo "'-isystem`readlink -f $l`',"
	done
