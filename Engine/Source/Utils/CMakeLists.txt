# --- (located in X:/.../Engine/Engine/Source/EngineJascit/Physics) ---

# CMakeLists.txt for the Utils module
file(GLOB_RECURSE UTILS_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
if(MATH_SRC)
    add_library(UtilsModule STATIC ${UTILS_SRC})
    
    target_include_directories(UtilsModule PUBLIC
        ${ENGINE_INCLUDE_DIR} 
    )
    
    list(APPEND DynamicModules UtilsModule)
    
    set(DynamicModules ${DynamicModules} PARENT_SCOPE)
endif()
