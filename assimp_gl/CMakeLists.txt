cmake_minimum_required(VERSION 3.10)

project(assimp_gl)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp)
        
# Add header files
file(GLOB_RECURSE HEADER_FILES 
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp)

configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.h)
include_directories(${CMAKE_BINARY_DIR}/src)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

add_library(STB_IMAGE "thirdparty/stb_image.cpp")
add_library(GLAD "thirdparty/glad.c")

include_directories(${GLFW_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS}
                    ${OPENGL_INCLUDE_DIR}
                    ${ASSIMP_INCLUDE_DIR}
                    "${CMAKE_SOURCE_DIR}/src"
                    "${CMAKE_SOURCE_DIR}/include"
            )
                
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(${PROJECT_NAME}  ${HEADER_FILES} 
                                ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} 
                      ${GLFW_LIBRARIES} 
                      ${GLEW_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      assimp
                      STB_IMAGE
                      GLAD
                      dl
                      pthread)
