#!/bin/bash
export SCRIPT_DIRECTORY=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export BUILD_DIRECTORY_NAME='out'
export BUILD_DIRECTORY="$SCRIPT_DIRECTORY/$BUILD_DIRECTORY_NAME"
export BUILD_C_FLAGS="-flto -std=c99 -nostdlib -nostartfiles -m32 -Os -march=native -ffunction-sections -fdata-sections -fno-PIC -fno-stack-protector -fopenmp-simd -s -fno-ident -fno-asynchronous-unwind-tables -ffreestanding -falign-jumps=1 -falign-loops=1 -falign-functions=1 -fshort-enums -fpack-struct -static -Wall -Wextra"
export BUILD_DEFINES=""
export LINK_FLAGS="-flto -nostdlib -nostartfiles -m32 -fno-PIC -O1 -s -Wl,--static -static -fuse-ld=mold -Wl,--gc-sections -Wl,--omagic -Wl,--nmagic -Wl,--no-eh-frame-hdr"
export LINKER="ccache gcc"
export EXECUTABLE_NAME="main.out"
export declare EXECUTABLE_SECTIONS_TO_STRIP=(
    ".note.gnu.build-id"
    ".note.gnu.property"
    ".comment"
    ".eh_frame"
    ".eh_frame_hdr"
    ".relro_padding"
    ".got"
    ".got.plt"
    ".bss"
)

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

    printf -v sectionsToStripAsString -- "--remove-section %s " "${EXECUTABLE_SECTIONS_TO_STRIP[@]}"

    echo $sectionsToStripAsString

    if [ -z "${IS_CI}" ]; then
        objcopy "$BUILD_DIRECTORY/$EXECUTABLE_NAME" $sectionsToStripAsString

        strip --strip-section-headers "$BUILD_DIRECTORY/$EXECUTABLE_NAME"
    fi
fi

}
