/**
 * @file OccupancyGrid.cpp
 * Implementation file for occupancy grid
 */
#include "occupancy_grid/OccupancyGrid.h"
#include "occupancy_grid/errorHandler.h"
#include <opencv2/imgcodecs.hpp>
#include <iostream>

// sensor callback to handle new sensor data
void OccupancyGrid::sensorCallback(const LaserData laser_scan []){
    try{
        if(pixelSize_x == 0 || pixelSize_y == 0)
            throw IncorrectSize("Incorrect size for the occupancy grid! Size should not be zero");
        if(resolution == 0)
            throw IncorrectResolution("Resolution provided is incorrect! shoudl be more than zero!");
    }
    catch(IncorrectSize &e){
        std::cerr << e.what() << std::endl;
        exit(2);
    }
    catch(IncorrectResolution &e){
        std::cerr << e.what() << std::endl;
        exit(3);
    }

    if(occupancy_grid.empty()){
        initMap(laser_scan);
    }
    else{
        updateMap(laser_scan);
    }
}

// Initializing occupancy grid map
void OccupancyGrid::initMap(const LaserData laser_scan []){
    if(pt.x == 0 || pt.y == 0)
        pt = cv::Point(pixelSize_x/2, pixelSize_y/2);
    occupancy_grid = cv::Mat(pixelSize_x, pixelSize_y, CV_8UC1, cv::Scalar::all(125));
    int rob_pos_x = pixelSize_y - pt.y;
    int rob_pos_y = pixelSize_x - pt.x;

    for(int i = 0; i < readingsPerScan; i++){
        float range = laser_scan[i].range;
        float theta = laser_scan[i].theta;
        if(range > max_range)
            continue;
        for(float j = resolution; j < (range + (4*sensorCov)); j += resolution){
            int cur_x = rob_pos_y - ((j * sin(theta - curRobAngle))/resolution);
            int cur_y = rob_pos_x - ((j * cos(theta - curRobAngle))/resolution);
            OCCUPANCY_STATUS stat = invSensorModel(range, j);
            if(occupancy_grid.at<uchar>(cv::Point(cur_x, cur_y)) != 125)
                continue;
            if(stat == FREE){
                occupancy_grid.at<uchar>(cv::Point(cur_x, cur_y)) = 255;
            }
            else if(stat == OCCUPIED){
                occupancy_grid.at<uchar>(cv::Point(cur_x, cur_y)) = 0;
            }
            else if(stat == UNSEEN){
                occupancy_grid.at<uchar>(cv::Point(cur_x, cur_y)) = 125;
            }
        }
    }
}

// updating the new map by considering the decay rate
void OccupancyGrid::updateMap(const LaserData laser_scan []){

    int rob_pos_x = pixelSize_y - pt.y;
    int rob_pos_y = pixelSize_x - pt.x;
    
    for(int i = 0; i < readingsPerScan; i++){
        float range = laser_scan[i].range;
        float theta = laser_scan[i].theta;
        if(range > max_range)
            continue;
        for(float j = resolution; j < max_range; j += resolution){
            int cur_x = rob_pos_y - ((j * sin(theta - curRobAngle))/resolution);
            int cur_y = rob_pos_x - ((j * cos(theta - curRobAngle))/resolution);
            OCCUPANCY_STATUS stat = invSensorModel(range, j);
            int cur_pixel = static_cast<int>(occupancy_grid.at<uchar>(cv::Point(cur_x, cur_y)));
            int temp_pixel;
            if(stat == FREE){
                temp_pixel = 255;
            }
            else if(stat == OCCUPIED){
                temp_pixel = 0;
            }
            else if(stat == UNSEEN){
                temp_pixel = 125;
            }
            int new_pixel = temp_pixel - cur_pixel * (1 - decay_rate);
            if(new_pixel < 0) new_pixel = 0;
            else if(new_pixel > 255) new_pixel = 255;
            occupancy_grid.at<uchar>(cv::Point(cur_x, cur_y)) = new_pixel; 
        }
    }
}

// save map at any point
void OccupancyGrid::saveMap(const std::string file_name){
    cv::imwrite(file_name, occupancy_grid);
    std::cout << "Image has been written at location:" << file_name << std::endl;
}

// loading old map and continuing
void OccupancyGrid::loadMap(const std::string file_name){
    oldMap_stat = true;
    occupancy_grid = cv::imread(file_name, cv::IMREAD_GRAYSCALE);
    pixelSize_x = occupancy_grid.rows;
    pixelSize_y = occupancy_grid.cols;
}

// inverse sensor model
OCCUPANCY_STATUS OccupancyGrid::invSensorModel(const float range, const float query_range){
    if(range > (query_range - (sensorCov/2)) && range < (query_range + (sensorCov/2)))
        return OCCUPIED;
    else if(range > (query_range + (sensorCov/2)))
        return FREE;
    else 
        return UNSEEN;
}

// wraping angle 
float OccupancyGrid::wrapAngle(float theta){
            if(theta > PI)
                return (theta-PI);
            else 
                return theta;
        }