find_package(LLVM REQUIRED CONFIG)

FILE(GLOB_RECURSE BYTHON_TYPE_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(bython-type SHARED ${BYTHON_TYPE_SOURCE_FILES})
target_include_directories(bython-type PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../ast/
    ${CMAKE_CURRENT_SOURCE_DIR}/../bc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../utillib/

    ${EXTERNAL_INSTALL_LOCATION}/cpp-peglib
    ${LLVM_INCLUDE_DIRS}
)

target_compile_definitions(bython-type PRIVATE ${LLVM_DEFINITIONS})

target_link_libraries(bython-type PRIVATE ${llvm_libs} bython-util)

if (CLANG_TIDY)
    set_target_properties(
            bython-ast PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()
