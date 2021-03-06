# HashOctree library
[![Build Status](https://travis-ci.com/lpiekarski/hash-octree.svg?branch=master)](https://travis-ci.com/lpiekarski/hash-octree)

HashOctree structure acts just like a normal octree structure but also saves a lot of memory by recognizing which sub-octrees are the same and holding them in a single sub-octree instance. Worst case scenario if every sub-octree is different it takes the same amount of memory as classic octree structure implemented in a 64-bit system.

# *Table of Contents*
* [Installation](#installation)
* [Usage](#usage)
* [How does it work](#how-does-it-work)
* [Release notes](#release-notes)
* [License](#license)

# Installation
### Requirements:
* `git`
* `cmake` version 3.9 or higher
### Steps:
1. Open bash
2. Go to directory from which you want the library to be referenced
3. Execute:
    ```bash
    git clone https://github.com/lpiekarski/hash-octree.git/
    cd hash-octree
    cmake .
    make hash-octree
    ```
4. Link `libhash-octree.a` file to your project
5. Use `#include "{DIR}/hash-octree/include/hashoctree.h"` in your code, where `{DIR}` is path to directory in which you called `git clone`

# Usage
If you are looking for a guide on how to use this library check out the wiki pages. Most common use case is to create a HashOctree instance, insert some data into it using `HashOctree::addDataShape` and `HashOctree::addDataPoint` and then mutate contained data or test it for collision against some other objects.

# How does it work
HashOctree structure contains a root sub-octree reference and a dictionary consisting of *(parent key -> children keys)* bindings. Keys are generated by a specific hashing function satisfying the following: keys of 2 different sub-octrees which have the same children' keys and store the same data are equal. 

# Release notes
There are currently no releases.

# License
If you plan to use this library make sure you're familiar with its [license](LICENSE) terms.
