
string (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
set (TargetsConfig "${PROJECT_NAME_UPPER}Targets")
set (INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
set (LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")

set (TemplateConfig "${CMAKE_SOURCE_DIR}/../..//CMake/MSHtoCGNSConfig.cmake.in")
set (ProjectConfig "${CMAKE_BINARY_DIR}/${PROJECT_NAME_UPPER}Config.cmake")
set (VersionConfig "${CMAKE_BINARY_DIR}/${PROJECT_NAME_UPPER}ConfigVersion.cmake")

set (PROJECT_LIBRARIES "cgns_shared")
install (TARGETS "cgns_shared" EXPORT ${TargetsConfig} DESTINATION ${LIB_INSTALL_DIR})

include (CMakePackageConfigHelpers)
configure_package_config_file (${TemplateConfig} ${ProjectConfig} INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX} PATH_VARS PROJECT_NAME_UPPER PROJECT_NAME INCLUDE_INSTALL_DIR LIB_INSTALL_DIR PROJECT_LIBRARIES)

set (VERSION "3.4.0")
write_basic_package_version_file (${VersionConfig} VERSION ${VERSION} COMPATIBILITY ExactVersion)

install (FILES ${ProjectConfig} ${VersionConfig} DESTINATION ${CMAKE_INSTALL_PREFIX})
install (EXPORT ${TargetsConfig} DESTINATION ${CMAKE_INSTALL_PREFIX})
