# !/bin/bash

LIBRARY="boost-1.66.0"
DOWNLOAD_LINK="https://downloads.sourceforge.net/project/boost/boost/1.66.0/boost_1_66_0.tar.gz"
COMPRESSED_LIBRARY=$LIBRARY.tar.gz
VARIANT="debug"

wget $DOWNLOAD_LINK -O $COMPRESSED_LIBRARY

tar -x -z -f $COMPRESSED_LIBRARY
mv boost_1_66_0 $LIBRARY
cd $LIBRARY
./bootstrap.sh --with-libraries=system,filesystem,test \
			   --prefix=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY

./b2 	variant=$VARIANT \
		--cxxflags=-fPIC \
		link=shared runtime-link=shared \
		threading=multi \
	 	--prefix=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY install
