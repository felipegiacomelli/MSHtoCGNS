# BUILD_TYPE_OUTPUT_DIRECTORY   - Release or Debug (CASE SENSITIVE)
# LIBRARY_TYPE_OUTPUT_DIRECTORY - Shared or Static (CASE SENSITIVE)
#
# NOTE: ${MSHtoCGNS_DIR} = $ENV{MSHtoCGNS_DIR}/BUILD_TYPE_OUTPUT_DIRECTORY/LIBRARY_TYPE_OUTPUT_DIRECTORY
#
# MSHtoCGNS_FOUND
# MSHtoCGNS_INCLUDE_DIR
# MSHtoCGNS_LIBRARIES

include (${MSHtoCGNS_DIR}/MSHtoCGNSConfig.cmake)
find_path (MSHtoCGNS_INCLUDE_DIR CgnsInterface/CgnsCreator.hpp ${MSHtoCGNS_DIR}/include)
if (BUILD_SHARED_LIBS)
    find_library (MSHtoCGNS_LIBRARY_DIR CgnsInterface.so ${MSHtoCGNS_DIR}/libs)
else ()
    find_library (MSHtoCGNS_LIBRARY_DIR CgnsInterface.a ${MSHtoCGNS_DIR}/libs)
endif()

set (MSHtoCGNS_COMPONENTS BoostInterface CgnsInterface MshInterface MSHtoCGNS)

set (MSHtoCGNS_FOUND FALSE)
if (MSHtoCGNS_INCLUDE_DIR)
    if (MSHtoCGNS_LIBRARY_DIR)
        set (MSHtoCGNS_FOUND TRUE)
        set (MSHtoCGNS_INCLUDE_DIR ${MSHtoCGNS_INCLUDE_DIR})
        set (MSHtoCGNS_LIBRARIES ${MSHtoCGNS_COMPONENTS})
    endif ()
endif ()

if (MSHtoCGNS_FOUND)
    message ("-- Found MSHtoCGNS: ${MSHtoCGNS_DIR}")
else ()
    message ("\n\n-- Could not find MSHtoCGNS.\n\n")
endif ()
