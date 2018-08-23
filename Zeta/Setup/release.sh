# !/bin/bash

export SETUP_DIRECTORY=$PWD
export LIBRARY_INSTALL_DIRECTORY=$PWD/../Libraries

cd ..

if [ ! -d Libraries ]; then
	mkdir Libraries
fi

cd $SETUP_DIRECTORY

	LIBRARY="cgns-3.3.1"
	export FLIBS="-Wl,--no-as-needed -ldl -lz"
	export LIBS="-Wl,--no-as-needed -ldl -lz"
	export CLIBS="-ldl"

	git clone -b master https://github.com/CGNS/CGNS.git
	mv CGNS $LIBRARY
	cd $LIBRARY
	cd src
	./../src/configure --without-fortran --disable-cgnstools --enable-shared \
	                   --disable-debug --prefix=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY
	make -j 4
	make install

if [ $? -eq 0 ]; then
    rm -rf $SETUP_DIRECTORY/cgns-3.3.1
fi

cd $SETUP_DIRECTORY

	LIBRARY="boost-1.67.0"
	DOWNLOAD_LINK="https://downloads.sourceforge.net/project/boost/boost/1.67.0/boost_1_67_0.tar.gz"
	COMPRESSED_LIBRARY=$LIBRARY.tar.gz
	VARIANT="release"

	wget $DOWNLOAD_LINK -O $COMPRESSED_LIBRARY

	tar -x -z -f $COMPRESSED_LIBRARY
	mv boost_1_67_0 $LIBRARY
	cd $LIBRARY
	./bootstrap.sh --with-libraries=system,filesystem,test \
				   --prefix=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY

	./b2 	variant=$VARIANT \
			--cxxflags=-fPIC \
			link=shared runtime-link=shared \
			threading=multi \
			-j 4			\
		 	--prefix=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY install

if [ $? -eq 0 ]; then
	rm -rf $SETUP_DIRECTORY/boost-1.67.0
	rm -rf $SETUP_DIRECTORY/boost-1.67.0.tar.gz
fi
