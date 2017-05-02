#/*
#-----------------------------------------------------------------------------
# Filename:    CMakeLists.txt
#-----------------------------------------------------------------------------
#
# This source file is part of the
#    ___                 __    __ _ _    _ 
#   /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
#  //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
# / \_// (_| | | |  __/  \  /\  /| |   <| |
# \___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
#       |___/                              
#       Tutorial Framework
#       http://www.ogre3d.org/tikiwiki/
#-----------------------------------------------------------------------------
#*/
cmake_minimum_required(VERSION 2.6)
 
project(OgreApp)

include(cmake/FindCEGUI.cmake)
 
set(CMAKE_MODULE_PATH "/home/vjiaoblack/Compsci/;${CMAKE_MODULE_PATH}")


if(EXISTS "/usr/local/lib/OGRE/cmake")

	set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/") # We could just *assume* that developers uses this basepath : /usr/local

elseif(EXISTS "/usr/lib/OGRE/cmake")

	set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one

else ()
	message(SEND_ERROR "Failed to find module path.")
endif()
 
if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()
 
set(CMAKE_DEBUG_POSTFIX "_d")
 
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")
 
find_package(OGRE REQUIRED)
find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
	message(SEND_ERROR "Failed to find OIS.")
endif()
 
# Find Boost
# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
set(Boost_ADDITIONAL_VERSIONS "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )

# Components that need linking (NB does not include header-only components like bind)
set(OGRE_BOOST_COMPONENTS thread date_time)
find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)

if (NOT Boost_FOUND)
# Try again with the other type of libs
	set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
	find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
endif()

find_package(Boost QUIET)

# Set up referencing of Boost
include_directories(${Boost_INCLUDE_DIR})
add_definitions(-DBOOST_ALL_NO_LIB)
set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
 
 
set(HDRS
	./src/BaseApplication.h
	./src/TutorialApplication.h
)
 
set(SRCS
	./src/BaseApplication.cpp
	./src/TutorialApplication.cpp
)
 
 
include_directories(
	${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	${OGRE_Overlay_INCLUDE_DIRS}
    ${CEGUI_INCLUDE_DIRS})

 
add_executable(OgreApp WIN32 ${HDRS} ${SRCS})
 
set_target_properties(OgreApp PROPERTIES DEBUG_POSTFIX _d)
find_package(CEGUI REQUIRED OGRE)
 
target_link_libraries(
  OgreApp
  ${OGRE_LIBRARIES}
  ${OIS_LIBRARIES}
  ${OGRE_Overlay_LIBRARIES}
  ${OGRE_Terrain_LIBRARIES}
  ${CEGUI_LIBRARIES}
  ${CEGUI_OgreRenderer_LIBRARIES})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/res)
 
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build)

install(TARGETS OgreApp
	RUNTIME DESTINATION bin
	CONFIGURATIONS All)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/res
	DESTINATION ./
	CONFIGURATIONS Release RelWithDebInfo Debug
)

install(FILES ${CMAKE_SOURCE_DIR}/config/plugins.cfg
	${CMAKE_SOURCE_DIR}/config/resources.cfg
	DESTINATION bin
	CONFIGURATIONS Release RelWithDebInfo Debug
)
 
