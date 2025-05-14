target("Shader")
    set_kind("phony")
    after_build(function (target)
    if os.exists("$(builddir)/$(plat)/$(arch)/$(mode)/Shader") then
        os.rm("$(builddir)/$(plat)/$(arch)/$(mode)/Shader")
        print("Delete Shader files in build directory")
    end
        os.cp("Shader/**", "$(builddir)/$(plat)/$(arch)/$(mode)/Shader")
        print("Copy Shader files to build directory")
    end)