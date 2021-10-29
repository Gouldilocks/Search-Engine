cmake_minimum_required(VERSION 3.17)
project(flyingBeans)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS_DEBUG -O3)
#set(CMAKE_CXX_FLAGS_RELEASE "-O3")
configure_file(stopWords.txt stopWords.txt)
add_executable(flyingBeans main.cpp avlNode.h avlTests.cpp avlTree.h catch.hpp Document.h DocumentParser.cpp DocumentParser.h DSPair.h IndexHandler.cpp IndexHandler.h QueryEngine.h QueryEngine.cpp UserInterface.h UserInterface.cpp porter2_stemmer.cpp porter2_stemmer.h hashy.h hashyTests.cpp Document.cpp)