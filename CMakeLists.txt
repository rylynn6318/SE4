cmake_minimum_required(VERSION 3.17)
project(
        SE4             # 프로젝트 이름
        VERSION 0.0.1   # 프로젝트 버전
        LANGUAGES CXX   # 프로젝트 언어
)

set(CMAKE_CXX_STANDARD 20)

# 새 디렉토리 추가시 여기도 추가할 것
include_directories(include)
include_directories(include/component)
include_directories(include/entity)
include_directories(include/input)
include_directories(include/updater)
include_directories(include/world)

# 외부 라이브러리 추가
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

# CAUTION : 일단 편의를 위해 GLOB_RECURSE 사용함
file(GLOB_RECURSE SOURCE_FILE src/*.c*)
file(GLOB_RECURSE HEADER_FILE include/*.h*)

# 디버깅용
message("--> SOUᅟRCE FILES : ${SOURCE_FILE}")
message("--> HEADER FILES : ${HEADER_FILE}")

# STATIC 라이브러리로 뱉어낸
add_library(${PROJECT_NAME} STATIC ${HEADER_FILE} ${SOURCE_FILE})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# TODO : 에셋/리소스 파일 복사해야 함
# test/ 경로에 있는 파일들 추가해야함. 여긴 일단 GLOB_RECURSE 안씀
add_executable(${PROJECT_NAME}_test test/main.cpp ${HEADER_FILE} ${SOURCE_FILE})
target_link_libraries(${PROJECT_NAME}_test ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
file(COPY "test/resource" DESTINATION ${CMAKE_BINARY_DIR})

# 윈도우는 빌드하고 그 위치에 외부 라이브러리 넣어줘야 함
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

