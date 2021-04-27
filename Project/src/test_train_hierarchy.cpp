/* File name: test_train_hierarchy.cpp
 * Author: Kristian Angelin
 * Date: 2019-05-26
 * Description:  Program for testing train hierarchy for project trains.
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "test_train_hierarchy.h"
#include "sim_time.h"
#include "vehicle_engine.h"
#include "train.h"

void Test_train_hierarchy::run() {
    if(load_trains()){
        print_train_list();
    }
    else {
        std::cout << "COULD NOT LOAD FILE!!!" << std::endl;
    }
}

bool Test_train_hierarchy::load_trains() {
    std::ifstream open_file("Trains.txt");
    if(open_file.is_open()) {
        int temp_int;
        std::string temp_string;
        Sim_time temp_time;

        while (!open_file.eof()) {
            open_file >> temp_int;
            Train temp_train;
            temp_train.set_id(temp_int);
            open_file >> temp_string;
            temp_train.set_start_station(temp_string);
            open_file >> temp_string;
            temp_train.set_end_station(temp_string);
            open_file >> temp_time;
            temp_train.set_departure_time(temp_time);
            open_file >> temp_time;
            temp_train.set_arrival_time(temp_time);
            open_file >> temp_int;
            temp_train.set_max_speed(temp_int);

            // Load the train cart types
            std::istringstream iss;
            std::getline(open_file, temp_string);
            iss.str(temp_string);
            while (!iss.eof()) {
                iss >> temp_int;
                temp_train.add_train_requirement(temp_int);
            }
            trains.push_back(std::move(temp_train));

        }
        open_file.close();
        return true;
    }
    else {
        return false;
    }
}

void Test_train_hierarchy::print_train_list() {
    std::cout << "Train vector size: " << trains.size() << std::endl;
    std::vector<int> req_train_carts;
    for(auto const &i: trains) {
        std::cout << i.get_id() << " " << i.get_start_station() << " "
                    << i.get_end_station() << " " << i.get_start_time() << " "
                    << i.get_arrival_time() << " " << i.get_max_speed() << " ";
        req_train_carts = i.get_required_vehicle_types();
        auto start = req_train_carts.begin();
        auto stop = req_train_carts.end();
        for(auto it = start; it!= stop; ++it) {
            std::cout << (*it) << " ";
        }
        std::cout << std::endl;
    }
}