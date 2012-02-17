#!/bin/sh

echo "Generating test links."

for linkname in `grep 'Test-G[0-9]*-T[0-9]*.\.sh' Makefile.am | xargs` ; do
    if [ -L $linkname ]; then
        rm -f $linkname
    fi
    ln -s TestBase.sh $linkname
done

