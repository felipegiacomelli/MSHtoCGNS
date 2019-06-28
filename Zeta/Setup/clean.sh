#!/bin/bash

rm -rf *.tar.gz

if [ ! -d "boost-1.70.0" ]; then
    rm -rf boost-1.70.0
fi

if [ ! -d "hdf5-1.10.5" ]; then
    rm -rf hdf5-1.10.5
fi

if [ ! -d "cgns-3.4.0" ]; then
    rm -rf cgns-3.4.0
fi

cd ..
rm -rf Libraries
mkdir Libraries

cd Setup
