# 在Config中链接lua53库==========
set(INC_DIR ../3lib/lua53/include)
set(LINK_DIR ../3lib/lua53/lib)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})

link_libraries(lua53)
# 在Config中链接lua53库==========
# target_link_libraries(name lua53)  # 记得在工程的add_executable()后加上这句


aux_source_directory(. dir)
add_library(config ${dir} ${PROJECT_SOURCE_DIR}/code/common)