file(GLOB_RECURSE CLIENT_SOURCES "*.cpp" "*.c")
file(GLOB_RECURSE CLIENT_HEADERS "*.h" "*.hpp")

# Exe
add_executable(Client ${CLIENT_SOURCES} ${CLIENT_HEADERS})
# Includes
target_include_directories(Client
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Public
)
target_link_libraries(Client
        PRIVATE Hoshino
)

