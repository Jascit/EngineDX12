# ---
# src/Engine/Utils/CMakeLists.txt
# Path: src/Engine/Utils/CMakeLists.txt

file(GLOB UTILS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp
)

if(UTILS_SRC)
    add_library(UtilsModule STATIC ${UTILS_SRC})
    target_include_directories(UtilsModule PUBLIC
        ${CMAKE_SOURCE_DIR}/include/Engine/Utils
    )
    target_link_libraries(GraphicsModule PUBLIC CoreModule)
endif()