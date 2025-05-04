target("HoshinoEngine")
    set_kind("shared")
    add_files("**.cpp")
    add_includedirs("Public", {public = true})
    add_packages("glfw", "glm", "vulkansdk", "imgui", "glad", "spdlog", "magic_enum")
    add_defines("HOSHINO_EXPORTS", "HOSHINO_ASSERT", "MAGIC_ENUM")
    if is_os("windows") then
        add_defines("GLFW_INCLUDE_NONE")  -- 仅Windows添加此定义
    end
    
    set_pcxxheader("Public/Core/HoshinoPch.h")
