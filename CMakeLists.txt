cmake_minimum_required(VERSION 3.10.0)
project(ImageViewer)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)
find_package(PkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMAGE REQUIRED sdl2_image)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

link_directories(${SDL2_IMAGE_LIBRARY_DIRS})
link_directories(${SDL2_IMAGE_LIBRARY_DIRS})

add_executable(${CMAKE_PROJECT_NAME} ${SRC})
target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
