cmake_minimum_required(VERSION 3.10)
project(GameServer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/Activity)
include_directories(${CMAKE_SOURCE_DIR}/include/Answer)
include_directories(${CMAKE_SOURCE_DIR}/include/Common)
include_directories(${CMAKE_SOURCE_DIR}/include/Config)
include_directories(${CMAKE_SOURCE_DIR}/include/Character)
include_directories(${CMAKE_SOURCE_DIR}/include/DB)
include_directories(${CMAKE_SOURCE_DIR}/include/Game)
include_directories(${CMAKE_SOURCE_DIR}/include/GameService)
include_directories(${CMAKE_SOURCE_DIR}/include/Map)
include_directories(${CMAKE_SOURCE_DIR}/include/Mem)
include_directories(${CMAKE_SOURCE_DIR}/include/Network)
include_directories(${CMAKE_SOURCE_DIR}/include/Database)
include_directories(${CMAKE_SOURCE_DIR}/include/Utility)
include_directories(${CMAKE_SOURCE_DIR}/include/Other)

# 收集所有源文件
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# 创建可执行文件
add_executable(gameserver ${SOURCES})

# 链接库（根据需要添加）
target_link_libraries(gameserver 
    pthread
    dl
    # mysqlclient
    # curl
)

# 安装规则
install(TARGETS gameserver DESTINATION bin)
