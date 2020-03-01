add_subdirectory(ast)
add_subdirectory(type)

find_package(LLVM REQUIRED CONFIG)

set(BYTHON bython)

add_executable(${BYTHON} main.cpp)
target_include_directories(${BYTHON} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/

    ${EXTERNAL_INSTALL_LOCATION}/cpp-peglib
    ${EXTERNAL_INSTALL_LOCATION}/cxxopts/include
    ${LLVM_INCLUDE_DIRS}
)

set(GRAMMAR_PATH ${CMAKE_SOURCE_DIR}/grammar/bython.peg CACHE STRING "Path to the .peg gramma")
target_compile_definitions(${BYTHON} PRIVATE ${LLVM_DEFINITIONS} -DGRAMMAR_PATH="${GRAMMAR_PATH}")
message(STATUS "Grammar set to ${GRAMMAR_PATH}")

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(${BYTHON} PRIVATE bython-ast bython-type ${llvm_libs})

set( CMAKE_CXX_FLAGS "-O2 -Wall -pthread")
set( CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
