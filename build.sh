#!/bin/bash

set -e

cd src
mkdir -p build && cd build
cmake ..
make
ctest -V --output-on-failure

if [[ "$unamestr" == 'Darwin' ]]; then
	platform='mac'
elif [[ "$unamestr" == 'Linux' ]]; then
	platform='lin'
elif [[ "$unamestr" == 'FreeBSD' ]]; then
	platform='bsd'
fi

#if [[ $platform == 'lin' ]]; then
#   sh -x setup/linux/create-.sh
if [[ $platform == 'mac' ]]; then
	cd ../../setup
	cpack -G Bundle -C cpack.mac
#elif [[ $platform == 'bsd' ]]; then
#   sh -x setup/bsd/create-setup.sh
fi
cpack


#$platform='unknown'
#unamestr=`uname`
#

#echo Creating makefiles ...

#pushd .
#cd src
#$QTDIR/bin/qmake -recursive Launchy.pro "CONFIG+=release"

# $QTDIR/bin/lupdate src/Launchy.pro

# $QTDIR/bin/lrelease src/common/common.pro
# $QTDIR/bin/lrelease src/launchy/launchy.pro
# $QTDIR/bin/lrelease src/plugins/controly/controly.pro
# $QTDIR/bin/lrelease src/plugins/runner/runner.pro
# $QTDIR/bin/lrelease src/plugins/verby/verby.pro
# $QTDIR/bin/lrelease src/plugins/weby/weby.pro

#echo Building ...
#make

#popd

#echo Creating setup ...
#if [[ $platform == 'lin' ]]; then
#   sh -x setup/linux/create-.sh
#elif [[ $platform == 'mac' ]]; then
#   sh -x setup/mac/create-setup.sh
#elif [[ $platform == 'bsd' ]]; then
#   sh -x setup/bsd/create-setup.sh
#fi

echo Done
