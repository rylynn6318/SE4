cmake_minimum_required(VERSION 3.17)
project(
        SE4             # 프로젝트 이름
        VERSION 0.0.1   # 프로젝트 버전
        LANGUAGES CXX   # 프로젝트 언어
)

set(CMAKE_CXX_STANDARD 20)

# ------------------------------ 프로젝트 파일들 설정 ------------------------------

# 새 디렉토리 추가시 여기도 추가할 것
include_directories(include)
include_directories(include/component)
include_directories(include/entity)
include_directories(include/input)
include_directories(include/updater)
include_directories(include/world)

# CAUTION : 일단 편의를 위해 GLOB_RECURSE 사용함
file(GLOB_RECURSE SOURCE_FILE src/*.c*)
file(GLOB_RECURSE HEADER_FILE include/*.h*)

# 디버깅용
message("--> SOUᅟRCE FILES : ${SOURCE_FILE}")
message("--> HEADER FILES : ${HEADER_FILE}")

# ---------------------------------- 빌드 설정 -----------------------------------
# STATIC 라이브러리로 뱉어낸다
add_library(${PROJECT_NAME} STATIC ${HEADER_FILE} ${SOURCE_FILE})
# test/ 경로에 있는 파일들 추가해야함. 여긴 일단 GLOB_RECURSE 안씀.
# TODO : 추후에 위에서 뱉어낸 엔진 쓰는걸로 변경
add_executable(${PROJECT_NAME}_test test/main.cpp ${HEADER_FILE} ${SOURCE_FILE})
# 테스트 실행용 리소스 파일 복사
file(COPY "test/resource" DESTINATION ${CMAKE_BINARY_DIR})

# ----------------------------- 외부 라이브러리 추가--------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

# SDL2와 똘마니들
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

# glog
set(GLOG_DIR "extern/glog")
add_subdirectory("${GLOG_DIR}")
include_directories("${GLOG_DIR}/include")

# box2d 로드해서 캐시에 저장되기 전에 옵션 기본값을 off로 설정함
option(BOX2D_BUILD_UNIT_TESTS "Build the Box2D unit tests" OFF)
option(BOX2D_BUILD_TESTBED "Build the Box2D testbed" OFF)
option(BOX2D_BUILD_DOCS "Build the Box2D documentation" OFF)
option(BOX2D_USER_SETTINGS "Override Box2D settings with b2UserSettings.h" OFF)
# box2d
set(BOX2D_DIR "extern/box2d")
add_subdirectory("${BOX2D_DIR}")
include_directories("${BOX2D_DIR}/include")
message("--> BOX2D_BUILD_TESTBED : ${BOX2D_BUILD_TESTBED}")

# ------------------------------------ 링크 -------------------------------------
# 추후에 glog 정도만 PUBLIC 으로 하고 나머진 PRIVATE 으로 의존성 설정
target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        glog
        box2d
        )
# TODO : 테스트용 또한 최종적으로는 엔진만 링크하게 변경
target_link_libraries(${PROJECT_NAME}_test
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        glog
        box2d
        )

# -------------------------------- 윈도우용 셋팅 ----------------------------------
# 윈도우는 빌드하고 그 위치에 컴파일 하지 않은 SDL 라이브러리 넣어줘야 함
if (WIN32)
    # SDL2
    foreach (DLL ${SDL2_DLLS})
        file(COPY ${DLL} DESTINATION ${CMAKE_BINARY_DIR})
    endforeach ()
    foreach (LIB ${SDL2_LIBRARIES})
        file(COPY ${LIB} DESTINATION ${CMAKE_BINARY_DIR})
    endforeach ()
    # SDL2 Image
    foreach (DLL ${SDL2_IMAGE_DLLS})
        file(COPY ${DLL} DESTINATION ${CMAKE_BINARY_DIR})
    endforeach ()
    foreach (LIB ${SDL2_IMAGE_LIBRARIES})
        file(COPY ${LIB} DESTINATION ${CMAKE_BINARY_DIR})
    endforeach ()
endif ()

