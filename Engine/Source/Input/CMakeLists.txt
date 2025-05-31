# --- (located in X:/.../Engine/Engine/Source/EngineJascit/Input) ---

# CMakeLists.txt for the Input module
file(GLOB_RECURSE INPUT_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
if(INPUT_SRC)
    add_library(InputModule STATIC ${INPUT_SRC})
    
    target_include_directories(InputModule PUBLIC
        ${ENGINE_INCLUDE_DIR} 
    )
    
    list(APPEND DynamicModules InputModule)
    
    set(DynamicModules ${DynamicModules} PARENT_SCOPE)
endif()
