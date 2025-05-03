# CMakeLists.txt for the Engine subproject (located in C:/CPP/Engine/Engine)
cmake_minimum_required(VERSION 3.15)
project(EngineProject LANGUAGES CXX)

# Set the C++ standard and enforce its use
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENGINE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Engine/src/include")

# Define a preprocessor macro for the project root directory
add_definitions(-DPROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Add subdirectories for the engine modules. Note the use of absolute paths using CMAKE_SOURCE_DIR,
# which refers to the root directory (C:/CPP/Engine).
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/src/Engine/Core)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/src/Engine/Graphics)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/src/Engine/Math)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/src/Engine/Input)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/src/Engine/Physics)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/src/Engine/Utils)
# add_subdirectory(src/External)  # Uncomment if you have external dependencies

# Create the executable target.
# The file Engine/src/Engine.cpp should contain your main() or equivalent code.
add_executable(Engine WIN32
    src/Engine.cpp  
)

# Link Windows SDK libraries if needed
target_link_libraries(Engine d3d12 dxgi dxguid d3dcompiler)

# Link the static library modules (example for a few modules)
target_link_libraries(Engine
    CoreModule
    GraphicsModule
    MathModule
    # InputModule
    # PhysicsModule
    # UtilsModule
)

#If tests are needed, they can be added separately:
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/Tests)
