cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(bython)

find_program(CLANG_TIDY
        NAMES "clang-tidy")
if (CLANG_TIDY)
    message(STATUS "clang-tidy found: ${CLANG_TIDY}")
    set(CLANG_TIDY_COMMAND
            "${CLANG_TIDY}" -checks=-*,bugprone-*,cert-*,clang-*,cppcoreguidelines-*,modernize-*,performance-*,readability-* -header-filter='^include')
endif()

find_package(Git REQUIRED)
find_package(Threads REQUIRED)

set(EXTERNAL_INSTALL_LOCATION ${PROJECT_SOURCE_DIR}/dep)

add_subdirectory(src)
add_subdirectory(stdlib)

FILE(GLOB_RECURSE ALL_BYTHON_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

# Adding clang-format target if executable is found
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  add_custom_target(
    clang-format ALL
    COMMAND ${CLANG_FORMAT}
    -i
    -style=LLVM
    ${ALL_BYTHON_SOURCE_FILES}
    )
else()
  message(STATUS "Could not find clang-format")
endif()

OPTION(BUILD_DOXYGEN "Build the documentation" ON)
if(BUILD_DOXYGEN)
    find_package(Doxygen)
    if(DOXYGEN)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
        
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        
        add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
    endif()
endif()