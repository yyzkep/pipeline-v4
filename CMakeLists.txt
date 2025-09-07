cmake_minimum_required(VERSION 3.28) #aaa
project(pipeline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

include(CPM.cmake)

CPMAddPackage("gh:ocornut/imgui@1.91.4") # imgui
CPMAddPackage("gh:cursey/safetyhook@0.6.6") # hooking
CPMAddPackage(
  NAME freetype
  GITHUB_REPOSITORY freetype/freetype
  GIT_TAG VER-2-13-2
  OPTIONS
    "CMAKE_POLICY_DEFAULT_CMP0012=NEW"
    "CMAKE_POLICY_DEFAULT_CMP0063=NEW"
    "CMAKE_POLICY_DEFAULT_CMP0077=NEW"
)


file(GLOB_RECURSE SRC_FILES # wizard type of shit right here
  app/*.cpp
  inc/*.cpp
)

add_library(pipeline SHARED ${SRC_FILES})

if(imgui_ADDED)
  target_sources(pipeline PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_dx9.cpp
  )
  
  target_include_directories(pipeline PUBLIC
    $<BUILD_INTERFACE:${imgui_SOURCE_DIR}>
    $<BUILD_INTERFACE:${imgui_SOURCE_DIR}/backends>
  )

  target_compile_definitions(pipeline PUBLIC
    "IMGUI_USER_CONFIG=\"${CMAKE_SOURCE_DIR}/inc/imconfig.hpp\""
    IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
  )
endif()

#I DONT FUCKING CARE ANYMORE IM FUCKING GOING FUCKING INSANE FUCKING FUCKING FUCKING SHIT
#I ACTUALLY FUCKING HATE DEBUGGING SHIT
#AAAAAAAAAAAAAAAAAAA

# add_definitions(-DWINDOWS_IGNORE_PACKING_MISMATCH) was caused by matrix.hpp...

target_include_directories(pipeline PRIVATE inc)
target_compile_definitions(pipeline PRIVATE NOMINMAX) # solves autism with std::max & std::min

#autism
if (MSVC)
    target_compile_options(pipeline PRIVATE /permissive-)
else()
    target_compile_options(pipeline PRIVATE -fpermissive) 
endif()


target_link_libraries(pipeline
  safetyhook::safetyhook 
  freetype
  d3d9
)
