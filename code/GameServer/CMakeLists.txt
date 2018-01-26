include_directories(${PROJECT_SOURCE_DIR}/code/common/)
include_directories(${PROJECT_SOURCE_DIR}/code/common/net)

# 可执行文件对应
add_executable(GameServer.exe main.cpp)

target_link_libraries(GameServer.exe net)