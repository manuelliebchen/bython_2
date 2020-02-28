find_package(LLVM REQUIRED CONFIG)

FILE(GLOB_RECURSE BYTHON_AST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(bython-ast SHARED ${BYTHON_AST_SOURCE_FILES})
target_include_directories(bython-ast PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/expression/
    ${CMAKE_CURRENT_SOURCE_DIR}/constant/
    ${CMAKE_CURRENT_SOURCE_DIR}/../type/
    ${CMAKE_CURRENT_SOURCE_DIR}/../bc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../utillib/

    ${EXTERNAL_INSTALL_LOCATION}/cpp-peglib
    ${LLVM_INCLUDE_DIRS}
)

target_compile_definitions(bython-ast PRIVATE ${LLVM_DEFINITIONS} -DGRAMMAR_PATH="${GRAMMAR_PATH}")

set( CMAKE_CXX_FLAGS "-O2 -Wall -pthread")
target_link_libraries(bython-ast PRIVATE bython-util bython-type)

if (CLANG_TIDY)
    set_target_properties(
            bython-ast PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()
