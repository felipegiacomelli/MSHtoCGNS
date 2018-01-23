project (CgnsInterfaceTest)

set (Dependencies BoostInterface CgnsInterface)

include_directories (${CMAKE_SOURCE_DIR}/include)

file (GLOB ${PROJECT_NAME}_sources ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/source/*.cpp)

add_executable (${PROJECT_NAME} ${${PROJECT_NAME}_sources})

foreach (Dependency ${Dependencies})
	target_link_libraries (${PROJECT_NAME} ${Dependency})
endforeach ()