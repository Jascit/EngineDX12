# CMakeLists.txt for the Engine subproject (located in C:/CPP/Engine/Engine)
cmake_minimum_required(VERSION 3.15)
project(EngineProject LANGUAGES CXX)
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# Add source to this project's executable.
add_executable (EngineJascit WIN32 
	${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source)

target_link_libraries(EngineJascit
    EngineModules
)

# Link Windows SDK libraries if needed
target_link_libraries(EngineJascit d3d12 dxgi dxguid d3dcompiler)


#If tests are needed, they can be added separately:
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Tests)
