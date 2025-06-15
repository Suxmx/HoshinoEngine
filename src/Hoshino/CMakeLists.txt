# 收集 HoshinoEngine 的源文件和头文件
file(GLOB_RECURSE HOSHINO_SOURCES "Private/*.cpp" "Private/*.c")
file(GLOB_RECURSE HOSHINO_HEADERS "Public/*.h" "Public/*.hpp")

# 创建引擎库
add_library(Hoshino STATIC ${HOSHINO_SOURCES} ${HOSHINO_HEADERS})
if(MSVC)
    # MSVC特定配置
    target_compile_options(Hoshino PRIVATE /W4)
endif()
# 导出宏定义（用于DLL导出）
target_compile_definitions(Hoshino PRIVATE "HOSHINO_STATIC_BUILD")
target_compile_definitions(Hoshino PRIVATE "HOSHINO_VULKAN")
# 设置包含目录
# 将 Public 目录设为 PUBLIC 包含路径，这样依赖此库的项目也会自动包含此路径
target_include_directories(Hoshino
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

# 设置预编译头文件
target_precompile_headers(Hoshino
        PUBLIC
        "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/Public/HoshinoPch.h>"
)

# 链接实际第三方库
target_link_libraries(Hoshino
        PUBLIC 3rdParty
)

# 设置属性以便在IDE中组织文件
source_group("Source Files" FILES ${HOSHINO_SOURCES})
source_group("Header Files" FILES ${HOSHINO_HEADERS})