# ---
# Tests/CMakeLists.txt
# Path: Tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE TEST_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

if(TEST_SRC)
    add_executable(CoreTests ${TEST_SRC})
    target_include_directories(CoreTests PRIVATE
        ${CMAKE_SOURCE_DIR}/Engine/include
    )
    target_link_libraries(CoreTests PRIVATE CoreModule MathModule GraphicsModule)

    enable_testing()
    add_test(NAME CoreTests COMMAND CoreTests)
endif()
