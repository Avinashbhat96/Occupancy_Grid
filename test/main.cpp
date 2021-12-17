/**
 * @file main.cpp
 * Testing occupancy grid class with generated laser data
 */
#include "occupancy_grid/laserdata.h"
#include "occupancy_grid/OccupancyGrid.h"
#include "occupancy_grid/test_utils.h"
#include <iostream>

int main(){
    // number of readings per scan
    const int num_of_readings {360};

    // Instance of the occupancy grid
    FDTECH::OccupancyGrid occ_grid;
    
    // setting resolution of the grid cell
    occ_grid.setResolution(0.2);
    
    // setting size of the occupancy grid
    occ_grid.setSize(80, 80);
    
    // setting number of scans per scan
    occ_grid.setReadingPerScan(num_of_readings);

    // Initializing the position and radial velocity of the car and person
    const float person_start_pos {0.8}, car_start_pos {2.8};
    const float person_rad_vel {0.05}, car_rad_vel {0.1};
    
    // File name of occupancy grid
    std::string file_name {"occ_grid0.png"};

    // Loop over and updating the map, With the constant velocity for person and car
    for(int i {0}; i < 10; i++){

        // generating the laser data
        struct LaserData *laserdata = generateLaserData(num_of_readings, (car_start_pos + i * car_rad_vel), (person_start_pos + i * person_rad_vel));
        
        // calling sensor callback
        occ_grid.sensorCallback(laserdata);
        
        // updating the file name
        file_name.replace(file_name.find(".png")-1, 1, std::to_string(i));
        
        // saving map
        occ_grid.saveMap(file_name);
    }
    std::cout << "Test has been completed!" << std::endl;
    return 0;
}