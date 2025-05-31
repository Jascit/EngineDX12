# --- (located in X:/.../Engine/Engine/Source/EngineJascit/Graphics) ---

# CMakeLists.txt for the Graphics module
file(GLOB_RECURSE GRAPHICS_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
if(GRAPHICS_SRC)
    add_library(GraphicsModule STATIC ${GRAPHICS_SRC})
    
    target_include_directories(GraphicsModule PUBLIC
        ${ENGINE_INCLUDE_DIR}  
    )
    
    list(APPEND DynamicModules GraphicsModule)
    
    set(DynamicModules ${DynamicModules} PARENT_SCOPE)
endif()
