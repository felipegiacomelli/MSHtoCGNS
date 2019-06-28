#!/bin/bash

LIBRARY=hdf5-1.10.5
DOWNLOAD_LINK="https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.5/src/hdf5-1.10.5.tar.gz"
COMPRESSED_LIBRARY=hdf5-1.10.5.tar.gz
CMAKE_FLAGS="-DHDF5_BUILD_CPP_LIB=FALSE -DHDF5_BUILD_FORTRAN=FALSE -DBUILD_SHARED_LIBS=TRUE -DH5_ENABLE_STATIC_LIB=FALSE -DBUILD_STATIC_EXECS=FALSE"
BUILD_TYPE=`echo ${BUILD_TYPE,,}`

if [ ! -d "$LIBRARY" ]; then
    if [ ! -f "$COMPRESSED_LIBRARY" ]; then
        wget $DOWNLOAD_LINK -O $COMPRESSED_LIBRARY
        tar -x -z -f $COMPRESSED_LIBRARY -C ./
    else
        tar -x -z -f $COMPRESSED_LIBRARY -C ./
    fi
fi

cd $LIBRARY
mkdir build
cd build

cmake .. $CMAKE_FLAGS -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY/$BUILD_TYPE >\dev\null
make -j $NUMBER_OF_CORES >/dev/null 2>&1
make install >/dev/null 2>&1
