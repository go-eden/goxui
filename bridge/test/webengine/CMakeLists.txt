# 用于测试的辅助进程
set(NAME WebEngineTest)

add_executable(${NAME} main.cpp)
target_link_libraries(${NAME} PUBLIC goxui-web)
