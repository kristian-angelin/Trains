/*  File name: simulation.cpp
 *  Author: Kristian Angelin
 *  Date: 2019-05-30
 *  Description: Implementation of class Simulation.
 */

#include <sstream>
#include <fstream>
#include <algorithm>
#include "simulation.h"

// Constructor
Simulation::Simulation(std::shared_ptr<Sim_logger> e_logger) :
    logger(e_logger) {}

void Simulation::add_event(std::shared_ptr <Train_event> new_event) {
    event_queue.emplace(new_event);
}

// Run simulation until stop time.
bool Simulation::run_sim(Sim_time stop_time, bool flag_silent_run) {
    // Make sure sim doesn't run longer then end time.
    if(end_time < stop_time) {
        stop_time = end_time;
    }
    while(current_time < stop_time) {
        // Check if next event is due and the queue is not empty.
        // If we encounter an event, run all events of current time.
        while(current_time == event_queue.top()->get_event_time() && !event_queue.empty()){
            run_next_event(flag_silent_run);
        }
        ++current_time;
    }
    // If sim is at the end, keep running until all running trains have arrived and disassembled.
    if(current_time == end_time) {
        while(!event_queue.empty()) {
            while(current_time == event_queue.top()->get_event_time() && !event_queue.empty()) {
                std::string status = event_queue.top()->get_train()->get_status();
                // Run event with train status RUNNING or ARRIVED. Otherwise remove event.
                if (status == "RUNNING" || status == "ARRIVED") {
                    run_next_event(flag_silent_run);
                    end_time = current_time;
                } else {
                    event_queue.pop();
                }
            }
            ++current_time;
        }
        return false;
    }
    return true;
}

// Find the next event and run sim to event.
bool Simulation::go_to_next_event() {
    Sim_time stop_time = event_queue.top()->get_event_time();
    if(stop_time <= end_time) {
        while(current_time < stop_time) {
            ++current_time;
        }
        run_next_event();
        return true;
    }
    return false;
}

// Run the next event in event queue.
void Simulation::run_next_event(bool flag_silent_run) {
    std::shared_ptr<Train_event> temp_event = event_queue.top();
    event_queue.pop();
    std::shared_ptr<Train> temp_train;
    temp_event->process_event();
    if(!flag_silent_run) {
        if (get_train_by_number(temp_event->get_train_id(), temp_train)) {
            logger->log_event(temp_event);
        } else {
            std::cout << "[ERROR]: Could not print train info!" << std::endl;
        }
    }
}

// Load stations and store in stations vector.
bool Simulation::load_stations_from_file() {
    std::ifstream open_file(STATIONS_FILE);
    if(open_file.is_open()) {
        std::string temp_string;

        // For each station, load all vehicles
        while(!open_file.eof()) {
            open_file >> temp_string;
            std::shared_ptr<Train_station> temp_station = std::make_shared<Train_station>();
            temp_station->set_name(temp_string);
            std::getline(open_file, temp_string);

            // Delims for reading vehicles from file.
            const std::string start_delim = "(";
            const std::string end_delim = ")";

            size_t start_pos = temp_string.find(start_delim);
            size_t end_pos = temp_string.find(end_delim);

            // For each train station, load vehicles from file and add to station.
            while(start_pos != std::string::npos) {
                std::string train_carts;
                train_carts.assign(temp_string, start_pos + 1, (end_pos - start_pos - 1));
                std::istringstream iss;
                iss.str(train_carts);
                int temp_id;
                int temp_type;
                int temp_param_0;
                int temp_param_1;

                // Load all vehicles and create object of correct type.
                while (!iss.eof()) {
                    iss >> temp_id >> temp_type;
                    if(temp_type == 0) {
                        iss >> temp_param_0 >> temp_param_1;
                        temp_station->add_vehicle(std::shared_ptr<Vehicle>
                                                          (new Coach(temp_id, temp_type, temp_param_0,
                                                                     temp_param_1)));
                    }
                    else if(temp_type == 1) {
                        iss >> temp_param_0;
                        temp_station->add_vehicle(std::shared_ptr<Vehicle>
                                                          (new Sleeping_car(temp_id, temp_type, temp_param_0)));
                    }
                    else if(temp_type == 2) {
                        iss >> temp_param_0 >> temp_param_1;
                        temp_station->add_vehicle(std::shared_ptr<Vehicle>
                                                          (new Open_freight_car(temp_id, temp_type, temp_param_0,
                                                                                temp_param_1)));
                    }
                    else if(temp_type == 3) {
                        iss >> temp_param_0;
                        temp_station->add_vehicle(std::shared_ptr<Vehicle>
                                                          (new Covered_freight_car(temp_id, temp_type,
                                                                                   temp_param_0)));
                    }
                    else if(temp_type == 4) {
                        iss >> temp_param_0 >> temp_param_1;
                        temp_station->add_vehicle(std::shared_ptr<Vehicle>
                                                          (new Electrical_engine(temp_id, temp_type, temp_param_0,
                                                                                 temp_param_1)));
                    }
                    else if(temp_type == 5) {
                        iss >> temp_param_0 >> temp_param_1;
                        temp_station->add_vehicle(std::shared_ptr<Vehicle>
                                                          (new Diesel_engine(temp_id, temp_type, temp_param_0,
                                                                             temp_param_1)));
                    }
                }
                start_pos = temp_string.find(start_delim, ++start_pos);
                end_pos = temp_string.find(end_delim, start_pos);
            }
            // Add vehicle to station vehicle pool
            stations.push_back(std::move(temp_station));
        }
        return true;
    }
    else {
        return false;
    }
}

// Load trains and store in trains vector.
bool Simulation::load_trains_from_file() {
    std::ifstream open_file(TRAINS_FILE);
    if(open_file.is_open()) {
        int temp_int;
        std::string temp_string;
        Sim_time temp_time;
        // For each train, load train data from file
        while (!open_file.eof()) {
            open_file >> temp_int;
            std::shared_ptr<Train> temp_train = std::make_shared<Train>();
            temp_train->set_id(temp_int);
            open_file >> temp_string;
            temp_train->set_start_station(temp_string);
            open_file >> temp_string;
            temp_train->set_end_station(temp_string);
            open_file >> temp_time;
            temp_train->set_departure_time(temp_time);
            open_file >> temp_time;
            temp_train->set_arrival_time(temp_time);
            open_file >> temp_int;
            temp_train->set_max_speed(temp_int);

            // Load the train cart types
            std::istringstream iss;
            std::getline(open_file, temp_string);
            iss.str(temp_string);
            while (!iss.eof()) {
                iss >> temp_int;
                temp_train->add_train_requirement(temp_int);
            }
            // Search for the right starting station and add to
            // its list of trains.
            for(auto &i: stations) {
                if(i->get_name() == temp_train->get_start_station()) {
                    temp_train->set_current_station(i);
                    i->add_train(temp_train);
                }
            }
            trains.push_back(temp_train);
            temp_train.reset();
        }
        open_file.close();
        return true;
    }
    else {
        return false;
    }
}

// Load train routes and store in train map.
bool Simulation::load_train_routes_from_file() {
    std::ifstream open_file(MAP_FILE);
    if(open_file.is_open()) {

        Train_route route;
        Train_route reverse_route;
        std::string temp_string;
        int temp_int;
        // For each train route, load route data from file and add to map.
        while (!open_file.eof()) {

            open_file >> temp_string;
            route.set_start_station(temp_string);
            reverse_route.set_end_station(temp_string);

            open_file >> temp_string;
            route.set_end_station(temp_string);
            reverse_route.set_start_station(temp_string);

            open_file >> temp_int;
            route.set_distance(temp_int);
            reverse_route.set_distance(temp_int);

            train_map.emplace_back(route);
            train_map.emplace_back(reverse_route);
        }
        open_file.close();
        return true;
    }
    else {
        return false;
    }
}

// Load all events where the trains make first assembly try
void Simulation::load_starting_events(std::shared_ptr<Simulation> sim) {
    for(auto const &i: trains) {
        std::shared_ptr<Train_event> temp
                (std::make_shared<Assemble_event>((i->get_start_time() - ASSEMBLY_TIME),
                        i, sim, logger));
        event_queue.emplace(temp);
    }
}

// Search for train
bool Simulation::get_train_by_number(int train_number, std::shared_ptr<Train> &train) {
    for(auto &i: trains) {
        if(train_number == i->get_id()) {
            train = i;
            return true;
        }
    }
    return false;
}

// Search for train by entering a vehicle id to se what train its attached to.
bool Simulation::get_train_by_vehicle_id(int vehicle_id, std::shared_ptr<Train> &train) {
    for(auto &t: trains) {
        for(auto &v: t->get_train_vehicles()) {
            if(vehicle_id == v->get_vehicle_id()) {
                train = t;
                return true;
            }
        }
    }
    return false;
}

// Search for station
bool Simulation::get_train_station(std::string name, std::shared_ptr<Train_station> &station) {
    for(auto &i: stations) {
        if(name == i->get_name()){
            station = i;
            return true;
        }
    }
    return false;
}

// Get all station names.
std::vector<std::string> Simulation::get_station_names() {
    std::vector<std::string> names;
    for(auto &i: stations) {
        names.emplace_back(i->get_name());
    }
    return names;
}

// Check the distance of train route in train map.
int Simulation::get_route_distance(std::string start_station, std::string end_station) {
    int distance = 0;
    for(auto &i : train_map) {
        if(i.get_start_station() == start_station) {
            if(i.get_end_station() == end_station) {
                distance = i.get_distance();
            }
        }
    }
    return distance;
}

// Search for vehicle in stations and trains.
bool Simulation::get_vehicle_by_id(int id, std::shared_ptr<Vehicle> &vehicle) {
    for(auto s: stations){
        for(auto v: s->get_vehicles()) {
            if(v->get_vehicle_id() == id) {
                vehicle = v;
                return true;
            }
        }
    }
    for(auto t: trains){
        for(auto v: t->get_train_vehicles()) {
            if(v->get_vehicle_id() == id) {
                vehicle = v;
                return true;
            }
        }
    }
    return false;
}

// Search for vehicle and check its location.
bool Simulation::get_vehicle_location(int id, std::string &location) {
    for(auto s: stations){
        for(auto v: s->get_vehicles()) {
            if(v->get_vehicle_id() == id) {
                std::stringstream ss;
                ss << *v;
                location = ss.str() + " [Location: " + s->get_name() + " station]";
                return true;
            }
        }
    }
    for(auto t: trains){
        for(auto v: t->get_train_vehicles()) {
            if(v->get_vehicle_id() == id) {
                std::stringstream ss;
                ss << *v;
                location = ss.str() + " [Location: attached to train " +
                            std::to_string(t->get_id()) + "]";
                return true;
            }
        }
    }
    return false;
}