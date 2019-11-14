from debian:sid

# Everything in the folder is ignored, because the dependencies are pulled via git submodule or apt
# see .dockerignore

# Install deps from apt 
run apt update && \
        apt install -y ninja-build llvm-8-tools llvm-8-runtime llvm-8-dev clang-8 clang-tidy-8 clang-format-8 cmake git ca-certificates doxygen graphviz --no-install-recommends

# Set compilers to point to clang
run update-alternatives --install /usr/bin/cc cc /usr/bin/clang-8 100 && update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-8 100

# Set symlinks to fix include paths
run ln -s /usr/include/llvm-8/llvm /usr/include/llvm && ln -s /usr/include/llvm-c-8/llvm-c /usr/include/llvm-c
