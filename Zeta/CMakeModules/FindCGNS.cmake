# CGNS_FOUND
# CGNS_INCLUDE_DIR
# CGNS_LIBRARIES

include (${CGNS_DIR}/CGNSConfigVersion.cmake)
include (${CGNS_DIR}/CGNSConfig.cmake)
include (${CGNS_DIR}/CGNSTargets.cmake)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (CGNS REQUIRED_VARS CGNS_LIBRARY_DIR CGNS_LIBRARIES CGNS_INCLUDE_DIR VERSION_VAR PACKAGE_VERSION)
mark_as_advanced (CGNS_INCLUDE_DIR CGNS_LIBRARY_DIR)
