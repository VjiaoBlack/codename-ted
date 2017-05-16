#
# CMakeLists.txt
#
# Victor Jiao
# (Pulled from the Ogre Wiki Tutorials)
#
cmake_minimum_required(VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)
 
project(OgreApp)

include(cmake/FindCEGUI.cmake)

 
set(CMAKE_MODULE_PATH "/home/vjiaoblack/Classes/CMSC-23800/codename-ted/cmake/;${CMAKE_MODULE_PATH}")

if(EXISTS "/usr/local/lib/OGRE/cmake")
	set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/") # We could just *assume* that developers uses this basepath : /usr/local
elseif(EXISTS "/usr/lib/OGRE/cmake")
	set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
	set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one
PiratePanic ()
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


find_package(CEGUI REQUIRED)
if (NOT CEGUI_FOUND)
	message(SEND_ERROR "Failed to find CEGUI.")
endif()

link_directories( /usr/lib )
link_directories( /home/lib )
link_directories( /usr/include )
link_directories( /usr/lib/x86_64-linux-gnu )


# find_package(SKYX REQUIRED)

# Set up referencing of Boost
include_directories(${Boost_INCLUDE_DIR})
add_definitions(-DBOOST_ALL_NO_LIB)
set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})


#find_package(HYDRAX REQUIRED)
#include_directories(${HYDRAX INCLUDE_DIRS})
# set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${HYDRAX_LIBRARIES})

set(HDRS
	./src/PiratePanic.h
	./src/PiSkyX.h
	./src/TdBike.h

	./game_server/client.hpp
	./game_server/game_structs.hpp
	./game_server/json_serializer.hpp

	./src/hydrax/Enums.h
	./src/hydrax/Hydrax.h
	./src/hydrax/CfgFileManager.h
	./src/hydrax/Prerequisites.h
	./src/hydrax/CMakeCache.txt
	./src/hydrax/GodRaysManager.h
	./src/hydrax/Image.h
	./src/hydrax/Makefile
	./src/hydrax/RttManager.h
	./src/hydrax/CMakeLists.txt
	./src/hydrax/GPUNormalMapManager.h
	./src/hydrax/MaterialManager.h
	./src/hydrax/TextureManager.h
	./src/hydrax/DecalsManager.h
	./src/hydrax/Help.h
	./src/hydrax/Mesh.h
	./src/hydrax/Modules/Module.h
	./src/hydrax/Modules/ProjectedGrid/ProjectedGrid.h
	./src/hydrax/Modules/RadialGrid/RadialGrid.h
	./src/hydrax/Modules/SimpleGrid/SimpleGrid.h
	./src/hydrax/Noise/FFT/FFT.h
	./src/hydrax/Noise/Noise.h
	./src/hydrax/Noise/Perlin/Perlin.h

	./src/skyx/GPUManager.h
	./src/skyx/Prerequisites.h
	./src/skyx/AtmosphereManager.h
	./src/skyx/SkyX.h
	./src/skyx/BasicController.h
	./src/skyx/MeshManager.h
	./src/skyx/ColorGradient.h
	./src/skyx/CloudsManager.h
	./src/skyx/Controller.h
	./src/skyx/MoonManager.h
	./src/skyx/VCloudsManager.h
	./src/skyx/VClouds/Ellipsoid.h
	./src/skyx/VClouds/GeometryBlock.h
	./src/skyx/VClouds/Lightning.h
	./src/skyx/VClouds/VClouds.h
	./src/skyx/VClouds/DataManager.h
	./src/skyx/VClouds/FastFakeRandom.h
	./src/skyx/VClouds/GeometryManager.h
	./src/skyx/VClouds/LightningManager.h
)

set(SRCS
	./src/PiratePanic.cpp
	./src/PiSkyX.cpp
	./src/TdBike.cpp

	./game_server/client.cpp
	./game_server/game_structs.cpp
	./game_server/json_serializer.cpp

	./src/hydrax/CfgFileManager.cpp
	./src/hydrax/Prerequisites.cpp
	./src/hydrax/GodRaysManager.cpp
	./src/hydrax/Image.cpp
	./src/hydrax/CMakeCache.txt
	./src/hydrax/RttManager.cpp
	./src/hydrax/GPUNormalMapManager.cpp
	./src/hydrax/Makefile
	./src/hydrax/CMakeLists.txt
	./src/hydrax/MaterialManager.cpp
	./src/hydrax/TextureManager.cpp
	./src/hydrax/DecalsManager.cpp
	./src/hydrax/Help.cpp
	./src/hydrax/Mesh.cpp
	./src/hydrax/Enums.cpp
	./src/hydrax/Hydrax.cpp
	./src/hydrax/Modules/Module.cpp
	./src/hydrax/Modules/ProjectedGrid/ProjectedGrid.cpp
	./src/hydrax/Modules/RadialGrid/RadialGrid.cpp
	./src/hydrax/Modules/SimpleGrid/SimpleGrid.cpp
	./src/hydrax/Noise/FFT/FFT.cpp
	./src/hydrax/Noise/Noise.cpp
	./src/hydrax/Noise/Perlin/Perlin.cpp

	./src/skyx/AtmosphereManager.cpp
	./src/skyx/SkyX.cpp
	./src/skyx/BasicController.cpp
	./src/skyx/MeshManager.cpp
	./src/skyx/ColorGradient.cpp
	./src/skyx/CloudsManager.cpp
	./src/skyx/MoonManager.cpp
	./src/skyx/VCloudsManager.cpp
	./src/skyx/GPUManager.cpp
	./src/skyx/Prerequisites.cpp
	./src/skyx/VClouds/DataManager.cpp
	./src/skyx/VClouds/FastFakeRandom.cpp
	./src/skyx/VClouds/GeometryManager.cpp
	./src/skyx/VClouds/LightningManager.cpp
	./src/skyx/VClouds/Ellipsoid.cpp
	./src/skyx/VClouds/GeometryBlock.cpp
	./src/skyx/VClouds/Lightning.cpp
	./src/skyx/VClouds/VClouds.cpp
)
 
 
include_directories(
	${OIS_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
	${OGRE_SAMPLES_INCLUDEPATH}
	${OGRE_Overlay_INCLUDE_DIRS}
    ${CEGUI_INCLUDE_DIRS})

include_directories(/usr/local/include/cegui-0/CEGUI/)

 
add_executable(OgreApp WIN32 ${HDRS} ${SRCS})
 
set_target_properties(OgreApp PROPERTIES DEBUG_POSTFIX _d)


 
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
 
