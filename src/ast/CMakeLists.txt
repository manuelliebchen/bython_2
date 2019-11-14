project(bython-ast)
set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)

set(BYTHON_AST_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTConstant/ASTStringConstant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTConstant/ASTBooleanConstant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTConstant/ASTFloatConstant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTConstant/ASTIntegerConstant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTConstant/ASTConstant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTExpression/ASTArithmeticExpression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTExpression/ASTNoneArithmeticExpression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTExpression/ASTExpression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTExpression/ASTVariableExpression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTExpression/ASTIfExpression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTExpression/ASTCallExpression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTExpression/ASTLetStatement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTExpression/ASTBlockExpression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTVariableDeclaration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTFunction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTRoot.cpp
)

add_library(bython-ast ${BYTHON_AST_SOURCE_FILES})
target_include_directories(bython-ast PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTExpression/
    ${CMAKE_CURRENT_SOURCE_DIR}/ASTConstant/
    ${CMAKE_CURRENT_SOURCE_DIR}/../type/
    ${CMAKE_CURRENT_SOURCE_DIR}/../bc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../utillib/

    ${EXTERNAL_INSTALL_LOCATION}/cpp-peglib
    ${LLVM_INCLUDE_DIRS}
)

set(GRAMMAR_PATH ${CMAKE_SOURCE_DIR}/resources/bython.peg)
target_compile_definitions(bython-ast PRIVATE ${LLVM_DEFINITIONS} -DGRAMMAR_PATH="${GRAMMAR_PATH}")
message(STATUS "Grammar set to ${GRAMMAR_PATH}")

llvm_map_components_to_libnames(llvm_libs support core irreader)

set( CMAKE_CXX_FLAGS "-O2 -Wall -pthread")
target_link_libraries(bython-ast PRIVATE ${llvm_libs} bython-util bython-type)

if (CLANG_TIDY)
    set_target_properties(
            bython-ast PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    message(STATUS "Enabled clang-tidy for bython-main")
endif()
