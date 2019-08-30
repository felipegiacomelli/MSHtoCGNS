CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

##################################################################
# BUILD FILES OUTPUT LOCATION
##################################################################
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/static_libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/shared_libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)

##################################################################
# OPERATIONAL SYSTEMS CONFIGURATION
##################################################################
if(UNIX)
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m64 -std=c++17 -Wall -Wextra -Wnon-virtual-dtor -pedantic -Wno-parentheses -fsanitize=undefined" CACHE STRING "Default compiler flags")

    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif()
    string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)

    if(BUILD_SHARED_LIBS)
        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fPIC" CACHE STRING "Necessary for shared libraries")
        set(LIBRARY_TYPE "shared")
    else()
        set(LIBRARY_TYPE "static")
    endif()

    set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-g -DDEBUG" CACHE STRING "Debug compiler flags")
    set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-O3 -DNDEBUG" CACHE STRING "Release compiler flags")
else()
    message(FATAL_ERROR "CMAKE HAS NOT BEEN CONFIGURED FOR THE CURRENT OPERATIONAL SYSTEM.\n")
endif()

add_definitions("-DTEST_INPUT_DIRECTORY=\"${CMAKE_SOURCE_DIR}/Zeta/Test/\"")
add_definitions("-DSCRIPT_DIRECTORY=\"${CMAKE_SOURCE_DIR}/Zeta/\"")

##################################################################
# ABOUT THE PROJECT
##################################################################
project(MSHtoCGNS)
set(VERSION_MAJOR "2")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/Libraries/mshtocgns-${VERSION}/${BUILD_TYPE}/${LIBRARY_TYPE}" CACHE PATH "Installation location" FORCE)
endif()

##################################################################
# EXTERNAL LIBRARIES
##################################################################
if(DEFINED ENV{CMAKE_MODULES})
    set(CMAKE_MODULE_PATH $ENV{CMAKE_MODULES})
else()
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Zeta/CMakeModules)
endif()

##############
# BOOST
##############
set(BOOST_VERSION "1.70.0")

if(DEFINED ENV{BOOST_DIR})
    set(BOOST_ROOT $ENV{BOOST_DIR}/${BUILD_TYPE}/${LIBRARY_TYPE})
else()
    set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/Zeta/Libraries/boost-${BOOST_VERSION}/${BUILD_TYPE})
endif()

set(Components_Boost system filesystem unit_test_framework test_exec_monitor)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost ${BOOST_VERSION} EXACT COMPONENTS ${Components_Boost} REQUIRED)
if(Boost_FOUND)
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
endif()

##############
# CGNS
##############
set(CGNS_VERSION "3.4.0")

if(DEFINED ENV{CGNS_DIR})
    set(CGNS_DIR $ENV{CGNS_DIR}/${BUILD_TYPE}/${LIBRARY_TYPE})
else()
    set(CGNS_DIR ${CMAKE_SOURCE_DIR}/Zeta/Libraries/cgns-${CGNS_VERSION}/${BUILD_TYPE})
endif()

find_package(CGNS ${CGNS_VERSION} EXACT COMPONENTS cgns_${LIBRARY_TYPE} REQUIRED)
if(CGNS_FOUND)
    include_directories(${CGNS_INCLUDE_DIR})
endif()

##################################################################
# MACROS
##################################################################
macro(add_executable _target)
    _add_executable(${_target} ${ARGN})
    target_link_libraries(${_target} ${Boost_LIBRARIES})
    target_link_libraries(${_target} ${CGNS_LIBRARIES})
endmacro()

set(TargetsConfig "${PROJECT_NAME}Targets")
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/libs)

macro(add_library _target)
    _add_library(${_target} ${ARGN})
    target_link_libraries(${_target} ${Boost_LIBRARIES})
    target_link_libraries(${_target} ${CGNS_LIBRARIES})
    set_target_properties(${_target}  PROPERTIES PREFIX "")
    set_target_properties(${_target}  PROPERTIES VERSION ${VERSION})
    set_property(GLOBAL APPEND PROPERTY GlobalTargetList ${_target})
    install(TARGETS ${PROJECT_NAME} EXPORT ${TargetsConfig} DESTINATION ${LIB_INSTALL_DIR})
endmacro()

##################################################################
# COMPONENTS
##################################################################
enable_testing()
option(SKIP_MSH "Skip MshInterface build" OFF)

add_subdirectory(CgnsInterface)
add_subdirectory(CgnsInterfaceTest)
add_test(NAME CgnsInterfaceTest COMMAND CgnsInterfaceTest)

add_subdirectory(Manipulation)
add_subdirectory(ManipulationTest)
add_test(NAME ManipulationTest COMMAND ManipulationTest)

add_subdirectory(Utilities)
add_subdirectory(UtilitiesTest)
add_test(NAME UtilitiesTest COMMAND UtilitiesTest)

if(NOT SKIP_MSH)
    add_subdirectory(MshInterface)
    add_subdirectory(MshInterfaceTest)
    add_test(NAME MshInterfaceTest COMMAND MshInterfaceTest)

    add_subdirectory(Manipulator)
    add_subdirectory(MSHtoCGNS)
endif()

##################################################################
# INSTALLATION
##################################################################
set(TemplateConfig "${CMAKE_SOURCE_DIR}/Zeta/CMake/${PROJECT_NAME}Config.cmake.in")
set(ProjectConfig "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake")
set(VersionConfig "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
get_property(PROJECT_LIBRARIES GLOBAL PROPERTY GlobalTargetList)

include(CMakePackageConfigHelpers)
configure_package_config_file(${TemplateConfig} ${ProjectConfig} INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX} PATH_VARS PROJECT_NAME_UPPER PROJECT_NAME INCLUDE_INSTALL_DIR LIB_INSTALL_DIR PROJECT_LIBRARIES)

write_basic_package_version_file(${VersionConfig} VERSION ${VERSION} COMPATIBILITY ExactVersion)

install(FILES ${ProjectConfig} ${VersionConfig} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/MSHtoCGNS DESTINATION ${INCLUDE_INSTALL_DIR})
install(EXPORT ${TargetsConfig} DESTINATION ${CMAKE_INSTALL_PREFIX})

##################################################################
# DISPLAY INFORMATION
##################################################################
message("\n")
message(STATUS "Project: ${PROJECT_NAME} ${VERSION}")
message(STATUS "Build type: ${BUILD_TYPE}")
message(STATUS "Library type: ${LIBRARY_TYPE}")

message("\n")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compile flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "Release flags: ${CMAKE_CXX_FLAGS_RELEASE}")

message("\n")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
