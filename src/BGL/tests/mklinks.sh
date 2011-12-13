#!/bin/sh

echo "Generating test links."

for linkname in `grep 'Test-G[0-9]*-T[0-9]*.\.sh' Makefile.am | xargs` ; do
    ln -s TestBase.sh $linkname
done

