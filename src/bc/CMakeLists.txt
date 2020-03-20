find_package(LLVM REQUIRED CONFIG)

FILE(GLOB_RECURSE BYTHON_BC_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(bython-bc SHARED ${BYTHON_BC_SOURCE_FILES})
target_include_directories(bython-bc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../

    ${EXTERNAL_INSTALL_LOCATION}/cpp-peglib
    ${LLVM_INCLUDE_DIRS}
)

target_compile_definitions(bython-bc PUBLIC ${LLVM_DEFINITIONS})
#target_link_libraries(bython-bc PUBLIC ${llvm_libs})
