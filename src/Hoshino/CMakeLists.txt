file(GLOB_RECURSE HOSHINO_SOURCES "Private/*.cpp" "Private/*.c")
file(GLOB_RECURSE HOSHINO_HEADERS "Public/*.h" "Public/*.hpp")

# Library
add_library(Hoshino STATIC ${HOSHINO_SOURCES} ${HOSHINO_HEADERS})
# Compiler Spec
if(MSVC)
    target_compile_options(Hoshino PRIVATE /W4)
endif()

# Definitions
target_compile_definitions(Hoshino PRIVATE "HOSHINO_STATIC_BUILD")
target_compile_definitions(Hoshino PRIVATE "HOSHINO_VULKAN")
# Include & Pch
target_precompile_headers(Hoshino
        PUBLIC
        "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/Public/HoshinoPch.h>"
)
target_include_directories(Hoshino
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public
)
target_link_libraries(Hoshino
        PUBLIC 3rdParty
)