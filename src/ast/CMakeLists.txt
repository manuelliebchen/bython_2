find_package(LLVM REQUIRED CONFIG)

set(BYTHON_AST_LIB bython-ast)

FILE(GLOB_RECURSE BYTHON_AST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${BYTHON_AST_LIB} SHARED ${BYTHON_AST_SOURCE_FILES})
target_include_directories(${BYTHON_AST_LIB} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../

    ${EXTERNAL_INSTALL_LOCATION}/cpp-peglib
    ${LLVM_INCLUDE_DIRS}
)

target_compile_definitions(${BYTHON_AST_LIB} PRIVATE ${LLVM_DEFINITIONS} -DGRAMMAR_PATH="${GRAMMAR_PATH}")

set( CMAKE_CXX_FLAGS "-O2 -Wall -pthread")
target_link_libraries(${BYTHON_AST_LIB} PRIVATE bython-util bython-type)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")