#!/bin/bash

KERNEL_NAME=$(uname -s)

if [ $KERNEL_NAME == "Linux" ]; then
    export LD_PRELOAD="./libft_malloc.so"
elif [ $KERNEL_NAME == "Darwin" ]; then
    export DYLD_LIBRARY_PATH=.
    export DYLD_INSERT_LIBRARIES="libft_malloc.so"
    export DYLD_FORCE_FLAT_NAMESPACE=1
else
echo "Unknown kernel $KERNEL_NAME"
exit 1
fi
$@
