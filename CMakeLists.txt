# CMakeList.txt: CEvent 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("CEvent")

# 将源代码添加到此项目的可执行文件。
add_executable (CEvent  "main.cpp"  "PObject.h" "PObject.cpp" "EventLoop.cpp"   "debug.h" "PThread.h" "PThread.cpp" "PTimer.h" "PTimer.cpp" "CAS.h" "PTCPSocket.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CEvent PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
