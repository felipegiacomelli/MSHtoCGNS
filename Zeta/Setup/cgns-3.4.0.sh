#!/bin/bash

LIBRARY="cgns-3.4.0"
export FLIBS="-Wl,--no-as-needed -ldl -lz"
export LIBS="-Wl,--no-as-needed -ldl -lz"
export CLIBS="-ldl"
CMAKE_FLAGS="-DCGNS_ENABLE_FORTRAN=FALSE -DCGNS_ENABLE_HDF5=TRUE -DHDF5_NEED_ZLIB=TRUE -DCGNS_BUILD_SHARED=TRUE"
BUILD_TYPE=`echo ${BUILD_TYPE,,}`

HDF5=hdf5-1.10.5
export HDF5_DIR=$LIBRARY_INSTALL_DIRECTORY/$HDF5/$BUILD_TYPE
HDF5_LIBS="-DHDF5_LIBRARIES=$HDF5_DIR/lib/libhdf5.so;$HDF5_DIR/lib/libhdf5_hl.so;$HDF5_DIR/lib/libhdf5_tools.so"

if [ ! -d "$LIBRARY" ]; then
    git clone -b master "https://github.com/CGNS/CGNS.git"
    mv CGNS $LIBRARY
fi

cd $LIBRARY
git checkout v3.4.0
search="FIND_HDF_COMPONENTS C shared"
replace="FIND_HDF_COMPONENTS C"
sed -i "s/${search}/${replace}/g" CMakeLists.txt
cat $SETUP_DIRECTORY/CMakeLists.txt >> CMakeLists.txt

mkdir build
cd build

echo "cmake .. $CMAKE_FLAGS -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY/$BUILD_TYPE $HDF5_LIBS"
cmake .. $CMAKE_FLAGS -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY/$BUILD_TYPE $HDF5_LIBS
make -j $NUMBER_OF_CORES
make install
