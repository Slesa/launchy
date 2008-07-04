#!/bin/bash

dir=`pwd`


if [ $# -ne 1 ]
then
    op="release"
else
    op=$1
fi

if [ $op = "deb" ]
then

    exit
fi




# Build Launchy
cd ..
qmake-qt4 Launchy.pro && make $op

# Build the platforms
cd platforms/gnome
qmake-qt4 gnome.pro && make $op
cd ../kde
qmake-qt4 kde.pro && make $op


# Build the plugins
cd ../../plugins/weby
qmake-qt4 weby.pro && make $op
cd ../calcy
qmake-qt4 calcy.pro && make $op
cd ../gcalc
qmake-qt4 gcalc.pro && make $op
cd ../runner
qmake-qt4 runner.pro && make $op


