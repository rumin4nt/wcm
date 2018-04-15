cmake_minimum_required(VERSION 3.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(wcm)
set(project_name wcm)

set(wcm_VERSION_MAJOR 0)
set(wcm_VERSION_MINOR 0)
set(wcm_VERSION_PATCH 1)

option(BuildLibrary "Build wcm library" ON)
#option(BuildDocumentation "Build documentation (requires doxygen)" OFF)
option(BuildDemo "BuildDemo" ON)
option(BuildTest "BuildTest" ON)


if(CMAKE_BUILD_TYPE)
  if (${CMAKE_BUILD_TYPE} STREQUAL "release")
    #set(BuildDocumentation ON)
  else()
    set(CMAKE_BUILD_TYPE "debug")
  endif()
else()
  set(CMAKE_BUILD_TYPE "debug")
endif()

macro(use_c99)
  if(CMAKE_VERSION VERSION_LESS "3.1")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set(CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
    endif()
  else()
    set(CMAKE_C_STANDARD 99)
  endif()
endmacro(use_c99)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(OperatingSystem "macos")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  SET(OperatingSystem "linux")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(WIN32)
  SET(OperatingSystem "windows")
ENDIF()

#find_package(Doxygen)
#option(BUILD_DOCUMENTATION
#       "Create and install the HTML based API
#documentation (requires Doxygen)"
#       ${DOXYGEN_FOUND})

#if(BUILD_DOCUMENTATION)
#  set(build_doc YES)
#endif()

find_package(LibGLFW )
include_directories(${LibGLFW_INCLUDE_DIRS})
set(LIBS ${LIBS} ${LibGLFW_LIBRARIES})

if (GLFW_FOUND)
	if (${CMAKE_BUILD_TYPE} STREQUAL "release")
		set(BuildDemo YES)
	endif()
	#if(BuildDemo)
	#	set(build_demo YES)
	#endif()
else()
	message("Did not find GLFW, cannot build demo.")
endif()


if(${OperatingSystem} MATCHES "macos")
  file(GLOB sources
            *.c
            src/wcm.c
            src/*.m
            ${CMAKE_CURRENT_SOURCE_DIR}/contrib/wacom/macos/*.m)
  FIND_LIBRARY(COCOA_LIB Cocoa)
  FIND_LIBRARY(OPENGL_LIB OpenGL)
  FIND_LIBRARY(CARBON_LIB Carbon)
  FIND_LIBRARY(CV_LIB CoreVideo)
  FIND_LIBRARY(FOUNDATION_LIB Foundation)

endif()

if(${OperatingSystem} MATCHES "windows")
  file(GLOB sources src/wcm.c src/wcm_tablet_ez.c src/wcm_tablet_ez_windows.c)
endif()

if(${OperatingSystem} MATCHES "linux")
  file(GLOB sources wcm.c src/wcm_tablet_ez.c src/*linux*.c src/*x11*.c)
  add_library(wcm STATIC ${sources} ${FOUNDATION_LIB}) # target_link_libraries(w
                                                       # cm LINK_PUBLIC )
endif()

if(${OperatingSystem} MATCHES "macos")
  add_library(wcm STATIC ${sources} ${FOUNDATION_LIB})
  target_link_libraries(
    wcm LINK_PUBLIC ${COCOA_LIB} ${CARBON_LIB} ${CV_LIB} ${OPENGL_LIB})
endif()

if(BuildDemo)
  add_subdirectory(demo)
endif()

target_include_directories(${project_name} PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(${project_name} PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/include)
target_include_directories(${project_name} PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib)
target_include_directories(${project_name} PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/deps)

