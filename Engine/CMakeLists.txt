# ---
# Engine-level CMakeLists.txt (C:\CPP\Engine\Engine\CMakeLists.txt)
cmake_minimum_required(VERSION 3.15)
project(EngineProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Module subdirectories
add_subdirectory(src/Engine/Core)
add_subdirectory(src/Engine/Graphics)
add_subdirectory(src/Engine/Math)
add_subdirectory(src/Engine/Input)
add_subdirectory(src/Engine/Physics)
add_subdirectory(src/Engine/Utils)

# External (placeholder)
# add_subdirectory(src/External)


# Executable target
add_executable(Engine WIN32
    src/Engine.cpp  
)
target_link_libraries(Engine d3d12 dxgi dxguid d3dcompiler)

target_link_libraries(Engine
    CoreModule
    GraphicsModule
    MathModule
    #InputModule
    #PhysicsModule
    #UtilsModule
)

# Tests
#if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Tests/CMakeLists.txt")
#    add_subdirectory(Tests)
#endif()
