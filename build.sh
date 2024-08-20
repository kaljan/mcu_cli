#!/bin/bash

export TARGET=${PWD##*/};
export BUILD_MODE="release"
export MCU_NAME="stm32f103c8"
export REBUILD="no"
export RELEASE=1
export DEBUG=0

function print_info() {
    echo -e "\e[1m---- \e[33mENVIRONMENT\e[37m ----"
    echo -e "\e[1;35mTARGET       \e[37m: \e[36m"$TARGET
    echo -e "\e[1;35mBUILD_MODE   \e[37m: \e[36m"$BUILD_MODE
    echo -e "\e[1;35mMCU_NAME     \e[37m: \e[36m"$MCU_NAME
    echo -e "\e[1;35mDEBUG        \e[37m: \e[33m"$DEBUG
    echo -e "\e[1;35mRELEASE      \e[37m: \e[33m"$RELEASE
    echo -en "\e[0m"
    echo -e "\e[1m---- \e[33mProject info from Makefile\e[37m ----"
    make target_info
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
            [ "$BUILD_MODE" == "debug" ] && export DEBUG=1
            [ "$BUILD_MODE" == "release" ] && export RELEASE=1
            shift
            ;;
        -d=*|--device=*)
            MCU_NAME="${1#*=}"
            shift
            ;;
        -r|--rebuild)
            REBUILD="yes"
            shift
            ;;
        -c|--clean)
            print_info
            make clean
            exit 0
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
            echo "-m=<MODE>,--mode=<MODE>       Select build mode debug or release"
            echo "-d=<DEVICE>,--device=<DEVICE> Select device stm32f103cb with 64k flash or stm32f103cb with 128k flash"
            echo "-f=<FILE>,--file=<FILE>       Get device name from file FILE"
            echo "-r,--rebuild                  Rebuild project"
            echo "-c,--clean                    Clean project"
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

export_mode $BUILD_MODE

print_info

if [ $REBUILD == "yes" ]
then
echo -e "\e[1m+--------------------------------"
echo -e "| \e[31mClean "$TARGET"_"$MCU_NAME
echo -e "\e[37m+--------------------------------"
echo -e "\e[0m"
    make clean
fi

echo -e "\e[1m+--------------------------------"
echo -e "| \e[34mBuild "$TARGET"_"$MCU_NAME
echo -e "\e[37m+--------------------------------"

time colormake
