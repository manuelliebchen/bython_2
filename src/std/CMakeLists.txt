FILE(GLOB_RECURSE BYTHON_STD_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_library(bython-std SHARED ${BYTHON_STD_SOURCE_FILES})
target_include_directories(bython-std PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
