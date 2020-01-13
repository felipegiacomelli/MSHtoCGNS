#!/bin/bash

rm -rf *.tar.gz

if [ ! -d "boost-$BOOST_VERSION" ]; then
    rm -rf boost-$BOOST_VERSION
fi

if [ ! -d "cgns-$CGNS_VERSION" ]; then
    rm -rf cgns-$CGNS_VERSION
fi

cd ..
rm -rf Libraries
mkdir Libraries

cd Setup
