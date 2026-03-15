#!/bin/bash

help() {
echo "Usage: $0 [OPTION]... FILE DIR"
echo
echo "Delete all hard links (synonyms) of FILE inside DIR and its subdirectories."
echo
echo "Options:"
echo "  -h, --help     show this help message and exit"
echo "Exit status:"
echo " 0   if OK,"
echo " 1   if invalid arguments or missing parameters"
echo " 2   if FILE does not exist"
echo " 3   if DIR does not exist or is not a directory"
}

if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    help
    exit 0
fi
if [ $# -ne 2 ]; then
    help
    exit 1
fi
file=$1
dir=$2
if [ ! -f "$file" ]; then
    echo "Error: file '$file' does not exist"
    exit 2
fi
if [ ! -d "$dir" ]; then
    echo "Error: directory '$dir' does not exist"
    exit 3
fi
file=$(realpath "$file")
inode=$(ls -i "$file" | awk '{print $1}')
for f in $(find "$dir" -type f)
do
    inode2=$(ls -i "$f" | awk '{print $1}')
    file2=$(realpath "$f")
    if [ "$inode2" = "$inode" ] && [ "$file2" != "$file" ]; then
        echo "deleting $f"
        rm "$f"
    fi
done
