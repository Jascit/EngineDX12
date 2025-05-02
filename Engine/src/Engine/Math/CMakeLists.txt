# ---
# src/Engine/Math/CMakeLists.txt
# Path: src/Engine/Math/CMakeLists.txt

file(GLOB MATH_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp
)

add_library(MathModule STATIC ${MATH_SRC})

target_include_directories(MathModule PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine/include/
)

target_link_libraries(MathModule PUBLIC CoreModule)
