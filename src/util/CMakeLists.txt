FILE(GLOB_RECURSE BYTHON_UTIL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(bython-util SHARED ${BYTHON_UTIL_SOURCE_FILES})
target_include_directories(bython-util PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../

    ${EXTERNAL_INSTALL_LOCATION}/cpp-peglib
)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
