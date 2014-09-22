
cmake_minimum_required( VERSION 3.0.0 )

project( mockgen )

### find llvm ###
set( LLVM_PREFIX $ENV{LLVM_PREFIX} CACHE PATH "LLVM/Clang install prefix" )

if ( NOT LLVM_PREFIX )
    message( FATAL_ERROR "Please set LLVM_PREFIX" )
endif()

include_directories( ${LLVM_PREFIX}/include )
link_directories( ${LLVM_PREFIX}/lib )

if ( WIN32 )
    # these came from `llvm-config.exe --cxxflags`
    add_definitions(
        -D_CRT_SECURE_NO_DEPRECATE
        -D_CRT_SECURE_NO_WARNINGS
        -D_CRT_NONSTDC_NO_DEPRECATE
        -D_CRT_NONSTDC_NO_WARNINGS
        -D_SCL_SECURE_NO_DEPRECATE
        -D_SCL_SECURE_NO_WARNINGS
        -D__STDC_CONSTANT_MACR
        -D__STDC_FORMAT_MACROS
        -D__STDC_LIMIT_MACROS
        -wd4146
        -wd4180
        -wd4244
        -wd4258
        -wd4267
        -wd4291
        -wd4345
        -wd4351
        -wd4355
        -wd4503
        -wd4624
        -wd4722
        -wd4800
        -w14062
        -we4238
        )
endif()

add_executable( mockgen
  Main.cpp
  )

target_link_libraries( mockgen
  clangTooling
  clangFrontend
  clangDriver
  LLVMOption
  clangParse
  LLVMMCParser
  clangSerialization
  clangSema
  clangEdit
  clangAnalysis
  LLVMBitReader
  clangASTMatchers
  clangAST
  clangLex
  clangBasic
  LLVMMC
  LLVMSupport
  )

