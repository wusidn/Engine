CMAKE_MINIMUM_REQUIRED (VERSION 3.4)

PROJECT (tests)


#指定头文件路径
include_directories(${PROJECT_ROOT_DIR}/src)
include_directories(${PROJECT_ROOT_DIR}/deps)

ADD_EXECUTABLE(demo demo/main.cpp )

#链接库
TARGET_LINK_LIBRARIES(demo engine)

#输出路径
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_DIR}/build/bin)


#设置编译选项
if(WIN32)
    SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -Wall")
    SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -Wall")
elseif(UNIX)
    SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O0 -Wall -g")
    SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -O3 -Wall")
endif()