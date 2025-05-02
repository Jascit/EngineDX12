# Top-level CMakeLists.txt (C:\CPP\Engine\CMakeLists.txt)
cmake_minimum_required(VERSION 3.15)
project(EngineProjectRoot)

include_directories($ENV{WindowsSdkDir}Include/$ENV{WindowsSDKVersion}/um)
include_directories($ENV{WindowsSdkDir}Include/$ENV{WindowsSDKVersion}/shared)
include_directories($ENV{WindowsSdkDir}Include/$ENV{WindowsSDKVersion}/winrt)

link_directories($ENV{WindowsSdkDir}Lib/$ENV{WindowsSDKVersion}/um/x64)

add_subdirectory(Engine)