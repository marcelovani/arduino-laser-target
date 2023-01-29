#!/bin/bash
clear

mkdir -p build

# Load includes
includes=$(cat main.ino | grep '^#include\s\".*[\.cpp|\.h]' | tr -d '"')
echo ${includes}

# Init build
echo > ./build/main.ino

for item in ${includes}
do
    if [[ "$item" == *".h"* || "$item" == *".cpp"* ]]; then
        cat $item >> ./build/main.ino
        echo >> ./build/main.ino
    fi
done


cat main.ino | grep -v 'include "' >> ./build/main.ino
echo Generated ./build/main.ino
