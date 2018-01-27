include_directories(${PROJECT_SOURCE_DIR}/code/common/)
include_directories(${PROJECT_SOURCE_DIR}/code/common/net)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(client.exe main.cpp)
target_link_libraries(client.exe net)