cmake_minimum_required(VERSION 3.16)

# create sample app project
project(vulkan_test LANGUAGES C CXX)

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

file(GLOB_RECURSE FILES_SRC src/*.*)

add_executable(${PROJECT_NAME} WIN32 ${FILES_SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm stb volk)
