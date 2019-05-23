#!/bin/bash

LIBRARY="boost-1.70.0"
DOWNLOAD_LINK="https://downloads.sourceforge.net/project/boost/boost/1.70.0/boost_1_70_0.tar.gz"
COMPRESSED_LIBRARY=$LIBRARY.tar.gz
BUILD_TYPE=`echo ${BUILD_TYPE,,}`

if [ ! -d "$LIBRARY" ]; then
    if [ ! -f "$COMPRESSED_LIBRARY" ]; then
        wget $DOWNLOAD_LINK -O $COMPRESSED_LIBRARY
        tar -x -z -f $COMPRESSED_LIBRARY
        mv boost_1_70_0 $LIBRARY
    else
        tar -x -z -f $COMPRESSED_LIBRARY
        mv boost_1_70_0 $LIBRARY
    fi
fi

cd $LIBRARY

./bootstrap.sh --with-libraries=system,filesystem,test --prefix=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY/$BUILD_TYPE

./b2 variant=$BUILD_TYPE --cxxflags=-fPIC link=shared runtime-link=shared threading=multi -j $NUMBER_OF_CORES --prefix=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY/$BUILD_TYPE install > /dev/null 2>&1
