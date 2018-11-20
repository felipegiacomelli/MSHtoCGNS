CMAKE_MINIMUM_REQUIRED (VERSION 3.6)

##################################################################
# BUILD FILES OUTPUT LOCATION
##################################################################
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/static_libs)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/shared_libs)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (CMAKE_CXX_STANDARD 17)

##################################################################
# OPERATIONAL SYSTEMS CONFIGURATION
##################################################################
if (UNIX)
    message ("\n-- UNIX has been detected.")
    set (CMAKE_C_COMPILER gcc)
    set (CMAKE_CXX_COMPILER g++)
    set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m64 -std=c++17 -Wall -Wextra -Wnon-virtual-dtor -pedantic -Wno-parentheses" CACHE STRING "Setting default compiler flags.")

    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set (BUILD_TYPE_OUTPUT_DIRECTORY "Debug")
    elseif (CMAKE_BUILD_TYPE MATCHES "Release")
        set (BUILD_TYPE_OUTPUT_DIRECTORY "Release")
    else ()
        message ("\n-- CMAKE_BUILD_TYPE must be set to Debug or Release.\n-- It will be set to Release.\n")
        set (CMAKE_BUILD_TYPE "Release")
        set (BUILD_TYPE_OUTPUT_DIRECTORY "Release")
    endif ()

    if (BUILD_SHARED_LIBS)
        set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fPIC" CACHE STRING "Setting default compiler flags.")
        set (LIBRARY_TYPE_OUTPUT_DIRECTORY "Shared")
    else ()
        set (LIBRARY_TYPE_OUTPUT_DIRECTORY "Static")
    endif ()

    set (CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-g -DDEBUG" CACHE STRING "Setting default compiler flags.")
    set (CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-O3 -DNDEBUG" CACHE STRING "Setting default compiler flags")

    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${BUILD_TYPE_OUTPUT_DIRECTORY})
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${BUILD_TYPE_OUTPUT_DIRECTORY})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BUILD_TYPE_OUTPUT_DIRECTORY})
else ()
    message ("\n-- CMAKE HAS NOT BEEN CONFIGURED FOR THE CURRENT OPERATIONAL SYSTEM.\n")
endif ()

add_definitions("-DTEST_INPUT_DIRECTORY=\"${CMAKE_SOURCE_DIR}/Zeta/Test/\"")
add_definitions("-DSCRIPT_DIRECTORY=\"${CMAKE_SOURCE_DIR}/Zeta/\"")

##################################################################
# ABOUT THE PROJECT
##################################################################
project (MSHtoCGNS)
set (VERSION_MAJOR "0")
set (VERSION_MINOR "10")
set (VERSION_PATCH "0")
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "$ENV{HOME}/Libraries/${PROJECT_NAME}-${VERSION}" CACHE PATH "Installation location" FORCE)
endif ()

##################################################################
# EXTERNAL LIBRARIES
##################################################################
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Zeta/CMakeModules)
set (BOOST_VERSION "1.67.0")
set (CGNS_VERSION  "3.3.1" )

##############
# BOOST
##############
set (BOOST_ROOT ${CMAKE_SOURCE_DIR}/Zeta/Libraries/boost-1.67.0/${BUILD_TYPE_OUTPUT_DIRECTORY})
set (Components_Boost system filesystem unit_test_framework test_exec_monitor)
set (Boost_USE_MULTITHREADED ON)
find_package (Boost ${BOOST_VERSION} COMPONENTS ${Components_Boost} REQUIRED)
if (Boost_FOUND)
    include_directories (${Boost_INCLUDE_DIRS})
endif ()

##############
# CGNS
##############
set (CGNS_DIR ${CMAKE_SOURCE_DIR}/Zeta/Libraries/cgns-3.3.1/${BUILD_TYPE_OUTPUT_DIRECTORY})
find_package (CGNS ${CGNS_VERSION} REQUIRED)
if (CGNS_FOUND)
    include_directories (${CGNS_INCLUDE_DIR})
endif ()

##############
# MACROS
##############
macro (add_executable _target)
    _add_executable (${_target} ${ARGN})
    target_link_libraries (${_target} ${Boost_LIBRARIES})
    target_link_libraries (${_target} ${CGNS_LIBRARIES})
endmacro ()

set (Distribution "${PROJECT_NAME}Config")

macro (add_library _target)
    _add_library (${_target} ${ARGN})
    target_link_libraries (${_target} ${Boost_LIBRARIES})
    target_link_libraries (${_target} ${CGNS_LIBRARIES})
    set_target_properties (${_target}  PROPERTIES PREFIX "" VERSION ${VERSION})
    install (TARGETS ${PROJECT_NAME} EXPORT ${Distribution} DESTINATION ${BUILD_TYPE_OUTPUT_DIRECTORY}/${LIBRARY_TYPE_OUTPUT_DIRECTORY}/libs)
    install (DIRECTORY ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION ${BUILD_TYPE_OUTPUT_DIRECTORY}/${LIBRARY_TYPE_OUTPUT_DIRECTORY}/include)
    install (EXPORT ${Distribution} DESTINATION ${BUILD_TYPE_OUTPUT_DIRECTORY}/${LIBRARY_TYPE_OUTPUT_DIRECTORY})
endmacro ()

install (DIRECTORY ${CMAKE_SOURCE_DIR}/include/Grid DESTINATION ${BUILD_TYPE_OUTPUT_DIRECTORY}/${LIBRARY_TYPE_OUTPUT_DIRECTORY}/include)
install (DIRECTORY ${CMAKE_SOURCE_DIR}/include/Utilities DESTINATION ${BUILD_TYPE_OUTPUT_DIRECTORY}/${LIBRARY_TYPE_OUTPUT_DIRECTORY}/include)

##################################################################
# COMPONENTS
##################################################################
add_subdirectory (BoostInterface)
add_subdirectory (BoostInterfaceTest)
add_subdirectory (MshInterface)
add_subdirectory (MshInterfaceTest)
add_subdirectory (CgnsInterface)
add_subdirectory (CgnsInterfaceTest)
add_subdirectory (FileMend)
add_subdirectory (FileMendTest)
add_subdirectory (UtilitiesTest)

# add_subdirectory (CartesianWell)
add_subdirectory (Mender)
add_subdirectory (MSHtoCGNS)
add_subdirectory (MultipleBases)

#################################################################
# TESTING
#################################################################
enable_testing()
add_test(NAME BoostInterfaceTest COMMAND BoostInterfaceTest)
add_test(NAME MshInterfaceTest COMMAND MshInterfaceTest)
add_test(NAME CgnsInterfaceTest COMMAND CgnsInterfaceTest)
add_test(NAME FileMendTest COMMAND FileMendTest)
add_test(NAME UtilitiesTest COMMAND UtilitiesTest)

##################################################################
# DISPLAY INFORMATION
##################################################################
message ("\n-- Project: ${PROJECT_NAME} ${VERSION}")
message ("-- Build type: ${BUILD_TYPE_OUTPUT_DIRECTORY}")
message ("-- Install prefix: ${CMAKE_INSTALL_PREFIX}\n")
message ("-- C++ compiler: ${CMAKE_CXX_COMPILER}")
message ("-- Compile flags: ${CMAKE_CXX_FLAGS}")
message ("-- Debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
message ("-- Release flags: ${CMAKE_CXX_FLAGS_RELEASE}\n")
