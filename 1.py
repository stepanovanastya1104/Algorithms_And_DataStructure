import os
import sys

file = sys.argv[1]
directory = sys.argv[2]
file = os.path.abspath(file)
inode = os.stat(file).st_ino
for root, dirs, files in os.walk(directory):
    for name in files:
        path = os.path.join(root, name)
        file2 = os.path.abspath(path)
        inode2 = os.stat(path).st_ino
        if inode2 == inode and file2 != file:
            print("remove", path)

            os.remove(path)
