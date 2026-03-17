#!/usr/bin/env python3

import os
import sys
import argparse

parser = argparse.ArgumentParser(
    description="Delete all hard links (synonyms) of FILE inside DIR and its subdirectories."
)
parser.add_argument("-f", "--file", help="source file")
parser.add_argument("-d", "--directory", help="directory to search")
args = parser.parse_args()
file = args.file
directory = args.directory
if not file or not directory:
    parser.print_help()
    sys.exit(1)
if not os.path.isfile(file):
    print(f"Error: file '{file}' does not exist")
    sys.exit(2)
if not os.path.isdir(directory):
    print(f"Error: directory '{directory}' does not exist")
    sys.exit(3)
file = os.path.abspath(file)
inode = os.stat(file).st_ino
for root, dirs, files in os.walk(directory):
    for name in files:
        path = os.path.join(root, name)
        file2 = os.path.abspath(path)
        inode2 = os.stat(path).st_ino
        if inode2 == inode and file2 != file:
            print("deleting", path)
            os.remove(path)
