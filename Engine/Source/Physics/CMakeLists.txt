# --- (located in X:/.../Engine/Engine/Source/EngineJascit/Physics) ---

# CMakeLists.txt for the Physic module
file(GLOB_RECURSE PHYSIC_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
if(MATH_SRC)
    add_library(PhysicModule STATIC ${PHYSIC_SRC})
    
    target_include_directories(PhysicModule PUBLIC
        ${ENGINE_INCLUDE_DIR} 
    )
    
    list(APPEND DynamicModules PhysicModule)
    
    set(DynamicModules ${DynamicModules} PARENT_SCOPE)
endif()
