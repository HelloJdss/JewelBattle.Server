# 在DBTool中链接Mysql库=============
set(INC_DIR ../3lib/Mysql/include)
set(LINK_DIR ../3lib/Mysql/lib)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})

link_libraries(mysql)

aux_source_directory(. dir)
add_library(dbtool ${dir} ${PROJECT_SOURCE_DIR}/code/common)