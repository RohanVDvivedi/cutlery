# cutlery
A c library that provides configurable data structures, serving as a backpack for any systems programming project.

## Setup instructions

**Download source code :**
 * `git clone https://github.com/RohanVDvivedi/cutlery.git`

**Build from source :**
 * `cd cutlery`
 * `make clean all`

**Install from the build :**
 * `sudo make install`
 * ***Once you have installed from source, you may discard the build by*** `make clean`

## Using The library
 * add `-lcutlery` linker flag, while compiling your application
 * do not forget to include appropriate public api headers as and when needed. this includes
   * `#include<array.h>`
   * `#include<stack.h>`
   * `#include<queue.h>`
   * `#include<heap.h>`
   * `#include<linkedlist.h>`
   * `#include<bst.h>`
   * `#include<hashmap.h>`
   * `#include<dstring.h>`
   * `#include<bitmap.h>`

## Instructions for uninstalling library

**Uninstall :**
 * `cd cutlery`
 * `sudo make uninstall`