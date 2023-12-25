# CMakeList.txt: CEvent 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.0)


project(CEvent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-gdwarf-2 -std=c++17)





#set(CMAKE_CXX_STANDARD 17)
link_directories(${CMAKE_SOURCE_DIR}/lib/protobuf/lib)
link_directories(${CMAKE_SOURCE_DIR}/lib/openssl/lib64)

include_directories(${CMAKE_SOURCE_DIR}/linux)
#include_directories(${CMAKE_SOURCE_DIR}/lib/include)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/utils)

include_directories(${CMAKE_SOURCE_DIR}/lib/openssl/include)
include_directories(${CMAKE_SOURCE_DIR}/proto)
include_directories(${CMAKE_SOURCE_DIR}/lib/protobuf/include)



file(GLOB SOURCE_CODE "linux/*.cpp" "*.cpp" "ssl3/*.cpp" "test/*.cpp" "proto/*.cc" )

add_executable(${PROJECT_NAME} ${SOURCE_CODE} )


# add_library(${PROJECT_NAME} SHARED ${SOURCE_CODE})

 target_link_libraries(${PROJECT_NAME} libssl.a)
 target_link_libraries(${PROJECT_NAME} libcrypto.a)
 target_link_libraries(${PROJECT_NAME} libprotobuf.a)

 #target_link_libraries(${PROJECT_NAME} libws2_32.lib)

# TOD: 如有需要，请添加测试并安装
