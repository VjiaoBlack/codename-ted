# - Locate SKYX LIBRARIES
# This module defines
#  SKYX_FOUND, if false, do not try to link to SKYX
#  SKYX_INCLUDE_DIR, where to find headers.
#  SKYX_LIBRARIES, the LIBRARIES to link against
#  SKYX_BINARY_REL - location of the main bullet binary (release)
#  SKYX_BINARY_DBG - location of the main bullet binaries (debug)

include(FindPkgMacros)
include(PreprocessorUtils)
findpkg_begin(SKYX)
set(SKYX_FIND_REQUIRED 1)

# Get path, convert backslashes as ${ENV_${var}}
getenv_path(SKYX_HOME)
getenv_path(SKYX_DIR)
getenv_path(SKYX_ROOT)
getenv_path(PROGRAMFILES)

# construct search paths from environmental hints and
# OS specific guesses
if (WIN32)
  set(SKYX_PREFIX_GUESSES
    ${ENV_PROGRAMFILES}/skyx
    ${ENV_PROGRAMFILES}/SkyX
    ${ENV_PROGRAMFILES}/SKYX
    C:/SKYX-SDK
    C:/lib/skyx
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;SKYX_HOME]
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;SKYX_DIR]
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;SKYX_ROOT]
  )
elseif (UNIX)
  set(SKYX_PREFIX_GUESSES
    /opt/skyx
    /opt/SkyX
    /opt/SKYX
    /usr
    /usr/local
    $ENV{HOME}/skyx
    $ENV{HOME}/SkyX
    $ENV{HOME}/SKYX
  )
endif ()

set(SKYX_PREFIX_PATH
   $ENV{SKYX_HOME} $ENV{SKYX_DIR} $ENV{SKYX_ROOT}
   ${SKYX_PREFIX_GUESSES}
)
create_search_paths(SKYX)

set(SKYX_RESET_VARS 
  SKYX_INCLUDE_DIR 
  SKYX_LIBRARY_REL SKYX_LIBRARY_DBG)

set(SKYX_PREFIX_WATCH ${SKYX_PREFIX_PATH})
clear_if_changed(SKYX_PREFIX_WATCH ${SKYX_RESET_VARS})

# try to locate SKYX via pkg-config
use_pkgconfig(SKYX_PKGC "SkyX")

# locate SKYX include files

find_path(SKYX_INCLUDE_DIR NAMES SkyX.h HINTS ${SKYX_INC_SEARCH_PATH} ${SKYX_FRAMEWORK_INCLUDES} ${SKYX_PKGC_INCLUDE_DIRS} PATH_SUFFIXES "" "skyx" "SkyX")
get_debug_names(SKYX_LIBRARY_NAMES)
find_library(SKYX_LIBRARY_REL NAMES ${SKYX_LIBRARY_NAMES} HINTS ${SKYX_LIB_SEARCH_PATH} ${SKYX_PKGC_LIBRARY_DIRS} ${SKYX_FRAMEWORK_SEARCH_PATH} PATH_SUFFIXES "" "release" "relwithdebinfo" "minsizerel")
find_library(SKYX_LIBRARY_DBG NAMES ${SKYX_LIBRARY_NAMES_DBG} HINTS ${SKYX_LIB_SEARCH_PATH} ${SKYX_PKGC_LIBRARY_DIRS} ${SKYX_FRAMEWORK_SEARCH_PATH} PATH_SUFFIXES "" "debug")
make_library_set(SKYX_LIBRARY)


list(REMOVE_DUPLICATES SKYX_INCLUDE_DIR)
findpkg_finish(SKYX)
add_parent_dir(SKYX_INCLUDE_DIRS SKYX_INCLUDE_DIR)


if (NOT SKYX_FOUND)
  return()
endif ()



get_filename_component(SKYX_LIBRARY_DIR_REL "${SKYX_LIBRARY_REL}" PATH)
get_filename_component(SKYX_LIBRARY_DIR_DBG "${SKYX_LIBRARY_DBG}" PATH)
set(SKYX_LIBRARY_DIRS ${SKYX_LIBRARY_DIR_REL} ${SKYX_LIBRARY_DIR_DBG})


# find binaries
if (WIN32)
   find_file(SKYX_BINARY_REL NAMES "SkyX.dll" HINTS ${SKYX_BIN_SEARCH_PATH} PATH_SUFFIXES "" "release" "relwithdebinfo" "minsizerel")
   find_file(SKYX_BINARY_DBG NAMES "SkyX_d.dll" HINTS ${SKYX_BIN_SEARCH_PATH} PATH_SUFFIXES "" "debug" )
endif()
   
get_filename_component(SKYX_BINARY_DIR_REL "${SKYX_BINARY_REL}" PATH)
get_filename_component(SKYX_BINARY_DIR_DBG "${SKYX_BINARY_DBG}" PATH)
set(SKYX_LIBRARY_DIRS ${SKYX_BINARY_DIR_REL} ${SKYX_BINARY_DIR_DBG})
mark_as_advanced(SKYX_BINARY_REL SKYX_BINARY_DBG SKYX_BINARY_DIR_REL SKYX_BINARY_DIR_DBG)


clear_if_changed(SKYX_PREFIX_WATCH)