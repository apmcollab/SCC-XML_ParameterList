cmake_minimum_required (VERSION 3.15)
project (XML_ParameterList)

file(GLOB Library_Sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp" ) 

add_library (XML_ParameterList STATIC ${Library_Sources})

target_compile_features    (XML_ParameterList PUBLIC cxx_std_17)
target_include_directories (XML_ParameterList PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


