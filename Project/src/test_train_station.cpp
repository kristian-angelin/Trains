/*  File name: test_train_station.cpp
 *  Author: Kristian Angelin
 *  Date: 2019-05-28
 *  Description: 
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "test_train_station.h"

void Test_train_station::run() {
    if(load_stations()) {
        print_stations();
    }
    else {
        std::cout << "COULD NOT LOAD STATIONS!" << std::endl;
    }
}

bool Test_train_station::load_stations() {
    std::ifstream open_file("TrainStations.txt");
    if(open_file.is_open()) {
        std::string temp_string;

        while(!open_file.eof()) {
            open_file >> temp_string;
            Train_station temp_station;
            temp_station.set_name(temp_string);
            std::getline(open_file, temp_string);

            std::string start_delim = "(";
            std::string end_delim = ")";

            size_t start_pos = temp_string.find(start_delim);
            size_t end_pos = temp_string.find(end_delim);

            while(start_pos != std::string::npos) {
                std::string train_carts;
                train_carts.assign(temp_string, start_pos + 1, (end_pos - start_pos - 1));
                std::istringstream iss;
                iss.str(train_carts);
                int temp_id;
                int temp_type;
                int temp_param_0;
                int temp_param_1;
                while (!iss.eof()) {
                    iss >> temp_id >> temp_type;
                    if(temp_type == 0) {
                        iss >> temp_param_0 >> temp_param_1;
                        temp_station.add_vehicle(std::unique_ptr<Vehicle>
                                                         (new Coach(temp_id, temp_type, temp_param_0,
                                                                    temp_param_1)));
                    }
                    else if(temp_type == 1) {
                        iss >> temp_param_0;
                        temp_station.add_vehicle(std::unique_ptr<Vehicle>
                                                         (new Sleeping_car(temp_id, temp_type, temp_param_0)));
                    }
                    else if(temp_type == 2) {
                        iss >> temp_param_0 >> temp_param_1;
                        temp_station.add_vehicle(std::unique_ptr<Vehicle>
                                                         (new Open_freight_car(temp_id, temp_type, temp_param_0,
                                                                               temp_param_1)));
                    }
                    else if(temp_type == 3) {
                        iss >> temp_param_0;
                        temp_station.add_vehicle(std::unique_ptr<Vehicle>
                                                         (new Covered_freight_car(temp_id, temp_type,
                                                                                  temp_param_0)));
                    }
                    else if(temp_type == 4) {
                        iss >> temp_param_0 >> temp_param_1;
                        temp_station.add_vehicle(std::unique_ptr<Vehicle>
                                                         (new Electrical_engine(temp_id, temp_type, temp_param_0,
                                                                                temp_param_1)));
                    }
                    else if(temp_type == 5) {
                        iss >> temp_param_0 >> temp_param_1;
                        temp_station.add_vehicle(std::unique_ptr<Vehicle>
                                                         (new Diesel_engine(temp_id, temp_type, temp_param_0,
                                                                            temp_param_1)));
                    }
                    else {
                        std::cout << "ERROR! Couldn't read the file correctly!" << std::endl;
                    }
                }
                start_pos = temp_string.find(start_delim, ++start_pos);
                end_pos = temp_string.find(end_delim, start_pos);
            }
            stations.push_back(std::move(temp_station));
        }
        return true;
    }
    else {
        return false;
    }
}

void Test_train_station::print_stations() {
    std::cout << "Station vector size: " << stations.size() << std::endl;
    for(auto const &i: stations) {
        std::cout << i.get_name() << " = " << i.get_total_carts() << std::endl;
       // i.print_vehicles();
    }
}