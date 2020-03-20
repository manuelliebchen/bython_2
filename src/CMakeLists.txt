find_package(LLVM REQUIRED CONFIG)

set(GRAMMAR_PATH ${CMAKE_SOURCE_DIR}/grammar/bython.peg CACHE STRING "Path to the .peg gramma")

set(BYTHON bython)

add_executable(${BYTHON} main.cpp)
target_include_directories(${BYTHON} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/

    ${EXTERNAL_INSTALL_LOCATION}/cpp-peglib
    ${EXTERNAL_INSTALL_LOCATION}/cxxopts/include
    ${LLVM_INCLUDE_DIRS}
)

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_compile_definitions(${BYTHON} PUBLIC ${LLVM_DEFINITIONS} -DGRAMMAR_PATH="${GRAMMAR_PATH}")
target_link_libraries(${BYTHON} PUBLIC bython-ast bython-type ${llvm_libs})

add_subdirectory(ast)
add_subdirectory(type)
add_subdirectory(bc)
add_subdirectory(std)
