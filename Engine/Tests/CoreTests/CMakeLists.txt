# --- src/Engine/Tests/CoreTests/CMakeLists.txt ---
# ������� �� .h ����� � �������� �������� �� �����������
file(GLOB_RECURSE CORE_TESTS_INCLUDES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h
)
# ������� �� .cpp ����� � �������� �������� �� �����������
file(GLOB_RECURSE CORE_TESTS_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# ��������� �������� ��������
add_library(CoreTests STATIC ${CORE_TESTS_SRC})

# ������� ���� �� ������������ �����
target_include_directories(CoreTests PUBLIC
    ${CMAKE_SOURCE_DIR}/Engine/include
    ${CMAKE_SOURCE_DIR}/Engine/Tests/CoreTests/Include
)

enable_testing()
target_link_libraries(CoreTests PRIVATE CoreModule MathModule GraphicsModule)

add_test(NAME CoreTests COMMAND CoreTests)