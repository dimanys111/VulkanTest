cmake_minimum_required(VERSION 3.16)

# create sample app project
project(vulkan_test LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VKB_WSI_SELECTION "XCB" CACHE STRING "Select WSI target (XCB, XLIB, WAYLAND, D2D)")

# Use FindVulkan module added with CMAKE 3.7
if (NOT CMAKE_VERSION VERSION_LESS 3.7.0)
        message(STATUS "Using module to find Vulkan")
        find_package(Vulkan)
endif()

# Add third party libraries
add_subdirectory(../../Vulkan-Samples/third_party third_party)

file(GLOB_RECURSE FILES_SRC CONFIGURE_DEPENDS src/*.*)

add_executable(${PROJECT_NAME} WIN32 ${FILES_SRC})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm stb imgui ${Vulkan_LIBRARY})

target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)

set(SHADER_DIR_GLSL "shaders")
file(GLOB SHADERS_GLSL CONFIGURE_DEPENDS "${SHADER_DIR_GLSL}/*.*")
source_group("shaders" FILES ${SHADERS_GLSL})

add_custom_target(shaders SOURCES ${SHADERS_GLSL})
