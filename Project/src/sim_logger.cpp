/*  File name: log_event.cpp
 *  Author: Kristian Angelin
 *  Date: 2019-06-01
 *  Description: Implementation of class Sim_logger.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "sim_logger.h"

// Constructor
Sim_logger::Sim_logger() {
    // Clear the contents of the logfile.
    std::ofstream out_file;
    out_file.open(LOG_FILE, std::ios_base::trunc);
    out_file.close();
}

// When a even occurs
void Sim_logger::log_event(std::shared_ptr<Train_event> event) {
    print_event_info(event);
    save_event_to_log_file(event);
}

// Print event message to screen.
void Sim_logger::print_event_info(std::shared_ptr<Train_event> event) {
    std::cout << print_train_event(event).str();
}

// Save event message to file.
void Sim_logger::save_event_to_log_file(std::shared_ptr<Train_event> event) {
    // Temporary set log level to high to keep log file content consistent.
    std::string log_copy = log_level;
    log_level = "HIGH";
    std::ofstream out_file;
    out_file.open(LOG_FILE, std::ios_base::app);
    out_file << print_train_event(event).str();
    out_file.close();
    log_level = log_copy;
}

// Print event info to screen.
std::stringstream Sim_logger::print_train_event(std::shared_ptr<Train_event> event) {
    std::stringstream out_stream;
    if(log_level == "LOW"){
        out_stream << event->get_event_time().to_24h_time() << " " << *event->get_train();
    }
    else if(log_level == "HIGH") {
        out_stream << event->get_event_time().to_24h_time() << " " << *event->get_train()
                   << "Speed: " << event->get_train()->get_current_speed() << " km/h" << std::endl
                   << print_event_message(event).str();
    }
    out_stream << std::endl << std::endl;
    return out_stream;
}

// Print train info by log level
std::stringstream Sim_logger::print_train(std::shared_ptr<Train> train) {
    std::stringstream out_stream;
    if(log_level == "LOW") {
        out_stream << *train << std::endl;
    }
    else if(log_level == "HIGH") {
        out_stream << *train << std::endl << std::endl
                    << print_train_vehicles(train).str();
    }
    return out_stream;
}

// Print info about a trains attached vehicles by log level
std::stringstream Sim_logger::print_train_vehicles(std::shared_ptr<Train> train) {
    std::stringstream out_stream;
    out_stream << "Connected vehicles:" << std::endl;
    if(!train->get_train_vehicles().empty()) {
        for(auto &vehicle: train->get_train_vehicles()) {
            out_stream << *vehicle << std::endl;
        }
    }
    else {
        out_stream << "[NO VEHICLES}";
    }
    return out_stream;
}

// Print vehicles info by log level
std::stringstream Sim_logger::print_vehicle(std::shared_ptr<Vehicle> vehicle) {
    std::stringstream out_stream;
    if(log_level == "LOW") {
        out_stream <<  '[' << vehicle->convert_vehicle_type(vehicle->get_vehicle_type()) << ']'
                      << " ID: " << vehicle->get_vehicle_id();
    }
    else if(log_level == "HIGH") {
        out_stream << *vehicle;
    }
    return out_stream;
}

// Print historical events of a vehicle
std::stringstream Sim_logger::print_vehicle_history(std::shared_ptr<Vehicle> vehicle) {
    std::stringstream out_stream;
    out_stream << *vehicle << std::endl;
    if(!vehicle->get_event_history().empty()) {
        for(auto &event: vehicle->get_event_history()) {
            out_stream << event << std::endl;
        }
    }
    else {
        out_stream << "[NO HISTORY}";
    }
    return out_stream;
}
// Print station info by log level
std::stringstream Sim_logger::print_station(std::shared_ptr<Train_station> station) {
    std::stringstream out_stream;
    if(log_level == "LOW") {
        out_stream << *station << std::endl
                    << "Trains: " << station->get_trains().size() << std::endl
                    << "Vehicle pool: " << station->get_vehicles().size();
    }
    else if(log_level == "HIGH") {
        out_stream << *station << std::endl << std::endl
                    << "Trains: " << station->get_trains().size() << std::endl
                    << "Vehicle pool: " << station->get_vehicles().size() << std::endl << std::endl
                    << print_station_trains(station).str() << std::endl
                    << print_station_vehicles(station).str();
    }
    return out_stream;
}

// Print info about trains at a station.
std::stringstream Sim_logger::print_station_trains(std::shared_ptr<Train_station> station) {
    std::stringstream out_stream;
    out_stream << "Trains at station: " << std::endl;
    if(!station->get_trains().empty()) {
        for(auto &s: station->get_trains()) {
            out_stream << *s << std::endl;
        }
    }
    else {
        out_stream << "[NO TRAINS!}";
    }
    return out_stream;
}

// Print info about a stations vehicles by log level
std::stringstream Sim_logger::print_station_vehicles(std::shared_ptr<Train_station> station) {
    std::stringstream out_stream;
    out_stream << "Vehicles in station pool: " << std::endl;
    if(!station->get_vehicles().empty()) {
        for(auto &v: station->get_vehicles()) {
            out_stream << *v << std::endl;
        }
    }
    else {
        out_stream << "[NO VEHICLES!}";
    }
    return out_stream;
}

// Event message depending on the event type.
std::stringstream Sim_logger::print_event_message(std::shared_ptr<Train_event> event) {
    std::stringstream message;
    std::string event_type = event->get_train()->get_status();
    if(event_type == "NOT ASSEMBLED"){
        message << "Not assembled, assembly at "
                << event->get_next_event_time(event_type).to_24h_time();
    }
    else if(event_type == "INCOMPLETE") {
        message << "Now incomplete, next try at "
                << event->get_next_event_time(event_type).to_24h_time();
    }
    else if(event_type == "ASSEMBLED") {
        message << "Now assembled, arriving at the platform "
                << event->get_next_event_time(event_type).to_24h_time();
    }
    else if(event_type == "READY") {
        message << "Now at the platform, departing at "
                << event->get_next_event_time(event_type).to_24h_time();
    }
    else if(event_type == "RUNNING") {
        message << "Has left the platform, traveling at speed "
                << event->get_train()->get_current_speed() << " km/h";
    }
    else if(event_type == "ARRIVED") {
        message << "Has arrived at the platform, disassembly at "
                << event->get_next_event_time(event_type).to_24h_time();
    }
    else if(event_type == "FINISHED") {
        message << "Is now disassembled";
    }
    return message;
}

// Print statistics
std::stringstream Sim_logger::print_statistics(std::vector<std::shared_ptr<Train>> &trains) {
    std::stringstream out_stream;
    out_stream << print_summary(trains).str() << std::endl << std::endl
                << print_trains_on_time(trains).str() << std::endl << std::endl
                << print_delayed_trains(trains).str() << std::endl << std::endl
                << print_non_departing_trains(trains).str();
    return out_stream;
}

// Print overall summary of the simulation results.
std::stringstream Sim_logger::print_summary(std::vector<std::shared_ptr<Train>> &trains) {
    std::stringstream out_stream;
    Sim_time total_departure_delay;
    Sim_time total_arrival_delay;
    int on_time = 0;
    int never_left = 0;
    int departed_late = 0;
    int arrived_late = 0;
    // Go through all trains and summarize the stats.
    for(auto &t : trains) {
        // Stats for a train that arrived at its destination
        if(t->get_status() == "FINISHED") {
            if(Sim_time(0,0) == t->get_arrival_delay()) {
                ++on_time;
            }
            if(Sim_time(0,0) < t->get_arrival_delay()) {
                ++arrived_late;
                total_arrival_delay = total_arrival_delay + t->get_arrival_delay();
            }
            if(Sim_time(0,0) < t->get_start_delay()) {
                ++departed_late;
                total_departure_delay = total_departure_delay + t->get_start_delay();
            }
        }
        // Stats for a train that never arrived at its destination
        if(t->get_status() == "INCOMPLETE" || t->get_status() == "ASSEMBLED" ||
            t->get_status() == "READY" || t->get_status() == "NOT ASSEMBLED") {
            ++never_left;
        }
    }
    out_stream << "***** [Summary of trains] *****" << std::endl
            << "Arrived on time: " << on_time << std::endl
            << "Never left the station: " << never_left << std::endl
            << "Departed late: " << departed_late << std::endl
            << "Arrived late: " << arrived_late << std::endl
            << "Total departure delay: " << total_departure_delay << std::endl
            << "Total arrival delay: " << total_arrival_delay << std::endl;
    return out_stream;
}

std::stringstream Sim_logger::print_trains_on_time(std::vector<std::shared_ptr<Train>> &trains) {
    std::stringstream out_stream;
    out_stream << "***** [TRAINS THAT ARRIVED ON TIME] *****" << std::endl << std::endl;
    for(auto &t: trains){
        if(t->get_status() == "FINISHED") {
            if(Sim_time(0,0) == t->get_arrival_delay()) {
                out_stream << *t << std::endl;
            }
        }
    }
    return out_stream;
}

std::stringstream Sim_logger::print_delayed_trains(std::vector<std::shared_ptr<Train>> &trains) {
    std::stringstream out_stream;
    out_stream << "***** [TRAINS THAT ARRIVED LATE] *****" << std::endl << std::endl;
    for(auto &t: trains){
        if(t->get_status() == "FINISHED") {
            if(Sim_time(0,0) < t->get_arrival_delay()) {
                out_stream << *t << std::endl;
            }
        }
    }
    return out_stream;
}

// Print info about trains that never departed and its missing vehicles.
std::stringstream Sim_logger::print_non_departing_trains(std::vector<std::shared_ptr<Train>> &trains) {
    std::stringstream out_stream;
    out_stream <<  "***** [TRAINS THAT NEVER DEPARTED] *****" << std::endl << std::endl;
    for(auto &t: trains){
        if(t->get_status() != "FINISHED") {
            out_stream << *t << std::endl
                        << std::setw(5) << " " <<  "Vehicles:" << std::endl;
            std::vector<int> req_vehicles = t->get_required_vehicle_types();
            for(auto &v : t->get_train_vehicles()) {
                out_stream << std::setw(10) << " " << *v << std::endl;

                auto it = std::find_if(req_vehicles.begin(), req_vehicles.end(), [v](int num){
                    return num == v->get_vehicle_type();
                });
                req_vehicles.erase(it);
            }
            out_stream << std::setw(5) << " " <<  "Missing vehicles:" << std::endl;
            for(auto &v : req_vehicles) {
                out_stream << std::setw(10) << " " << convert_int_to_vehicle_type(v) << std::endl;
            }
            out_stream << std::endl;
        }
    }
    return out_stream;
}

// Overloaded functions to log vehicle events in vehicle history.
void Sim_logger::log_vehicle_event(Sim_time event_time, std::shared_ptr<Vehicle> vehicle, std::shared_ptr<Train> train) {
    std::stringstream ss;
    ss << event_time;
    std::string event = ss.str() + " - Connected to train " + std::to_string(train->get_id());
    vehicle->add_event_to_history(event);

}
void Sim_logger::log_vehicle_event(Sim_time event_time, std::shared_ptr<Vehicle> vehicle, std::shared_ptr<Train_station> station) {
    std::stringstream ss;
    ss << event_time;
    std::string event = ss.str() + " - Disconnected from train and added to vehicle pool at " + station->get_name();
    vehicle->add_event_to_history(event);
}

// Take int to return vehicle type as string.
std::string Sim_logger::convert_int_to_vehicle_type(int type) {
    switch (type) {
        case 0:
            return "Coach";
        case 1:
            return "Sleeping car";
        case 2:
            return "Open freight car";
        case 3:
            return "Covered freight car";
        case 4:
            return "Electrical engine";
        case 5:
            return "Diesel engine";
        default:
            return "NULL";
    }
}