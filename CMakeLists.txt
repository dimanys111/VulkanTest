cmake_minimum_required(VERSION 3.16)

# create sample app project
project(vulkan_test LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add path for local cmake scripts
list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/bldsys/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/bldsys/cmake/module)

include(utils)
include(global_options)
include(sample_helper)
include(check_atomic)
include(component_helper)

# Use FindVulkan module added with CMAKE 3.7
if (NOT CMAKE_VERSION VERSION_LESS 3.7.0)
        message(STATUS "Using module to find Vulkan")
        find_package(Vulkan)
endif()

# Add third party libraries
add_subdirectory(../Vulkan-Samples/third_party third_party)

file(GLOB FILES_SRC CONFIGURE_DEPENDS src/obj_loader/*.*)

set(FILES_SRC_MY 
    src/main.cpp

    src/camera.cpp
    src/camera.h
    src/device.cpp
    src/device.h
    src/dirLight.cpp
    src/dirLight.h
    src/engine.cpp
    src/engine.h
    src/entity.cpp
    src/entity.h
    src/game.cpp
    src/game.h
    src/gameObject.cpp
    src/gameObject.h
    src/graphics.cpp
    src/graphics.h
    src/model.cpp
    src/model.h
    src/pipeline.cpp
    src/pipeline.h
    src/renderer.cpp
    src/renderer.h
    src/skyBox.cpp
    src/skyBox.h
    src/swapchain.cpp
    src/swapchain.h
    src/tools.cpp
    src/tools.h
    src/window.cpp
    src/window.h

    src/stdinclude.h
)

add_executable(${PROJECT_NAME} WIN32 ${FILES_SRC} ${FILES_SRC_MY})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm stb imgui ${Vulkan_LIBRARY})

target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)

set(SHADER_DIR_GLSL "shaders")
file(GLOB SHADERS_GLSL CONFIGURE_DEPENDS "${SHADER_DIR_GLSL}/*.*")
source_group("shaders" FILES ${SHADERS_GLSL})

add_custom_target(shaders SOURCES ${SHADERS_GLSL})
