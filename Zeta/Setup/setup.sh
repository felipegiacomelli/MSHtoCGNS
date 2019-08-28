#!/bin/bash

export SETUP_DIRECTORY=$PWD
export LIBRARY_INSTALL_DIRECTORY=$PWD/../Libraries
export BUILD_TYPE="release"
export NUMBER_OF_CORES=`nproc`

cd ..

if [ ! -d Libraries ]
then
    mkdir Libraries
fi

cd $SETUP_DIRECTORY

. cgns-3.4.0.sh

cd $SETUP_DIRECTORY

. boost-1.70.0.sh
