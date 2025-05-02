# --- src/Engine/Core/CMakeLists.txt ---

# Збираємо всі .cpp файли в поточній директорії та підкаталогах
file(GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Створюємо статичну бібліотеку
add_library(CoreModule STATIC ${CORE_SRC})

# Вказуємо шлях до заголовочних файлів
target_include_directories(CoreModule PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine/include
)