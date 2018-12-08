#!/bin/bash

export SETUP_DIRECTORY=$PWD
export LIBRARY_INSTALL_DIRECTORY=$PWD/../Libraries
export BUILD_TYPE="Release"
export NUMBER_OF_CORES=`nproc`

cd ..

if [ ! -d Libraries ]
then
    mkdir Libraries
fi

cd $SETUP_DIRECTORY

. cgns-3.3.1.sh

cd $SETUP_DIRECTORY

. boost-1.68.0.sh
