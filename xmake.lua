set_project("HoshinoEngine")

add_cxflags("/utf-8")
add_rules("mode.debug", "mode.releasedbg", "mode.release", "mode.minsizerel","mode.profile")
set_arch("x64")
-- add_cxxflags("-Wall")
set_languages("c++20")
add_requires("glfw", "glm 1.0.1","glad","spdlog 1.15.2","assimp v5.4.3") 
add_requires("imgui v1.91.8-docking",{configs = {glfw = true,opengl3 = true}})
includes("src/Hoshino","src/Client","Res") 

add_defines("HOSHINO_STATIC_BUILD","HOSHINO_ASSERT","HOSHINO_DEBUG_LOG")