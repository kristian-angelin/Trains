/*  File name: train_station.h
 *  Author: Kristian Angelin
 *  Date: 2019-05-28
 *  Description: Definition of Train_station class. Handles a train station and its pools
 *  of vehicles and trains.
 */

#ifndef DT060G_TRAIN_STATION_H
#define DT060G_TRAIN_STATION_H

// Forward declaration
class Train;

#include <vector>
#include <memory>
#include "train.h"
#include "vehicle.h"
#include "vehicle_engine.h"

class Train_station {
private:
    // Data member
    std::string station_name;
    std::vector<std::shared_ptr<Train>> trains; // Trains in the station
    std::vector<std::shared_ptr<Vehicle>> vehicles; // Pool of vehicles in the station.

public:
    // Constructor
    Train_station() = default;

    // Get functions
    std::string get_name() const { return station_name; }
    int get_total_carts() const { return vehicles.size(); }
    std::vector<std::shared_ptr<Vehicle>> get_vehicles() { return vehicles; }
    std::vector<std::shared_ptr<Train>> get_trains() { return trains; }
    bool get_vehicle_by_type(int type, std::shared_ptr<Vehicle> &vehicle);

    // Set functions
    void set_name(std::string name) { station_name = std::move(name); }

    // Functions for adding objects
    void add_train(std::shared_ptr<Train> train) { trains.push_back(train); }
    void add_vehicle(std::shared_ptr<Vehicle> vehicle) { vehicles.emplace_back(std::move(vehicle)); }

    bool remove_train(int id);

    // Sort vehicles by id
    void sort_vehicle_vector();

    // Operator overloading for printing train station info.
    friend std::ostream &operator<<(std::ostream &os, const Train_station &station) { return station.print(os); }
    std::ostream& print(std::ostream &os) const;
};


#endif //DT060G_TRAIN_STATION_H
