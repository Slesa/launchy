#!/bin/bash

dir=`pwd`


if [ $# -eq 0 ]
then
    op="release"
else
    op=$1
fi

if [ $op = "deb" ]
then
    if [ $# -ne 2 ]
    then
	echo "2 args needed to build deb, ./build.sh deb <version>"
	exit
    fi
    
    ver=$2

    svn export . launchy-$ver
    tar cfz launchy-$ver.tar.gz launchy-$ver/
    cd launchy-$ver
    dh_make -e karlinjf@sourceforge.net -f ../launchy-$ver.tar.gz

    exit
fi




# Build Launchy

qmake-qt4 Launchy.pro && make -f Makefile $op

# Build the platforms
cd platforms/gnome
qmake-qt4 gnome.pro && make -f Makefile $op
cd ../kde
qmake-qt4 kde.pro && make -f Makefile $op


# Build the plugins
cd ../../plugins/weby
qmake-qt4 weby.pro && make -f Makefile $op
cd ../calcy
qmake-qt4 calcy.pro && make -f Makefile $op
cd ../gcalc
qmake-qt4 gcalc.pro && make -f Makefile $op
cd ../runner
qmake-qt4 runner.pro && make -f Makefile $op


