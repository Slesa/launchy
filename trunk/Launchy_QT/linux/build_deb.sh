#!/bin/bash

dir=`pwd`

if [ $# -ne 1 ]
then
    echo "1 arg needed to build deb, ./build.sh <version>"
    exit
fi

ver=$1

rm -rf launchy*
svn export ../ launchy-$ver
tar cfz launchy-$ver.tar.gz launchy-$ver/
cd launchy-$ver
dh_make -e karlinjf@sourceforge.net -f ../launchy-$ver.tar.gz
cp ../debian/* debian/
dpkg-buildpackage -rfakeroot



