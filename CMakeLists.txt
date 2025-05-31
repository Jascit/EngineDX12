# CMakeList.txt : Top-level CMake project file, do global configuration

cmake_minimum_required(VERSION 3.15)

# Set the name of the root project
project(EngineProjectRoot)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Define a preprocessor macro for the project root directory
add_definitions(-DPROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Set the binary directory path
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/out/build")

# Set the install directory path (used by 'make install' if needed)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/out/install")

# Add necessary include directories for Windows SDK (if required)
include_directories($ENV{WindowsSdkDir}Include/$ENV{WindowsSDKVersion}/um)
include_directories($ENV{WindowsSdkDir}Include/$ENV{WindowsSDKVersion}/shared)
include_directories($ENV{WindowsSdkDir}Include/$ENV{WindowsSDKVersion}/winrt)

# Add library directory for Windows SDK libraries (64-bit)
link_directories($ENV{WindowsSdkDir}Lib/$ENV{WindowsSDKVersion}/um/x64)

# Add the subdirectory for the Engine subproject
add_subdirectory(Engine)
