#!/bin/bash 

PWDFROM=.

. ./prepare.sh
echo "--->" $PWDFROM
cd $PWDFROM

echo "==============================================================="
echo $0 PWD: $(pwd)
echo "==============================================================="

dpkg-buildpackage -B -uc -us 
