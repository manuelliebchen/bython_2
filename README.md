# Bython 2

Bython is a small functional programming language implementet with the help of peglib and llvm.
This is based on [bython](https://gitlab.com/compilerbau-ovgu/bython).

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
clang -o hello_world hello_world.ll -Llibc.so
export LD_LIBRARY_PATH=lib
./hello_world
```

## Todo
- list functions
- records