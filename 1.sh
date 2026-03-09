#!/bin/bash

file=$1
dir=$2

file=$(realpath "$file")
inode=$(ls -i "$file" | awk '{print $1}')

for f in $(find "$dir")
do
    if [ -f "$f" ]; then
        file2=$(realpath "$f")
        inode2=$(ls -i "$f" | awk '{print $1}')
        if [ "$inode2" = "$inode" ]; then
            if [ "$file2" != "$file" ]; then
                echo "deleting $f"
                rm "$f"
            fi
        fi
    fi
done