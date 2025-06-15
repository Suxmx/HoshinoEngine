# 3rdparty 库配置
add_library(3rdParty INTERFACE)
target_include_directories(3rdParty INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# GLFW
set(GLFW_DIR ./glfw)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF) ##GLFW会根据这些OPTION选择编译那些module
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

add_subdirectory(${GLFW_DIR} glfw_binary EXCLUDE_FROM_ALL)
# 将GLFW添加到3rdParty接口库
target_link_libraries(3rdParty INTERFACE glfw)
#glm
add_subdirectory(./glm)
## 创建一个库并生成别名
add_library(glm:glm ALIAS glm)
target_link_libraries(3rdParty INTERFACE glm)

# ImGui
add_subdirectory(imgui)
# 将ImGui添加到3rdParty接口库
target_link_libraries(3rdParty INTERFACE imgui)
# spdlog
add_subdirectory(spdlog)
target_link_libraries(3rdParty INTERFACE spdlog::spdlog)

# nvrhi
add_subdirectory(nvrhi)
target_link_libraries(3rdParty INTERFACE nvrhi_vk nvrhi)
