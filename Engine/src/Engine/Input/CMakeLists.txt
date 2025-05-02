# ---
# src/Engine/Input/CMakeLists.txt
# Path: src/Engine/Input/CMakeLists.txt
file(GLOB INPUT_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp
)

if(INPUT_SRC)
    add_library(InputModule STATIC ${INPUT_SRC})
    target_include_directories(InputModule PUBLIC
        ${CMAKE_SOURCE_DIR}/Engine/include/
    )
    target_link_libraries(InputModule PUBLIC CoreModule)
endif()


