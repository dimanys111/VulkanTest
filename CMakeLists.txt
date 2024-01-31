cmake_minimum_required(VERSION 3.16)

# create sample app project
project(vulkan_test LANGUAGES C CXX)

# Add third party libraries
add_subdirectory(../test_vulkan_hpp_my/third_party build)

file(GLOB_RECURSE FILES_SRC src/*.*)

add_executable(${PROJECT_NAME} WIN32 ${FILES_SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm stb volk)
