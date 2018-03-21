cmake_minimum_required (VERSION 3.0)

project(wcm)
set(project_name wcm)

set (wcm_VERSION_MAJOR 0)
set (wcm_VERSION_MINOR 0)
set (wcm_VERSION_PATCH 1)

macro(use_c99)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
    endif ()
  else ()
    set (CMAKE_C_STANDARD 99)
  endif ()
endmacro(use_c99)

find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API
documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
	set(build_doc YES)
endif()

file(GLOB sources src/*.c)

add_library(wcm SHARED ${sources})

target_include_directories (${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories (${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/include)
target_include_directories (${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib)
target_include_directories (${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/deps)

