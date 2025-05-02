
# ---
# src/Engine/Graphics/CMakeLists.txt
# Path: src/Engine/Graphics/CMakeLists.txt

file(GLOB GRAPHICS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp
)

add_library(GraphicsModule STATIC ${GRAPHICS_SRC})

target_include_directories(GraphicsModule PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine/include/
)

target_link_libraries(GraphicsModule PUBLIC CoreModule)