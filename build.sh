#!/bin/bash
export SCRIPT_DIRECTORY=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export BUILD_DIRECTORY_NAME='out'
export BUILD_DIRECTORY="$SCRIPT_DIRECTORY/$BUILD_DIRECTORY_NAME"
export BUILD_C_FLAGS="-flto -nostdlib -nostartfiles -m32 -Os -march=native -ffunction-sections -fdata-sections -fno-PIC"
export BUILD_DEFINES=""
export LINK_FLAGS="-flto -nostdlib -nostartfiles -m32 -s -O1 -Wl,--gc-sections"
export LINKER="ccache gcc"
export EXECUTABLE_NAME="main.out"

clear

source './config.sh' && {

mkdir -p "$BUILD_DIRECTORY"

for partToBuild in "${partsToBuild[@]}"; do

    source "$partToBuild/config.sh" && './build_general.sh' "$partToBuild" "$BUILD_C_FLAGS" "$BUILD_DEFINES"

    if [ $? -ne 0 ]; then
        break
    fi

done

if [ $? -eq 0 ]; then
    printf -v partsToBuildAsString "$BUILD_DIRECTORY/lib%s.a " "${partsToBuild[@]}"

    echo $partsToBuildAsString

    $LINKER $LINK_FLAGS $partsToBuildAsString -o $BUILD_DIRECTORY/$EXECUTABLE_NAME
fi

}
