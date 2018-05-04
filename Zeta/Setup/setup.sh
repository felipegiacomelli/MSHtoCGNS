# !/bin/bash

export SETUP_DIRECTORY=$PWD
export LIBRARY_INSTALL_DIRECTORY=$PWD/../Libraries

cd ..

if [ ! -d Libraries ]; then
	mkdir Libraries
fi

cd $SETUP_DIRECTORY

. cgns-3.3.1.sh
if [ $? -eq 0 ]; then
    rm -rf $SETUP_DIRECTORY/cgns-3.3.1
fi

cd $SETUP_DIRECTORY

. boost-1.66.0.sh
if [ $? -eq 0 ]; then
	rm -rf $SETUP_DIRECTORY/boost-1.66.0
	rm -rf $SETUP_DIRECTORY/boost-1.66.0.tar.gz
fi
