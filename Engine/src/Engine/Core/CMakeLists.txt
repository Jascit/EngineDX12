# --- src/Engine/Core/CMakeLists.txt ---

# ������� �� .cpp ����� � �������� �������� �� �����������
file(GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# ��������� �������� ��������
add_library(CoreModule STATIC ${CORE_SRC})

# ������� ���� �� ������������ �����
target_include_directories(CoreModule PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine/include
)