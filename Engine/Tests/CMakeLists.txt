# CMakeLists.txt for the Tests subprojects (located in C:/CPP/Engine/Engine/Tests)
cmake_minimum_required(VERSION 3.15)

set(TESTS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Engine/src/include")

add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/Tests/CoreTests)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/Tests/GraphicsTests)
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/Tests/PhysicsTests)
#add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/Tests/MathTests)
#add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/Tests/InputTests)
#add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/Tests/UtilsTests)

add_executable(Tests #WIN32
    CoreTests/test.cpp  
    # GraphicsTests/test.cpp  
    # PhysicsTests/test.cpp  
)
target_link_libraries(Tests d3d12 dxgi dxguid d3dcompiler)

target_link_libraries(Tests
    CoreTests
    GraphicsModule
    MathModule
    # InputModule
    # PhysicsModule
    # UtilsModule
)
# Optionally, if the executable needs to link with the test libraries, link here:
# target_link_libraries(Tests PRIVATE CoreTests GraphicsTests PhysicsTests)
