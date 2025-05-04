set_project("HoshinoEngine")

add_cxflags("/utf-8")
add_rules("mode.debug", "mode.releasedbg", "mode.release", "mode.minsizerel")
set_arch("x64")
-- add_cxxflags("-Wall")
set_languages("c++20")
add_requires("glfw", "glm","vulkansdk","imgui","glad","spdlog","magic_enum") 
includes("src/Hoshino","src/Client")