cmake_minimum_required(VERSION 3.9)
project(FR2017)

set(CMAKE_CXX_STANDARD 17)

find_package( Boost REQUIRED COMPONENTS system filesystem REQUIRED )
if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()
include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src SRC_DIR)
find_package( OpenCV REQUIRED )
find_package( Shark REQUIRED )
add_executable(FR2017 main.cpp ${SRC_DIR} )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
target_link_libraries (${PROJECT_NAME}
        OpenMeshCore
        OpenMeshTools
        )
target_link_libraries(${PROJECT_NAME} ${SHARK_LIBRARIES})
if(Boost_FOUND)
    target_link_libraries(FR2017 ${Boost_LIBRARIES})
endif()