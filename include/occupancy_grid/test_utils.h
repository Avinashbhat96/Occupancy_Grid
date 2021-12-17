/**
 * @file test_utils.h
 * This file an utility file to generate some testing data.
 */

#include "laserdata.h"
#include "OccupancyGrid.h"
#include <iostream>
#include <vector>

// create dynamic element for testing
std::vector<float> createDynamic(float dist, float angle_inc, float length){
    float cur_l {0};
    float c_l = dist * sin(angle_inc);
    std::vector<float> distances;
    for(int i {0}; i < (length/c_l); i++){
        distances.push_back(c_l + dist);
    }
    return distances;
}

// create static element for test
std::vector<float> createStatic(float dist, float angle, float angle_inc, float length){
    angle = std::fmod(angle, PI);
    float o_l = dist * sin(angle);
    float a_l = dist * cos(angle);
    std::vector<float> distances, dist_temp;
    float cur_l {0};
    float cur_angle {angle_inc};
    float dist_growing{0};
    for(int i = 0; cur_l < length; i++){
        float dist_c = dist / cos(cur_angle);
        dist_temp.push_back(dist_c);
        cur_angle += angle_inc;
        cur_l += (dist * sin(cur_angle) - dist_growing);
        dist_growing = dist * sin(cur_angle);
    }
    
    int mid_pt = dist_temp.size()/2;
    distances.resize(1+(2*dist_temp.size()));
    distances[(dist_temp.size())+1] = dist;
    for(int i = 0; i < dist_temp.size(); i++){
        distances[dist_temp.size() - i] = dist_temp.at(i);
        distances[dist_temp.size() + i] = dist_temp.at(i);
    }
    return distances;
}

// preparing array of data
float** createData(int num_of_readings, int car_start = 0, int person_start = 0){
    if(car_start==0)
        car_start = (3*num_of_readings/4);
    if(person_start==0)
        person_start = (num_of_readings/4);
    float** data = 0;
    data = new float*[num_of_readings];
    for(int i = 0; i < num_of_readings; i++){
        data[i] = new float[2];
        data[i][1] = ((2*PI) / (float)num_of_readings) * i;
    }

    const float angle_inc = 2 * PI / num_of_readings;

    std::vector<float> person, car, wall, box;
    float person_length {0.5}, car_length {2}, wall_length {4}, box_length{3};
    float person_dist {3}, car_dist {5}, wall_dist {6}, box_dist{4};
    
    person = createDynamic(person_dist, angle_inc, person_length);
    car = createDynamic(car_dist, angle_inc, car_length);
    wall = createStatic(wall_dist, 0, angle_inc, wall_length);
    box = createStatic(box_dist, 3.14, angle_inc, box_length);

    data[0][0] = wall_dist;
    for(int i = 1; i < (wall.size()/2); i++){
        data[i][0] = wall.at((wall.size()/2) + i);
        data[num_of_readings-i][0] = wall.at((wall.size()/2) + i);
    }
    data[num_of_readings/2][0] = box_dist;
    for(int i = 1; i < (box.size()/2); i++){
        data[(num_of_readings/2)+i][0] = box.at((box.size()/2) + i);
        data[(num_of_readings/2)-i][0] = box.at((box.size()/2) + i);
    }
    
    for(int i = 0; i < person.size(); i++){
        data[person_start+i][0] = person.at(i);
    }

    for(int i = 0; i < car.size(); i++){
        data[car_start+i][0] = car.at(i);
    }

    return data;
}

// Generating testing data of type laserdata
LaserData *generateLaserData(int num_of_readings, float for_car=2.6, float for_person=0.7){
    float** base_laser_data = createData(num_of_readings,(for_car*num_of_readings/4), (for_person*num_of_readings/4));
    LaserData *laserdata = new LaserData [num_of_readings];
    for(int i = 0; i < num_of_readings; i++){
        laserdata[i].range = base_laser_data[i][0];
        laserdata[i].theta = base_laser_data[i][1];
    }
    for(int i = 0; i < num_of_readings; i++){
        delete [] base_laser_data[i];
    }
    delete [] base_laser_data;
    return laserdata;
}
