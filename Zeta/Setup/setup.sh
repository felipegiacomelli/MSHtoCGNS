# !/bin/bash

export SETUP_DIRECTORY=$PWD
export LIBRARY_INSTALL_DIRECTORY=$PWD/../Libraries
export BUILD_TYPE="Debug"

cd ..

if [ ! -d Libraries ]; then
    mkdir Libraries
fi

cd $SETUP_DIRECTORY

. cgns-3.3.1.sh

cd $SETUP_DIRECTORY

. boost-1.67.0.sh
