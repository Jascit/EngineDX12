# ---
# src/Engine/Physics/CMakeLists.txt
# Path: src/Engine/Physics/CMakeLists.txt

file(GLOB PHYSICS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

if(PhysicsModule)
    add_library(PhysicsModule STATIC ${PHYSICS_SRC})
    target_include_directories(PhysicsModule PUBLIC
        ${CMAKE_SOURCE_DIR}Engine/include/
    )
    target_link_libraries(PhysicsModule PUBLIC CoreModule MathModule)
endif()

