cmake_minimum_required(VERSION 3.10)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
PROJECT(
  hdmap
  LANGUAGES CXX C
  HOMEPAGE_URL https://github.com/Fadis/hdmap
  DESCRIPTION "High Dimensional Map"
  VERSION 0.1.0
)
include(CheckIncludeFiles)
include(FindPkgConfig)
include(CheckCXXSourceCompiles)
include(GNUInstallDirs)


set( ADDITIONAL_COMPILE_FLAGS )
option( ENABLE_ADDRSAN "build with address sanitizer" OFF )
if( ENABLE_ADDRSAN )
  list(APPEND ADDITIONAL_COMPILE_FLAGS "-fsanitize=address" )
  if( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
    list(APPEND ADDITIONAL_COMPILE_FLAGS "-static-libasan" )
  endif()
endif()
option( ENABLE_PCOMPSAN "build with pointer compare sanitizer" OFF )
if( ENABLE_PCOMPSAN )
  list(APPEND ADDITIONAL_COMPILE_FLAGS "-fsanitize=pointer-compare" )
endif()
option( ENABLE_PSUBSAN "build with pointer subtract sanitizer" OFF )
if( ENABLE_PSUBSAN )
  list(APPEND ADDITIONAL_COMPILE_FLAGS "-fsanitize=pointer-subtract" )
endif()
option( ENABLE_THREADSAN "build with thread sanitizer" OFF )
if( ENABLE_THREADSAN )
  list(APPEND ADDITIONAL_COMPILE_FLAGS "-fsanitize=thread" )
endif()
option( ENABLE_LEAKSAN "build with leak sanitizer" OFF )
if( ENABLE_LEAKSAN )
  list(APPEND ADDITIONAL_COMPILE_FLAGS "-fsanitize=leak" )
endif()
option( ENABLE_UBSAN "build with undefined behavior sanitizer" OFF )
if( ENABLE_UBSAN )
  list(APPEND ADDITIONAL_COMPILE_FLAGS "-fsanitize=undefined" )
endif()
option( ENABLE_WERROR "build with -Werror" ON )
list( APPEND ADDITIONAL_COMPILE_FLAGS "-Wall" )
if( ENABLE_WERROR )
  list( APPEND ADDITIONAL_COMPILE_FLAGS "-Werror" )
endif()
if( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
  if( CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 10.0 )
    option( ENABLE_STATIC_ANALYZER "enable static analyzer" OFF)
    if( ENABLE_STATIC_ANALYZER )
      list(APPEND ADDITIONAL_COMPILE_FLAGS "-fanalyzer")
    endif()
  endif()
endif()
set( CMAKE_CXX_STANDARD "20" )

# compile option (Debug) 
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g ${ADDITIONAL_COMPILER_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g ${ADDITIONAL_COMPILER_FLAGS}")

# compile option (Release)
set(CMAKE_CXX_FLAGS_RELEASE "-march=native -O2 -fforce-addr -fomit-frame-pointer -fstrength-reduce -fno-strict-aliasing -funroll-loops ${ADDITIONAL_COMPILER_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "-march=native -O2 -fforce-addr -fomit-frame-pointer -fstrength-reduce -fno-strict-aliasing -funroll-loops ${ADDITIONAL_COMPILER_FLAGS}")

#if you don't want the full compiler output, remove the following line
set(CMAKE_VERBOSE_MAKEFILE OFF)

find_package(Boost 1.68.0 COMPONENTS system unit_test_framework REQUIRED )
find_package(JSON REQUIRED)
enable_testing()

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
  ${JSON_INCLUDE_DIRS}
)
link_directories(
  ${Boost_LIBRARY_DIRS}
)

set( EASYPACK_VENDOR "fadis" )
set( EASYPACK_RELEASE "1" )
set( EASYPACK_MAINTAINER "Naomasa Matsubayashi <foo@exmple.com>" )
set( EASYPACK_COMPRESSION_TYPE "xz" )
set( EASYPACK_LICENSE_NAME "MIT" )
include(EasyPack)

configure_file( ${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY) 
install(
  FILES
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
  DESTINATION
  ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig
  PERMISSIONS
  OWNER_READ OWNER_WRITE
  GROUP_READ
  WORLD_READ
)

subdirs( include test )

