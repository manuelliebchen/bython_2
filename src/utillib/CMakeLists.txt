FILE(GLOB_RECURSE BYTHON_UTIL_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(bython-util ${BYTHON_UTIL_SOURCE_FILES})
target_include_directories(bython-util PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/../AST

    ${EXTERNAL_INSTALL_LOCATION}/cpp-peglib
)

if (CLANG_TIDY)
    set_target_properties(
            bython-util PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    message(STATUS "Enabled clang-tidy for bython-main")
endif()
