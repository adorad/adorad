r"""
    During CI, Clang and MSVC implicitely place the binaries in `build/bin/{CMAKE_BUILD_TYPE}` where CMAKE_BUILD_TYPE
    is one of:
        1. Debug
        2. Release
        3. ReleaseWithDebInfo
        4. MinSizeRelease
    
    This makes it harder for us to run tests (especially the I/O operations which are not as straightforward as a
    language like Python). 

    This script essentially copies all the binaries within `build/bin/{CMAKE_BUILD_TYPE}` and places them in 
    `build/bin/`.
"""

import os 
import shutil
import sys 

ospd = os.path.dirname
# Navigate to root folder
# ../../../
ROOT = ospd(ospd(ospd(os.path.abspath(__file__))))
SOURCEROOT = ROOT
DESTROOT = os.path.join(SOURCEROOT, 'build', 'bin')

def run(CMAKE_BUILD_TYPE: str):
    print(' ---------------------- PYTHON BEFORE TESTS CI ----------------------')
    print(' ----------------------          RUN           ----------------------')
    copied = []
    path : str = os.path.join(DESTROOT, CMAKE_BUILD_TYPE)
    for binary in os.listdir(path):
        binary_src_path = os.path.join(path, binary)
        binary_dest_path = os.path.join(DESTROOT, binary)
        shutil.copy(binary_src_path, binary_dest_path)
        copied.append(binary)
    
    print('Copied the following files: \n')
    c : int = 1
    for i in copied:
        print(f'    {c}: {i}')
        c += 1

    print(' ----------------------          EXIT           ----------------------')
    

if __name__ == '__main__':
    if(len(sys.argv) < 2):
        print('Require only 1 argument : the `CMAKE_BUILD_TYPE`')
        sys.exit(1)

    if(len(sys.argv) > 2):
        print('Require only 1 argument : the `CMAKE_BUILD_TYPE`')
        sys.exit(1)

    CMAKE_BUILD_TYPE = sys.argv[1]
    run(CMAKE_BUILD_TYPE)