# RCSwitch Common Library
# Adaptation of RCSwitch Arduino library to be used on any system
#
# https://github.com/latchdevel/rc-switch-lib
#
# Copyright (c) 2024 Jorge Rivera. All right reserved.
# License GNU Lesser General Public License v3.0.

cmake_minimum_required(VERSION 3.18)

project(RCSwitch LANGUAGES CXX)

# Set version number for shared libraries and executables
set(CU_VERSION 1.0) # current version
set(SO_VERSION 1.0) # compatibility version

# Set C++ Standard
set(CMAKE_CXX_STANDARD 11)

# Set and display complier identification
if(NOT BUILD_COMPILER)
  SET(BUILD_COMPILER "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}" )
  MESSAGE( STATUS "Compiler: " ${BUILD_COMPILER} )
endif()

# Check if has parent directory
get_directory_property(hasParent PARENT_DIRECTORY)

# Check if set CMAKE_BUILD_TYPE var
if(NOT CMAKE_BUILD_TYPE)
  # Set default build type to "release" set -O3 -DNDEBUG
  set(DEFAULT_BUILD_TYPE "release")
  SET(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE})
  MESSAGE( STATUS "Build type set to default: " ${CMAKE_BUILD_TYPE} )
else()
  # Check if set and valid CMAKE_BUILD_TYPE var
  STRING( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE )
  if((CMAKE_BUILD_TYPE STREQUAL "debug") OR (CMAKE_BUILD_TYPE STREQUAL "release"))
    # If no has parent directory show message
    if(NOT hasParent)
      MESSAGE( STATUS "Build type set to: " ${CMAKE_BUILD_TYPE} )
    endif()
  else()
    MESSAGE( FATAL_ERROR "If set CMAKE_BUILD_TYPE it must be 'release' or 'debug'")
  endif()
endif()

# Setting build type to "debug" add only -g 
if(CMAKE_BUILD_TYPE STREQUAL "debug") 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
endif()

# Set C++ flags
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif(MSVC)
  set(MSVC_DISABLED_WARNINGS_LIST
    "C4996" # warning C4996: 'may be unsafe/disable deprecation'
            # To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
    "C4458" # warning C4458: 'declaration hides class member'
    "C4456" # warning C4456: 'declaration hides previous local declaration'
  )
  string(REPLACE "C" " -wd" MSVC_DISABLED_WARNINGS_STR ${MSVC_DISABLED_WARNINGS_LIST})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W4 ${MSVC_DISABLED_WARNINGS_STR}")
endif()

# If macOS builds a Mach-O universal binary with 2 architectures: x86_64 and arm64 for Apple M processors
if (APPLE) 
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
endif()

# Add sources for library to compile as object 
add_library( ${PROJECT_NAME}-obj OBJECT RCSwitch.cpp )

# Shared libraries need flag -fPIC
set_property( TARGET ${PROJECT_NAME}-obj PROPERTY POSITION_INDEPENDENT_CODE 1 )

# Shared library built from the same object files 
add_library( ${PROJECT_NAME}-dynamic SHARED $<TARGET_OBJECTS:${PROJECT_NAME}-obj> )

# File extension OS depends, like: RCSwitch.so or RCSwitch.dylib or RCSwitch.dll
set_target_properties( ${PROJECT_NAME}-dynamic PROPERTIES OUTPUT_NAME ${PROJECT_NAME} )

# Set version numbers for the versioned shared libraries target
set_target_properties( ${PROJECT_NAME}-dynamic PROPERTIES SOVERSION ${SO_VERSION} VERSION ${CU_VERSION} )

# Add static library, file extension OS depends, like: RCSwitch.a or RCSwitch.lib
add_library( ${PROJECT_NAME} STATIC $<TARGET_OBJECTS:${PROJECT_NAME}-obj> )

# Add install targets
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(TARGETS ${PROJECT_NAME}-dynamic DESTINATION lib)

# If no has parent directory, add uninstall targets
if(NOT hasParent)
  MESSAGE(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
  add_custom_target( uninstall
      "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/uninstall.cmake"
  )
endif()

# Add example source file, link static
add_executable( example example.cpp )
target_link_libraries( example PRIVATE ${PROJECT_NAME} )

# Add include directory to use #include <RCSwitch.h>
target_include_directories( example PRIVATE ${CMAKE_SOURCE_DIR} )
