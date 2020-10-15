cmake_minimum_required(VERSION 3.17)
project(SE42)

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


add_executable(SE42
        include/component/Component.hpp
        include/component/ComponentHandle.hpp
        include/component/ComponentManager.hpp
        include/component/ComponentMask.hpp
        include/component/iComponent.h
        include/entity/Entity.hpp
        include/entity/EntityHandle.hpp
        include/entity/EntityManager.hpp
        include/entity/EntityMap.hpp
        include/input/InputEventType.h
        include/input/InputManager.h
        include/input/InputState.h
        include/input/Key.h
        include/input/KeyAction.h
        include/updater/Updater.hpp
        include/world/World.hpp
        include/library.h
        include/se4.hpp
        src/component/Component.cpp
        src/component/ComponentMask.cpp
        src/component/InputComponent.cpp
        src/entity/EntityManager.cpp
        src/input/BaseInput.cpp
        src/input/InputManager.cpp
        src/updater/BadeUpdater.cpp
        src/updater/Updater.cpp
        src/world/World.cpp
        src/library.cpp
        test/main.cpp)


target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

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