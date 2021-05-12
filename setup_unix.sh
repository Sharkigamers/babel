#!/usr/bin/env bash

BUILD_DIR="./build"
DEBUG_QT="./client/build-clientUI-Desktop-Debug"

rm -rf $BUILD_DIR
rm -rf $DEBUG_QT
mkdir $BUILD_DIR && cd $BUILD_DIR && conan install .. --build missing && cmake .. -G "Unix Makefiles" && cmake --build . && cp ./bin/* ../