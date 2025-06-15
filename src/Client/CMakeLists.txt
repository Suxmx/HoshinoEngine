# 收集 Client 的源文件和头文件
file(GLOB_RECURSE CLIENT_SOURCES "*.cpp" "*.c")
file(GLOB_RECURSE CLIENT_HEADERS "*.h" "*.hpp")

# 创建客户端可执行程序
add_executable(Client ${CLIENT_SOURCES} ${CLIENT_HEADERS})

# 链接引擎和第三方库
target_link_libraries(Client
        PRIVATE Hoshino
)
# 设置包含目录
target_include_directories(Client
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

if(MSVC)
    # MSVC特定配置
    target_compile_options(Client PRIVATE /W4)
endif()