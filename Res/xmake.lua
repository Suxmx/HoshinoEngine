target("Res")
    set_kind("phony")
    after_build(function (target)
    if os.exists("$(builddir)/$(plat)/$(arch)/$(mode)/Res") then
        os.rm("$(builddir)/$(plat)/$(arch)/$(mode)/Res")
        print("Delete Res files in build directory")
    end
        os.cp("Res", "$(builddir)/$(plat)/$(arch)/$(mode)/Res")
        print("Copy Res files to build directory")
        if os.exists("$(builddir)/$(plat)/$(arch)/$(mode)/Res/xmake.lua") then 
            os.rm("$(builddir)/$(plat)/$(arch)/$(mode)/Res/xmake.lua")
        end
    end)