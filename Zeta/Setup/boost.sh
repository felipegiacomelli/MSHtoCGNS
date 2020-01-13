#!/bin/bash

LIBRARY="boost-$BOOST_VERSION"
DOWNLOAD_VERSION="${BOOST_VERSION//./_}"
DOWNLOAD_LINK="https://downloads.sourceforge.net/project/boost/boost/$BOOST_VERSION/boost_$DOWNLOAD_VERSION.tar.gz"
COMPRESSED_LIBRARY=$LIBRARY.tar.gz
BUILD_TYPE=`echo ${BUILD_TYPE,,}`

if [ ! -d "$LIBRARY" ]; then
    if [ ! -f "$COMPRESSED_LIBRARY" ]; then
        wget $DOWNLOAD_LINK -O $COMPRESSED_LIBRARY
        tar -x -z -f $COMPRESSED_LIBRARY
        mv "boost_$DOWNLOAD_VERSION" $LIBRARY
    else
        tar -x -z -f $COMPRESSED_LIBRARY
        mv "boost_$DOWNLOAD_VERSION" $LIBRARY
    fi
fi

cd $LIBRARY

./bootstrap.sh --with-libraries=system,filesystem,test,serialization,mpi --prefix=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY/$BUILD_TYPE
echo "using mpi ;" >> project-config.jam
./b2 variant=$BUILD_TYPE --cxxflags=-fPIC link=shared runtime-link=shared threading=multi -j $NUMBER_OF_CORES --prefix=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY/$BUILD_TYPE install > /dev/null 2>&1
