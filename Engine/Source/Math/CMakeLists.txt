# --- (located in X:/.../Engine/Engine/Source/EngineJascit/Math) ---

# CMakeLists.txt for the Math module
file(GLOB_RECURSE MATH_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
if(MATH_SRC)
    add_library(MathModule STATIC ${MATH_SRC})
    
    target_include_directories(MathModule PUBLIC
        ${ENGINE_INCLUDE_DIR} 
    )
    
    list(APPEND DynamicModules MathModule)
    
    set(DynamicModules ${DynamicModules} PARENT_SCOPE)
endif()
