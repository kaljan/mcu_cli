#!/bin/bash

export TARGET=${PWD##*/};
export BUILD_MODE="release"
export MCU_NAME="stm32f103c8"
export FLASH_ADDRESS="0x08000000"
export BUILD_SUFFIX="_Release"

function print_info() {
    echo -e "\e[1m---- \e[33mENVIRONMENT\e[37m ----"
    echo -e "\e[1;35mTARGET        \e[37m: \e[36m"$TARGET
    echo -e "\e[1;35mBUILD_MODE    \e[37m: \e[36m"$BUILD_MODE
    echo -e "\e[1;35mMCU_NAME      \e[37m: \e[36m"$MCU_NAME
    echo -e "\e[1;35mFLASH_ADDRESS \e[37m: \e[33m"$FLASH_ADDRESS
    echo -en "\e[0m"
}

function export_mode() {
    if [ $1 == "debug" ]
    then
        DEBUG=1
        RELEASE=0
    elif [ $1 == "release" ]
    then
        DEBUG=0
        RELEASE=1
    fi
}


function parse_args() {
    case $1 in
        -m=*|--mode=*)
            BUILD_MODE="${1#*=}"
            if [ "$BUILD_MODE" == "debug" ]
            then
                BUILD_SUFFIX="_Debug"
                DEBUG=1
            fi

            if [ "$BUILD_MODE" == "release" ]
            then
                BUILD_SUFFIX="_Release"
                RELEASE=1
            fi
            shift
            ;;
        -d=*|--device=*)
            MCU_NAME="${1#*=}"
            shift
            ;;
        -a=*|--address=*)
            FLASH_ADDRESS="${1#*=}"
            shift
            ;;
        -i|--info)
            print_info
            exit 0
            ;;
        -f=*|--file=*)
            SCRIPT="${1#*=}"
            echo "Source script: "$SCRIPT
            if [ -f $SCRIPT ]
            then
                source $SCRIPT
            else
                echo $SCRIPT": No such file or directory"
                exit 1
            fi
            shift
            ;;
        -h|--help)
            print_info
            echo "-m=<MODE>,--mode=<MODE>        Select build mode debug or release"
            echo "-d=<DEVICE>,--device=<DEVICE>  Select device stm32f103cb with 64k flash or stm32f103cb with 128k flash"
            echo "-a=<ADDR>,--address=<ADDR>     Flash memory start address"
            exit 0
            ;;
        -*|--*)
            echo "bad option: $1"
            exit 1
            ;;
        *)
        ;;
    esac

}

for i in "$@"
do
    parse_args $i
done

export BIN_PATH=$TARGET"_"$MCU_NAME$BUILD_SUFFIX/""$TARGET"_"$MCU_NAME".bin"
export ELF_PATH=$TARGET"_"$MCU_NAME$BUILD_SUFFIX/""$TARGET"_"$MCU_NAME".elf"

export ARTIFACT_PATH=$TARGET"_"$MCU_NAME$BUILD_SUFFIX/""$TARGET"_"$MCU_NAME

export_mode $BUILD_MODE

print_info

function flash() {
    if [ -z "$2" ] && [ ! -f $1 ]
    then
        echo "File: \""$1"\" not found"
        exit 1
    fi
    if [ -z "$2" ]
    then
        echo "ERROR: address is not set"
        exit 1
    fi

    echo "write firmware: "$1" to "$2
    st-flash --reset write $1 $2
}

flash $BIN_PATH $FLASH_ADDRESS

exit
