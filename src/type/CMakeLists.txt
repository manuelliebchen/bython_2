project(bython-type)
set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)

set(BYTHON_AST_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/FunctionType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TypeName.cpp
)

add_library(bython-type ${BYTHON_AST_SOURCE_FILES})
target_include_directories(bython-type PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../ast/
    ${CMAKE_CURRENT_SOURCE_DIR}/../bc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../utillib/

    ${EXTERNAL_INSTALL_LOCATION}/cpp-peglib
    ${LLVM_INCLUDE_DIRS}
)

target_compile_definitions(bython-type PRIVATE ${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(bython-type PRIVATE ${llvm_libs} bython-util)

if (CLANG_TIDY)
    set_target_properties(
            bython-ast PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    message(STATUS "Enabled clang-tidy for bython-main")
endif()
