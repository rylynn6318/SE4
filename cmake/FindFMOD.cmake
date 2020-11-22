if(WIN32)
    set(FMOD_DIR "${PROJECT_SOURCE_DIR}/extern/FMOD")
    message("FMOD_DIR : ${FMOD_DIR}")
endif()

IF(FMOD_LIBRARY AND FMOD_INCLUDE_DIR)
  # in cache already
  SET(FMOD_FIND_QUIETLY TRUE)
ENDIF(FMOD_LIBRARY AND FMOD_INCLUDE_DIR)

find_path(FMOD_INCLUDE_DIR fmod.h
        HINTS
        ENV FMODDIR
        ${FMOD_DIR}
        PATH_SUFFIXES FMOD
        # path suffixes to search inside ENV{SDL2DIR}
        PATHS
        ~/inc
        /inc
        )

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(VC_LIB_PATH_SUFFIX lib/x64)
else()
    set(VC_LIB_PATH_SUFFIX lib/x86)
endif()

if(NOT FMOD_LIBRARY)
    set(FMOD_LIBRARY ${FMODd_LIBRARY} )#CACHE FILEPATH "file cache entry initialized from old variable name")
endif()

find_library(FMOD_LIBRARY
        NAMES fmod_vc.lib
        HINTS
        ENV FMODDIR
        ${FMOD_DIR}
        PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX}
        PATHS
        ~/${VC_LIB_PATH_SUFFIX}
        /${VC_LIB_PATH_SUFFIX}
        )

set(FMOD_LIBRARIES ${FMOD_LIBRARY})
set(FMOD_INCLUDE_DIRS ${FMOD_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(FMOD
        REQUIRED_VARS FMOD_LIBRARIES FMOD_INCLUDE_DIRS)

# for backward compatibility
set(FMODd_LIBRARY ${FMOD_LIBRARIES})
set(FMOD_INCLUDE_DIR ${FMOD_INCLUDE_DIRS})
set(FMOD_FOUND ${FMOD_FOUND})

mark_as_advanced(FMOD_LIBRARY FMOD_INCLUDE_DIR)
message("FMOD_INCLUDE_DIR : ${FMOD_INCLUDE_DIR}")

# for Windows
set(FMOD_DLL_DIR "${FMOD_DIR}/${VC_LIB_PATH_SUFFIX}")
file(GLOB FMOD_DLLS "${FMOD_DLL_DIR}/*.dll")