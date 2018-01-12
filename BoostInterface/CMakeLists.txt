project (BoostInterface)

include_directories (${CMAKE_SOURCE_DIR}/include)

file (GLOB ${PROJECT_NAME}_sources ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/source/*.cpp)

add_library (${PROJECT_NAME} ${${PROJECT_NAME}_sources})