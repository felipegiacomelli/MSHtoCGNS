# !/bin/bash

LIBRARY="cgns-3.3.1"
export FLIBS="-Wl,--no-as-needed -ldl -lz"
export LIBS="-Wl,--no-as-needed -ldl -lz"
export CLIBS="-ldl"

git clone -b master https://github.com/CGNS/CGNS.git
mv CGNS $LIBRARY
cd $LIBRARY
cd src
./../src/configure --without-fortran --disable-cgnstools --enable-shared \
                   --enable-debug --prefix=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY
make -j 2
make install