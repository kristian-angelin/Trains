/*  File name: simulation.h
 *  Author: Kristian Angelin
 *  Date: 2019-05-30
 *  Description: Definition of Simulation class. Handles the actual simulation, based
 *  on discrete event-driven simulation.
 */

#ifndef DT060G_SIMULATION_H
#define DT060G_SIMULATION_H

#include <vector>
#include <queue>
#include <memory>
#include "train.h"
#include "train_station.h"
#include "train_event.h"
#include "sim_time.h"
#include "train_route.h"
#include "sim_logger.h"


class Simulation {
private:
    // Data members
    std::vector<std::shared_ptr<Train>> trains;             // All trains
    std::vector<std::shared_ptr<Train_station>> stations;   // All train stations
    std::vector<Train_route> train_map;                  // Train routes

    // Event queue
    std::priority_queue<std::shared_ptr<Train_event>,
                        std::vector<std::shared_ptr<Train_event>>, Event_comparison> event_queue;

    // Time data
    Sim_time start_time = {0, 0};
    Sim_time end_time  = {23, 59};
    Sim_time current_time = {0,0};
    Sim_time interval_time = {0, 10};

    // Constants
    const Sim_time ASSEMBLY_TIME = {0, 30};
    // File names
    const std::string TRAINS_FILE = "Trains.txt";
    const std::string STATIONS_FILE = "TrainStations.txt";
    const std::string MAP_FILE = "TrainMap.txt";

    std::shared_ptr<Sim_logger> logger;

public:
    explicit Simulation(std::shared_ptr<Sim_logger> e_logger);

    Sim_time get_start_time () const { return start_time; }
    Sim_time get_end_time () const { return end_time; }
    Sim_time get_current_time() const { return current_time; }
    Sim_time get_interval_time () const { return interval_time; }

    bool get_train_by_number(int train_number, std::shared_ptr<Train> &train);
    bool get_train_by_vehicle_id(int vehicle_id, std::shared_ptr<Train> &train);
    bool get_train_station(std::string name, std::shared_ptr<Train_station> &station);
    bool get_vehicle_by_id(int id, std::shared_ptr<Vehicle> &vehicle);
    bool get_vehicle_location(int id, std::string &location);

    std::vector<std::string> get_station_names();
    int get_route_distance(std::string start_station, std::string end_station);
    std::vector<std::shared_ptr<Train>> &get_all_trains() { return trains; }

    void set_start_time(Sim_time start_t) { start_time = start_t; }
    void set_end_time(Sim_time end_t) { end_time = end_t; }
    void set_interval_time(Sim_time interval_t) { interval_time = interval_t; }

    void add_event(std::shared_ptr<Train_event> new_event);

    // Run simulation forward
    bool run_sim(Sim_time stop_time, bool flag_silent_run = false); // Silent does not print events.
    bool go_to_next_event();

    void run_next_event(bool flag_silent_run = false); // Silent does not print event.

    // Loading from files
    bool load_trains_from_file();
    bool load_stations_from_file();
    bool load_train_routes_from_file();
    void load_starting_events(std::shared_ptr<Simulation> sim); // Is run after files are loaded.
};

#endif //DT060G_SIMULATION_H
