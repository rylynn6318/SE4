# TODO : 하드코딩된 변수들 줄이는 방향으로 수정

cmake_minimum_required(VERSION 3.17)
project(SE4)
set(ENGINE_NAME "${PROJECT_NAME}_ENGINE")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

set(EXTERN_DIR ${PROJECT_SOURCE_DIR}/extern)

# Only for Windows
set(SDL2_VERSION "2.0.12")
set(SDL2_IMAGE_VERSION "2.0.5")

#------------------- ADD_LIBRARY --------------

# Add all cpp source files under the src directory
file(GLOB SOURCES "src/*.cpp")
# se4 compile to lib
add_library(${PROJECT_NAME} ${SOURCES})
# Add all headers files under the include directory
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Add compiler errors/warnings flags
#target_compile_options(${PROJECT_NAME} PRIVATE $<$<C_COMPILER_ID:MSVC>:/W4 /WX>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)

#------------------- FIND LIBS ----------------

## find GLFW as submodule
#set(GLFW_DIR "${EXTERN_DIR}/glfw")
#set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
#set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
#set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
#set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
#add_subdirectory("${GLFW_DIR}")
## target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
#
## find glad
#set(GLAD_DIR "${EXTERN_DIR}/glad")
#add_library("glad" "${GLAD_DIR}/src/glad.c")
#target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")

# find SDL2
find_package(SDL2 REQUIRED COMPONENTS main)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS})

# find SDL2_image
# TODO : SDL2_IMAGE_INCLUDE_DIR? SDL2_IMAGE_INCLUDE_DIRS?
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
#
## etc
include_directories(${CMAKE_BINARY_DIR})

#------------------- LINK ------------------

## link glfw
#target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
#target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
#
## link glad
#target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
#target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

# link SDL2
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        SDL2
        ${SDL2_LIBS}
        )


# link SDL2_image
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        SDL2_image
        ${SDL2_IMAGE_LIBS}
        )

# Add SDL2 library
#find_package(SDL2 REQUIRED)
#target_link_libraries(${PROJECT_NAME} SDL2::Core)
#
## Add SDL2_image library
#find_package(SDL2_image REQUIRED)
#target_link_libraries(${PROJECT_NAME} SDL2::Image)

# Copy SDL2 DLLs to output folder on Windows
# TODO : fix it!
# TODO : Copy SDL2 libs and dlls to SE4\out\build\x64-Debug(기본값)  if Windows
if(WIN32)
    foreach(DLL ${SDL2_DLLS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
                ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endforeach()
endif()
# Copy assets
# file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

#--------------- Test execute --------------------
add_executable(SE4_test test/main.cpp)
target_link_libraries(SE4_test PUBLIC ${PROJECT_NAME})

# for debug
message(STATUS "SDL2_INCLUDE_DIRS : ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2main_INCLUDE_DIRS : ${SDL2main_INCLUDE_DIRS}")
message(STATUS "CMAKE_BINARY_DIR : ${CMAKE_BINARY_DIR}")
message(STATUS "SDL2_LIBS : ${SDL2_LIBS}")
message(STATUS "TARGET_FILE_DIR :${TARGET_FILE_DIR}")