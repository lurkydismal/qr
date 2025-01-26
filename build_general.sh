#!/bin/bash
TARGET_DIRECTORY=$1

source "$SCRIPT_DIRECTORY/config.sh" && \
    cd $TARGET_DIRECTORY && \
    make clean && \
    make -j $(( `nproc` - 1 )) "C_FLAGS=$2" "DEFINES=$3" && \
    mv "$OUTPUT_FILE" "$BUILD_DIRECTORY" && \
    clang-format --style="file:$SCRIPT_DIRECTORY/.clang-format" \
    -i \
    $FILE_LIST && \
    cd "$SCRIPT_DIRECTORY"
