package("nvrhi")
    add_deps("cmake")
    add_deps("vulkansdk")
    add_defines("NVRHI_WITH_DX11","NVRHI_WITH_DX12","NVRHI_WITH_VULKAN")
    set_sourcedir(path.join(os.scriptdir(), ""))
    on_install(function (package)
        local configs = {}
        
        import("package.tools.cmake").install(package, configs)
    end) 
    on_test(function (package)
    end)
package_end()