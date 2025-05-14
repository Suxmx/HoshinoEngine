set_project("HoshinoEngine")

add_cxflags("/utf-8")
add_rules("mode.debug", "mode.releasedbg", "mode.release", "mode.minsizerel")
set_arch("x64")
-- add_cxxflags("-Wall")
set_languages("c++20")
add_requires("glfw", "glm 1.0.1","glad","spdlog 1.15.2") 
add_requires("imgui v1.91.8-docking",{configs = {glfw = true,opengl3 = true}})
includes("src/Hoshino","src/Client") 

add_defines("HOSHINO_STATIC_BUILD","HOSHINO_ASSERT","HOSHINO_DEBUG_LOG")

-- 在每次运行时复制Shader到构建目录
on_run(function (target)
    local shader_src = "$(projectdir)/Shader"
    local shader_dst = "$(builddir)/$(plat)/$(arch)/$(mode)"
    os.cp(shader_src, shader_dst)
    print("Copied shader files from " .. shader_src .. " to " .. shader_dst)
end)