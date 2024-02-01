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

# Add third party libraries
add_subdirectory(../test_vulkan_hpp_my/third_party third_party)

file(GLOB FILES_SRC CONFIGURE_DEPENDS src/imgui/*.cpp src/obj_loader/*.cpp src/imgui/*.h src/obj_loader/*.h)

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

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm stb volk)

target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)

set(SHADER_DIR_GLSL "shaders")
file(GLOB SHADERS_GLSL CONFIGURE_DEPENDS "${SHADER_DIR_GLSL}/*.*")
source_group("shaders" FILES ${SHADERS_GLSL})

add_custom_target(shaders SOURCES ${SHADERS_GLSL})
