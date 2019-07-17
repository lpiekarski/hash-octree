# HashOctree library

## Usage
### Installation
Requirements:
* `git`
* `cmake` version 3.14 or higher
1. Open bash
2. Go to directory from which you want the library to be referenced
3. Execute:
    ```{bash}
    git clone https://github.com/lpiekarski/hash-octree.git/
    cd hash-octree
    cmake .
    make hash-octree
    ```
4. Link `libhash-octree.a` file to your project
5. Use `#include "{DIR}/hash-octree/include/hashoctree.h"` in your code, where `{DIR}` is path to directory in which you called `git clone`