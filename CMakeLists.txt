cmake_minimum_required(VERSION 3.26)
project(FIleSystem)

set(CMAKE_CXX_STANDARD 17)

# 设置源文件的相对路径
set(SOURCE_FILES
        source/main.cpp
        source/FilePhysicalControl.cpp
        source/Bitmap.cpp
        source/User.cpp
        source/MainOperation.cpp
        source/head.cpp
        source/ClusteredAllocation.cpp
)

set(HEADER_FILES
        head/FilePhysicalControl.h
        head/head.h
        head/Bitmap.h
        head/User.h
        head/MainOperation.h
        head/define.h
        head/init.h
        head/ClusteredAllocation.h
)

add_executable(FIleSystem ${SOURCE_FILES} ${HEADER_FILES})

# 添加安装规则
install(TARGETS FIleSystem DESTINATION bin)
install(FILES ${HEADER_FILES} DESTINATION include/FileSystem)

# 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
