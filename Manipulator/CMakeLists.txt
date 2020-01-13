project(Manipulator)

set(Dependencies Utilities CgnsInterface Manipulation)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE ${PROJECT_NAME}_sources ${PROJECT_SOURCE_DIR}/source/*.cpp)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_sources})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${EXE_INSTALL_DIR})

foreach(Dependency ${Dependencies})
    target_link_libraries(${PROJECT_NAME} ${Dependency})
endforeach()
