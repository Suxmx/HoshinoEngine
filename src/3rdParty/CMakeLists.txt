# 3rdparty 库配置
add_library(3rdParty INTERFACE)
# GLFW
add_subdirectory(./glfw glfw_binary EXCLUDE_FROM_ALL)
target_link_libraries(3rdParty INTERFACE glfw)

# glm
add_subdirectory(./glm)
add_library(glm:glm ALIAS glm)
target_link_libraries(3rdParty INTERFACE glm)

# ImGui
add_subdirectory(imgui)
target_link_libraries(3rdParty INTERFACE imgui)
# spdlog
add_subdirectory(spdlog)
target_link_libraries(3rdParty INTERFACE spdlog::spdlog)

# nvrhi
add_subdirectory(nvrhi)
target_link_libraries(3rdParty INTERFACE nvrhi_vk nvrhi)
