CMAKE_MINIMUM_REQUIRED (VERSION 3.6)

##################################################################
# BUILD FILES OUTPUT LOCATION
##################################################################
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/static_libs)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/shared_libs)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##################################################################
# OPERATIONAL SYSTEMS CONFIGURATION
##################################################################
if (UNIX)
	message ("\n-- UNIX has been detected.")
	set (CMAKE_C_COMPILER $ENV{OPENMPI_DIR}/bin/mpicc)
	set (CMAKE_CXX_COMPILER $ENV{OPENMPI_DIR}/bin/mpicxx)

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
		set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m64 -std=c++14 -fPIC" CACHE STRING "Setting default compiler flags.")
		set (LIBRARY_TYPE_OUTPUT_DIRECTORY "Shared")
    else ()
		set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m64 -std=c++14" CACHE STRING "Setting default compiler flags.")
		set (LIBRARY_TYPE_OUTPUT_DIRECTORY "Static")
    endif ()

	set (CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-g -DDEBUG -Wall" CACHE STRING "Setting default compiler flags.")
	set (CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-O3 -DNDEBUG -Wno-expansion-to-defined -Wno-deprecated-declarations" CACHE STRING "Setting default compiler flags")

	set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${BUILD_TYPE_OUTPUT_DIRECTORY})
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${BUILD_TYPE_OUTPUT_DIRECTORY})
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BUILD_TYPE_OUTPUT_DIRECTORY})
else ()
	message ("\n-- CMAKE HAS NOT BEEN CONFIGURED FOR THE CURRENT OPERATIONAL SYSTEM.\n")
endif ()

##################################################################
# ABOUT THE PROJECT
##################################################################
project (MSHtoCGNS)
set (VERSION_MAJOR "0")
set (VERSION_MINOR "4")
set (VERSION_PATCH "0")
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set (CMAKE_INSTALL_PREFIX "$ENV{HOME}/Libraries/${PROJECT_NAME}-${VERSION}" CACHE PATH "Installation location" FORCE)
endif ()

##################################################################
# EXTERNAL LIBRARIES
##################################################################
set (CMAKE_MODULE_PATH $ENV{CMAKE_MODULES})
set (BOOST_VERSION "1.65.1")
set (CGNS_VERSION  "3.3.0" )

##############
# BOOST
##############
set (BOOST_ROOT $ENV{BOOST_DIR}/${BUILD_TYPE_OUTPUT_DIRECTORY}/${LIBRARY_TYPE_OUTPUT_DIRECTORY})
set (Components_Boost chrono context filesystem graph_parallel log mpi program_options regex serialization signals system thread timer wave date_time graph math_c99 random test_exec_monitor exception unit_test_framework serialization)
set (Boost_USE_MULTITHREADED ON)
find_package (Boost ${BOOST_VERSION} COMPONENTS ${Components_Boost} REQUIRED)
if (Boost_FOUND)
    include_directories (${Boost_INCLUDE_DIRS})
endif ()

##############
# CGNS
##############
set (CGNS_DIR $ENV{CGNS_DIR}/${BUILD_TYPE_OUTPUT_DIRECTORY}/${LIBRARY_TYPE_OUTPUT_DIRECTORY})
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

##################################################################
# COMPONENTS
##################################################################
add_subdirectory (BoostInterface)
add_subdirectory (BoostInterfaceTest)
add_subdirectory (IO)
add_subdirectory (IOTest)
add_subdirectory (CgnsInterface)
# add_subdirectory (CgnsInterfaceTest)
add_subdirectory (MSHtoCGNS) 

#################################################################
# TESTING
#################################################################
enable_testing()
add_test(NAME BoostInterfaceTest COMMAND BoostInterfaceTest)
add_test(NAME IOTest COMMAND IOTest)

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