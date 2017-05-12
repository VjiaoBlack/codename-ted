# - Locate HYDRAX LIBRARIES
# This module defines
#  HYDRAX_FOUND, if false, do not try to link to HYDRAX
#  HYDRAX_INCLUDE_DIR, where to find headers.
#  HYDRAX_LIBRARIES, the LIBRARIES to link against
#  HYDRAX_BINARY_REL - location of the main bullet binary (release)
#  HYDRAX_BINARY_DBG - location of the main bullet binaries (debug)

include(FindPkgMacros)
include(PreprocessorUtils)
findpkg_begin(HYDRAX)
set(HYDRAX_FIND_REQUIRED 1)

# Get path, convert backslashes as ${ENV_${var}}
getenv_path(HYDRAX_HOME)
getenv_path(HYDRAX_DIR)
getenv_path(HYDRAX_ROOT)
getenv_path(PROGRAMFILES)

# construct search paths from environmental hints and
# OS specific guesses
if (WIN32)
  set(HYDRAX_PREFIX_GUESSES
    ${ENV_PROGRAMFILES}/hydrax
    ${ENV_PROGRAMFILES}/HydraX
    ${ENV_PROGRAMFILES}/HYDRAX
    C:/HYDRAX-SDK
    C:/lib/hydrax
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;HYDRAX_HOME]
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;HYDRAX_DIR]
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;HYDRAX_ROOT]
  )
elseif (UNIX)
  set(HYDRAX_PREFIX_GUESSES
    /opt/hydrax
    /opt/HydraX
    /opt/HYDRAX
    /usr
    /usr/lib
    /usr/local
    $ENV{HOME}/hydrax
    $ENV{HOME}/HydraX
    $ENV{HOME}/HYDRAX
  )
endif ()

set(HYDRAX_PREFIX_PATH
   $ENV{HYDRAX_HOME} $ENV{HYDRAX_DIR} $ENV{HYDRAX_ROOT}
   ${HYDRAX_PREFIX_GUESSES}
)
create_search_paths(HYDRAX)

set(HYDRAX_RESET_VARS 
  HYDRAX_INCLUDE_DIR 
  HYDRAX_LIBRARY_REL HYDRAX_LIBRARY_DBG)

set(HYDRAX_PREFIX_WATCH ${HYDRAX_PREFIX_PATH})
clear_if_changed(HYDRAX_PREFIX_WATCH ${HYDRAX_RESET_VARS})

# try to locate HYDRAX via pkg-config
use_pkgconfig(HYDRAX_PKGC "HydraX")

# locate HYDRAX include files

find_path(HYDRAX_INCLUDE_DIR NAMES HydraX.h HINTS ${HYDRAX_INC_SEARCH_PATH} ${HYDRAX_FRAMEWORK_INCLUDES} ${HYDRAX_PKGC_INCLUDE_DIRS} PATH_SUFFIXES "" "hydrax" "HydraX")
get_debug_names(HYDRAX_LIBRARY_NAMES)
find_library(HYDRAX_LIBRARY_REL NAMES ${HYDRAX_LIBRARY_NAMES} HINTS ${HYDRAX_LIB_SEARCH_PATH} ${HYDRAX_PKGC_LIBRARY_DIRS} ${HYDRAX_FRAMEWORK_SEARCH_PATH} PATH_SUFFIXES "" "release" "relwithdebinfo" "minsizerel")
find_library(HYDRAX_LIBRARY_DBG NAMES ${HYDRAX_LIBRARY_NAMES_DBG} HINTS ${HYDRAX_LIB_SEARCH_PATH} ${HYDRAX_PKGC_LIBRARY_DIRS} ${HYDRAX_FRAMEWORK_SEARCH_PATH} PATH_SUFFIXES "" "debug")
make_library_set(HYDRAX_LIBRARY)


list(REMOVE_DUPLICATES HYDRAX_INCLUDE_DIR)
findpkg_finish(HYDRAX)
add_parent_dir(HYDRAX_INCLUDE_DIRS HYDRAX_INCLUDE_DIR)


if (NOT HYDRAX_FOUND)
  return()
endif ()



get_filename_component(HYDRAX_LIBRARY_DIR_REL "${HYDRAX_LIBRARY_REL}" PATH)
get_filename_component(HYDRAX_LIBRARY_DIR_DBG "${HYDRAX_LIBRARY_DBG}" PATH)
set(HYDRAX_LIBRARY_DIRS ${HYDRAX_LIBRARY_DIR_REL} ${HYDRAX_LIBRARY_DIR_DBG})


# find binaries
if (WIN32)
   find_file(HYDRAX_BINARY_REL NAMES "HydraX.dll" HINTS ${HYDRAX_BIN_SEARCH_PATH} PATH_SUFFIXES "" "release" "relwithdebinfo" "minsizerel")
   find_file(HYDRAX_BINARY_DBG NAMES "HydraX_d.dll" HINTS ${HYDRAX_BIN_SEARCH_PATH} PATH_SUFFIXES "" "debug" )
endif()
   
get_filename_component(HYDRAX_BINARY_DIR_REL "${HYDRAX_BINARY_REL}" PATH)
get_filename_component(HYDRAX_BINARY_DIR_DBG "${HYDRAX_BINARY_DBG}" PATH)
set(HYDRAX_LIBRARY_DIRS ${HYDRAX_BINARY_DIR_REL} ${HYDRAX_BINARY_DIR_DBG})
mark_as_advanced(HYDRAX_BINARY_REL HYDRAX_BINARY_DBG HYDRAX_BINARY_DIR_REL HYDRAX_BINARY_DIR_DBG)


clear_if_changed(HYDRAX_PREFIX_WATCH)