cmake_minimum_required(VERSION 3.17)
project(SE4)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/component)
include_directories(include/entity)
include_directories(include/input)
include_directories(include/updater)
include_directories(include/world)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCE_FILE src/*.c*)
file(GLOB_RECURSE HEADER_FILE include/*.h*)

message("${SOURCE_FILE}")
message("${HEADER_FILE}")

add_library(${PROJECT_NAME} ${HEADER_FILE} ${SOURCE_FILE})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

add_executable(${PROJECT_NAME}_test test/main.cpp ${HEADER_FILE} ${SOURCE_FILE})
target_link_libraries(${PROJECT_NAME}_test ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

if(WIN32)
    # SDL2
    foreach(DLL ${SDL2_DLLS})
        file(COPY ${DLL} DESTINATION ${CMAKE_BINARY_DIR})
    endforeach()
    foreach(LIB ${SDL2_LIBRARIES})
        file(COPY ${LIB} DESTINATION ${CMAKE_BINARY_DIR})
    endforeach()
    # SDL2 Image
    foreach(DLL ${SDL2_IMAGE_DLLS})
        file(COPY ${DLL} DESTINATION ${CMAKE_BINARY_DIR})
    endforeach()
    foreach(LIB ${SDL2_IMAGE_LIBRARIES})
        file(COPY ${LIB} DESTINATION ${CMAKE_BINARY_DIR})
    endforeach()
endif()