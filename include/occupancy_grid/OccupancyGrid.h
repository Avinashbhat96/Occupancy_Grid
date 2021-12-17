/**
 * @file OccupancyGrid.h
 * Occupancy Grid generator header file
 */
#pragma once
#include "laserdata.h"
#include <opencv2/core.hpp>

#define PI 3.14

// Represents the grid status
enum OCCUPANCY_STATUS {FREE, OCCUPIED, UNSEEN};

/**
 * @brief 
 * This class creates an occupancy grid for the given parameters and updates the grid
 * whenever new set of data is being passed
 */

class OccupancyGrid{
    
    private:
        int readingsPerScan {40}; 
        int pixelSize_x, pixelSize_y;
        float resolution;
        float sensorCov {0.2};
        bool oldMap_stat;
        float increment_angle = 2*PI/readingsPerScan;
        cv::Point pt;
        float curRobAngle {0};
        float decay_rate {1};
        float max_range {8};
    
    public:
        OccupancyGrid(): pixelSize_x{0}, pixelSize_y{0}, 
                        resolution{0}, oldMap_stat{false}, pt{cv::Point(0,0)}{}

        OccupancyGrid(int pixel_x, int pixel_y, float _res):
                        pixelSize_x{pixel_x}, pixelSize_y{pixel_y}, 
                        resolution{_res}{}

        ~OccupancyGrid()=default;
        
        // set grid size
        void setSize(const int _x, const int _y){
            pixelSize_x = _x;
            pixelSize_y = _y;
        }

        // set grid cell resolution
        void setResolution(const float _res){
            resolution = _res;
        }
        
        inline float getResolution() const {
            return resolution;
        }

        // set sensor maximum range
        void setSensorRange(const float _range){
            max_range = _range;
        }

        inline float getSensorRange() const {
            return max_range;
        }

        // Set decay rate for updating the map
        void setDecayRate(const float _dr){
            decay_rate = _dr;
        }

        inline float getDecayRate() const {
            return decay_rate;
        }

    public:

        /**
         * @brief 
         * sensor callback for updating the occupancy grid
         * @param laser_scan : data from the sensor model
         */
        void sensorCallback(const LaserData laser_scan []);

        /**
         * @brief 
         * saving occupancy grid to .png format
         * @param file_name : file name to save the occupancy grid
         */
        void saveMap(const std::string file_name);

        /**
         * @brief 
         * loading already existed map
         * @param file_name : load the map from the existing map
         */
        void loadMap(const std::string file_name);

        /**
         * @brief Set the Reading Per Scan object
         * set readings per each scan from the sensor
         * @param _count : number of the scans 
         */
        void setReadingPerScan(int _count){
            readingsPerScan = _count;
            increment_angle = 2 * PI / readingsPerScan;
        }

        /**
         * @brief 
         * for updating the robot pose, if required
         * @param _pt: new point of the robot
         */
        void updateRoboPose(const cv::Point &_pt){
            pt = _pt;
        }

        /**
         * @brief Set the Inc Angle object
         * set increment angle, optional
         * @param _inc : increment angle
         */
        void setIncAngle(float _inc){
            increment_angle = _inc;
        }

        float getIncAngle() const {
            return increment_angle;
        }

        /**
         * @brief Set the Robo Init Pose object
         * set initial pose of the robot, optional
         * @param _pt : starting point of the robot
         */
        void setRoboInitPose(const cv::Point &_pt){
            pt = _pt;
        }

    protected:
        cv::Mat occupancy_grid, probability_grid;

        // initializing the map for the first time
        void initMap(const LaserData laser_scan []);

        // updating map based on the decay rate
        void updateMap(const LaserData laser_scan []);

        // sensor inverse model for checking the status of the grid
        OCCUPANCY_STATUS invSensorModel(const float range, const float query_range);
        
        // wrapping angle between -pi and pi
        float wrapAngle(float theta);
};
