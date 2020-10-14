#------------------------------------------------------------------------------
# Usage: find_package(SDL2 [REQUIRED])
#
# Sets variables:
#     SDL2_IMAGE_INCLUDE_DIR
#     SDL2_IMAGE_LIB
#     SDL2_IMAGE_DLLS
#------------------------------------------------------------------------------

include(FindPackageHandleStandardArgs)

if(WIN32)
    find_path(SDL2_IMAGE_ROOT "include/SDL_image.h" PATHS "${EXTERN_DIR}/SDL2_image-${SDL2_IMAGE_VERSION}-dev" NO_DEFAULT_PATH)
    if(SDL2_IMAGE_ROOT)
        if (EXISTS "${SDL2_IMAGE_ROOT}/build/Debug/SDL2_image.lib")
            set(SDL2_IMAGE_INCLUDE_DIR "${SDL2_IMAGE_ROOT}/include")
            set(SDL2_IMAGE_LIB "${SDL2_IMAGE_ROOT}/build/Debug/SDL2_image.lib")
            set(SDL2_IMAGE_DLLS "${SDL2_IMAGE_ROOT}/build/Debug/SDL2_image.dll")
        endif()
    endif()
    if(NOT SDL2_IMAGE_FOUND)
        find_path(SDL2_IMAGE_ROOT "include/SDL_image.h" PATHS "${EXTERN_DIR}/SDL2_image-${SDL2_IMAGE_VERSION}" NO_DEFAULT_PATH)
        if(SDL2_IMAGE_ROOT)
            set(SDL2_IMAGE_INCLUDE_DIR "${SDL2_IMAGE_ROOT}/include")
            if(CMAKE_SIZEOF_VOID_P EQUAL 8)
                set(SDL2_IMAGE_LIB "${SDL2_IMAGE_ROOT}/lib/x64/SDL2_image.lib")
                set(SDL2_IMAGE_DLLS "${SDL2_IMAGE_ROOT}/lib/x64/SDL2_image.dll")
            else()
                set(SDL2_IMAGE_LIB "${SDL2_IMAGE_ROOT}/lib/x86/SDL2_image.lib")
                set(SDL2_IMAGE_DLLS "${SDL2_IMAGE_ROOT}/lib/x86/SDL2_image.dll")
            endif()
        endif()
    endif()

    message("SDL2_IMAGE_ROOT : ${SDL2_IMAGE_ROOT}")
    mark_as_advanced(SDL2_IMAGE_ROOT)
    find_package_handle_standard_args(SDL2_image SDL2_IMAGE_INCLUDE_DIR SDL2_IMAGE_LIB SDL2_IMAGE_DLLS)
else()
    # On MacOS, should be installed via Macports
    # On Ubuntu, install with: apt-get install libsdl2-dev
    find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h PATH_SUFFIXES SDL2_IMAGE)
    find_library(_SDL2_LIB SDL2_IMAGE)
    set(SDL2_LIBS ${SDL2})
    if(_SDL2_use_main)
        find_library(_SDL2main_LIB SDL2_IMAGE)
        list(APPEND SDL2_IMAGE_LIB)
    endif()

    mark_as_advanced(SDL2_IMAGE_INCLUDE_DIR _SDL2_LIB _SDL2main_LIB)
    find_package_handle_standard_args(SDL2_image SDL2_IMAGE_INCLUDE_DIR SDL2_IMAGE_LIB)
endif()