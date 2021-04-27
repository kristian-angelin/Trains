/* File name: train.h
 * Author: Kristian Angelin
 * Date: 2019-05-26
 * Description: Definition of Train class. A train runs a specified route in the simulation.
 */

#ifndef DT060G_TRAIN_H
#define DT060G_TRAIN_H

// Forward declaration
class Train_station;

#include <string>
#include <memory>
#include <vector>
#include <ostream>
#include "vehicle.h"
#include "vehicle_engine.h"
#include "sim_time.h"
#include "train_station.h"

class Train {
private:
    // Data members
    int train_number = 0;
    std::string start_station;
    std::string end_station;
    std::shared_ptr<Train_station> current_station; // If nullptr, the train is running.

    Sim_time start_time;
    Sim_time arrival_time;
    Sim_time start_delay;
    Sim_time arrival_delay;

    int max_speed = 0;
    int current_speed = 0;
    std::string current_status = "NOT ASSEMBLED";

    std::vector<int> required_vehicle_types;        // The vehicles needed to complete train.
    std::vector<std::shared_ptr<Vehicle>> vehicles; // Currently attached vechicles

public:
    Train() = default;

    // Get functions
    int get_id() const { return train_number; }
    std::string get_start_station() const { return start_station; }
    std::string get_end_station() const { return end_station; }
    std::shared_ptr<Train_station> get_current_station() const { return current_station; }

    Sim_time get_start_time() const { return start_time; }
    Sim_time get_arrival_time() const { return arrival_time; }
    Sim_time get_start_delay() const { return start_delay; }
    Sim_time get_arrival_delay() const { return arrival_delay; }

    int get_max_speed() const { return max_speed; }
    int get_current_speed() const { return current_speed; }
    int get_engine_max_speed(); // The speed of the slowest vehicle engine attached.

    std::string get_status() const { return current_status; }
    std::vector<int> get_required_vehicle_types() const { return required_vehicle_types; }
    std::vector<std::shared_ptr<Vehicle>> get_train_vehicles() { return vehicles; }

    // Set functions
    void set_id(int id) { train_number = id; }
    void set_start_station(std::string dep_station) { start_station = std::move(dep_station); }
    void set_end_station(std::string dest_station) { end_station = std::move(dest_station); }

    void set_departure_time(Sim_time dep_time) { start_time = dep_time; }
    void set_arrival_time(Sim_time arr_time) { arrival_time = arr_time; }
    void set_start_delay(Sim_time delay) { start_delay = delay; }
    void set_arrival_delay(Sim_time delay) { arrival_delay = delay; }

    void set_current_speed(int speed) { current_speed = speed; }
    void set_max_speed(int speed) { max_speed = speed; }
    void set_status(std::string status) { current_status = std::move(status); }
    void set_current_station(std::shared_ptr<Train_station> station) { current_station = station; }

    void add_train_requirement(int train_type) { required_vehicle_types.push_back(train_type); }
    void add_vehicle(std::shared_ptr<Vehicle> cart) {vehicles.emplace_back(std::move(cart)); }

    // Removes attached vehicles
    void remove_vehicles() { vehicles.clear(); }

    // Overloading operator <<
    friend std::ostream &operator<<(std::ostream &os, const Train &train) { return train.print(os); }
    std::ostream& print(std::ostream &os) const;
};

#endif //DT060G_TRAIN_H
