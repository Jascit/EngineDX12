# --- (located in X:/.../Engine/Engine/Source/EngineJascit/Core) ---

# CMakeLists.txt for the Core module
file(GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(CoreModule STATIC ${CORE_SRC})

target_include_directories(CoreModule PUBLIC
    ${ENGINE_INCLUDE_DIR} 
)

# ��������� CoreModule �� DynamicModules
list(APPEND DynamicModules CoreModule)

# �������� DynamicModules � ����������� ��������
set(DynamicModules ${DynamicModules} PARENT_SCOPE)
