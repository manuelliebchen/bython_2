# Bython 2

## Installation

Dependencies: ``clang llvm doxygen``

```
git clone git@github.com:manuelliebchen/bython_2.git
cd bython_2
git submodule update --init --remote --recursive
mkdir build
cd build
export C=/usr/bin/clang; export CXX=/usr/bin/clang++
cmake ..
make
```

## Usage

When in build directory.

```
bin/bython ../examples/hello_world.by 1> hello_world.ll
clang -o hello_world hello_world.ll lib/libbython-stdlib.so
export LD_LIBRARY_PATH=lib
./hello_world
```

## Todo
- extern func ...
- list functions
- records