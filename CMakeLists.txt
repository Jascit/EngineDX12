# Top-level CMakeLists.txt for the root project
cmake_minimum_required(VERSION 3.15)

# Set the name of the root project
project(EngineProjectRoot)

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
