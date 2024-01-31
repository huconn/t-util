# Required minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(T-UTIL)

#include path
if(NOT CMAKE_INSTALL_PREFIX)
	set(CMAKE_INSTALL_PREFIX "usr")
endif(NOT CMAKE_INSTALL_PREFIX)

if(NOT CMAKE_INSTALL_BINDIR)
	set(CMAKE_INSTALL_BINDIR "bin")
endif(NOT CMAKE_INSTALL_BINDIR)

if(NOT CMAKE_INSTALL_LIBDIR)
	set(CMAKE_INSTALL_LIBDIR "lib")
endif(NOT CMAKE_INSTALL_LIBDIR)

if(NOT CMAKE_INSTALL_INCLUDEDIR)
	set(CMAKE_INSTALL_INCLUDEDIR "include")
endif(NOT CMAKE_INSTALL_INCLUDEDIR)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GLIB2 glib-2.0)

message(STATUS "USE_T_TEST: '${USE_T_TEST}'")

###### stack canary
ADD_COMPILE_OPTIONS(-fstack-protector-all)

###### Include sub directory to compile
add_subdirectory(tu_calc)
add_subdirectory(tu_jp_factory)
if(USE_T_TEST)
add_subdirectory(tu_csv_report)
set(PC_LIB_REPORT "-ltu_csv_report")
endif()

configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)