#!/bin/bash

function flash() {
    file_name=$1"_"$2/$1.bin
    if [ ! -f $file_name ]
    then
        echo "File: \""$file_name"\" not found"
        exit 1
    fi
    echo "write firmware: "$file_name
    st-flash --reset write $file_name 0x08000000
}

export TARGET=${PWD##*/};

if [ "$1" == "debug" ]
then
    flash $TARGET Debug
    exit $?;
fi

if [ "$1" == "release" ]
then
    flash $TARGET Release
    exit $?;
fi

echo "usage: flash.sh [debug|release]"
