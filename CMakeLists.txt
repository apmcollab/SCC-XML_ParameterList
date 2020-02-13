cmake_minimum_required (VERSION 2.6)
project(XML_ParameterList)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


file(GLOB XML_ParameterList_SRC "*.h" "*.cpp" )

ADD_LIBRARY( XML_ParameterList STATIC ${XML_ParameterList_SRC} )

# Location for static library 
set_target_properties(XML_ParameterList PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ../lib)
