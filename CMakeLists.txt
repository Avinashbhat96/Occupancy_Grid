cmake_minimum_required(VERSION 2.8.3)

project(Occupancy_Grid)

add_definitions(-std=c++14)

set(CMAKE_BUILD_TYPE "RELEASE")

find_package(OpenCV REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS} 
)

add_library(occupancyGrid
src/OccupancyGrid.cpp
)

target_link_libraries(occupancyGrid
    ${OpenCV_LIBS}
)

add_executable(test_occupancy_grid test/main.cpp)

target_link_libraries(test_occupancy_grid
    occupancyGrid
    ${OpenCV_LIBS}
)
