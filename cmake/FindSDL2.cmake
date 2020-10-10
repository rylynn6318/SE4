# https://github.com/preshing/CMakeDemo/blob/master/modules/FindSDL2.cmake

#------------------------------------------------------------------------------
# Usage: find_package(SDL2 [REQUIRED] [COMPONENTS main])
#
# Sets variables:
#     SDL2_INCLUDE_DIRS
#     SDL2_LIBS
#     SDL2_DLLS
#------------------------------------------------------------------------------

include(FindPackageHandleStandardArgs)

# Check if "main" was specified as a component
set(_SDL2_use_main TRUE)
foreach(_SDL2_component ${SDL2_FIND_COMPONENTS})
    if(_SDL2_component STREQUAL "main")
        set(_SDL2_use_main TRUE)
    else()
        message(WARNING "Unrecognized component \"${_SDL2_component}\"")
    endif()
endforeach()

find_path(SDL2_INCLUDE_DIRS SDL.h PATH_SUFFIXES SDL2)
find_library(_SDL2_LIB SDL2)
set(SDL2_LIBS ${SDL2})
if(_SDL2_use_main)
    find_library(_SDL2main_LIB SDL2)
    list(APPEND SDL2_LIBS ${_SDL2main_LIB})
endif()

mark_as_advanced(SDL2_INCLUDE_DIRS _SDL2_LIB _SDL2main_LIB)
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_INCLUDE_DIRS SDL2_LIBS)
