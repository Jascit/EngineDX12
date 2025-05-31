# CMakeLists.txt for the Engine subproject (located in X:/.../TicTacToes_DX12/TicTacToes_DX12/Engine/Source)
# Add subdirectories for the engine modules. Note the use of absolute paths using 

# CMakeLists.txt for the Engine subproject (located in X:/.../Engine/Engine/Source/EngineJascit)

set(ENGINE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Engine/Include")

set(SUBDIRECTORIES
    Core
    Graphics 
    Input
    Math
    Utils 
    Physics
    External
)

set(DynamicModules)

foreach(SUBDIR ${SUBDIRECTORIES})
    set(SUBDIR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}")
    if(EXISTS "${SUBDIR_PATH}" AND IS_DIRECTORY "${SUBDIR_PATH}")
        add_subdirectory("${SUBDIR_PATH}")
    else()
        message(WARNING "Directory ${SUBDIR_PATH} does not exist.")
    endif()
endforeach()

message(STATUS "DynamicModules: ${DynamicModules}")
add_library(EngineModules INTERFACE)
target_link_libraries(EngineModules INTERFACE
    ${DynamicModules}
)