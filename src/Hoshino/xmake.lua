target("HoshinoEngine")
    set_kind("static")
    add_files("**.cpp")
    add_includedirs("Public", {public = true})
    add_packages("glfw", "glm", "vulkansdk", "imgui", "glad", "spdlog","assimp","nvrhi")
    add_defines("HOSHINO_EXPORTS") -- HoshinoEngine
    set_pcxxheader("Public/HoshinoPch.h")
