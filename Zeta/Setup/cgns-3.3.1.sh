# !/bin/bash

LIBRARY="cgns-3.3.1"
export FLIBS="-Wl,--no-as-needed -ldl -lz"
export LIBS="-Wl,--no-as-needed -ldl -lz"
export CLIBS="-ldl"

git clone -b master https://github.com/CGNS/CGNS.git

cd CGNS
cd src
./../src/configure --without-fortran --disable-cgnstools --enable-shared \
                   --enable-debug --prefix=$PWD/../../Libraries/$LIBRARY
make
make install