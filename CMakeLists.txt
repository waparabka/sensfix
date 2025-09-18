cmake_minimum_required (VERSION 4.0)

project (sensfix)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include(FetchContent)
FetchContent_Declare(rakhook GIT_REPOSITORY https://github.com/waparabka/rakhook.git)
FetchContent_MakeAvailable(rakhook)

add_library(${PROJECT_NAME} SHARED)

target_link_libraries(${PROJECT_NAME} PUBLIC rakhook)
target_sources(${PROJECT_NAME} PRIVATE source/plugin.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} SUFFIX ".asi")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)