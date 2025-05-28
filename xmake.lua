set_project("HoshinoEngine")

package("nvrhi")
    add_deps("cmake")
    add_deps("vulkansdk")
    add_defines("NVRHI_WITH_DX11","NVRHI_WITH_DX12","NVRHI_WITH_VULKAN")
    set_sourcedir(path.join(os.scriptdir(), "src/nvrhi"))
    on_install(function (package)
        local configs = {}
        
        import("package.tools.cmake").install(package, configs)
    end) 
    on_test(function (package)
    end)
package_end()




add_cxflags("/utf-8")
add_rules("mode.debug", "mode.releasedbg", "mode.release", "mode.minsizerel","mode.profile")
set_arch("x64")
-- add_cxxflags("-Wall")
set_languages("c++20")
add_requires("glfw", "glm 1.0.1","glad","spdlog 1.15.2","assimp v5.4.3","nvrhi","vulkansdk") 
add_requires("imgui v1.91.8-docking",{configs = {glfw = true,opengl3 = true}})
includes("src/Hoshino","src/Client","Res") 

add_defines("HOSHINO_STATIC_BUILD","HOSHINO_ASSERT","HOSHINO_DEBUG_LOG")