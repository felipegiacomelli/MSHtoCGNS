#!/bin/bash

export SETUP_DIRECTORY=$PWD
export LIBRARY_INSTALL_DIRECTORY=$PWD/../Libraries
export BUILD_TYPE="release"
export NUMBER_OF_CORES=`nproc`

export BOOST_VERSION="1.72.0"
export CGNS_VERSION="4.1.1"

cd ..

if [ ! -d Libraries ]
then
    mkdir Libraries
fi

cd $SETUP_DIRECTORY

. cgns.sh

cd $SETUP_DIRECTORY

. boost.sh
