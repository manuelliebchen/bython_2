cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(bython)

set(EXTERNAL_INSTALL_LOCATION ${PROJECT_SOURCE_DIR}/dep)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set( CMAKE_CXX_FLAGS "-O2 -Wall -pthread")

OPTION(CLANG_TIDY "Use clang-tidy warnings" ON)
if(CLANG_TIDY)
	set( CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*,-fuchsia-*,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-cert-dcl58-cpp,-cert-err58-cpp")
endif()

add_subdirectory(src)

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/examples DESTINATION ${CMAKE_BINARY_DIR})

FILE(GLOB_RECURSE ALL_BYTHON_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

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