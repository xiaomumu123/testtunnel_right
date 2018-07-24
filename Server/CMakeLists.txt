#cmake最小需要版本
cmake_minimum_required(VERSION 2.8)

#项目名字
project(testServer)

#ADD_SUBDIRECTORY(src)
#包含原程序,即把给定目录下的源程序复制给变量DIR_SRC
aux_source_directory(. DIR_SRC)

include_directories(./include/)
#生成程序
add_executable(testServer ${DIR_SRC})

