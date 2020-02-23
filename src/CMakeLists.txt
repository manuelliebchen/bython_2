add_subdirectory(ast)
add_subdirectory(type)
add_subdirectory(utillib)

find_package(LLVM REQUIRED CONFIG)

add_executable(bython main.cpp)
target_include_directories(bython PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/ast
    ${CMAKE_CURRENT_SOURCE_DIR}/type
    ${CMAKE_CURRENT_SOURCE_DIR}/bc
    ${CMAKE_CURRENT_SOURCE_DIR}/utillib

    ${EXTERNAL_INSTALL_LOCATION}/cpp-peglib
    ${EXTERNAL_INSTALL_LOCATION}/cxxopts/include
    ${LLVM_INCLUDE_DIRS}
)

set(GRAMMAR_PATH ${CMAKE_SOURCE_DIR}/resources/bython.peg CACHE STRING "Path to the .peg gramma")
target_compile_definitions(bython PRIVATE ${LLVM_DEFINITIONS} -DGRAMMAR_PATH="${GRAMMAR_PATH}")
message(STATUS "Grammar set to ${GRAMMAR_PATH}")

llvm_map_components_to_libnames(llvm_libs support core irreader)

set( CMAKE_CXX_FLAGS "-O2 -Wall -pthread")
target_link_libraries(bython PRIVATE ${llvm_libs} bython-ast bython-util bython-type)

if (CLANG_TIDY)
    set_target_properties(
            bython-ast PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    message(STATUS "Enabled clang-tidy for bython-main")
endif()
