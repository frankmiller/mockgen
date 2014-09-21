
cmake_minimum_required( VERSION 3.0.0 )

project( mockgen )

include_directories( C:/opt/llvm/include )

add_executable( mockgen
  Main.cpp
  )

set( clangDir "C:/opt/llvm/lib" )

target_link_libraries( mockgen
  ${clangDir}/clangTooling.lib
  ${clangDir}/clangFrontend.lib
  ${clangDir}/clangDriver.lib
  ${clangDir}/LLVMOption.lib
  ${clangDir}/clangParse.lib
  ${clangDir}/LLVMMCParser.lib
  ${clangDir}/clangSerialization.lib
  ${clangDir}/clangSema.lib
  ${clangDir}/clangEdit.lib
  ${clangDir}/clangAnalysis.lib
  ${clangDir}/LLVMBitReader.lib
  ${clangDir}/clangASTMatchers.lib
  ${clangDir}/clangAST.lib
  ${clangDir}/clangLex.lib
  ${clangDir}/clangBasic.lib
  ${clangDir}/LLVMMC.lib
  ${clangDir}/LLVMSupport.lib
  )

