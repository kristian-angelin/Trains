/*  File name: train_station.cpp
 *  Author: Kristian Angelin
 *  Date: 2019-05-28
 *  Description: Implementation of class Train_station
 */

#include <algorithm>
#include "train_station.h"

std::ostream& Train_station::print(std::ostream &os) const {
    os << station_name;
    return os;
}

// Search train station for vehicle by id
bool Train_station::get_vehicle_by_type(int type, std::shared_ptr<Vehicle> &vehicle){
    int pos = 0;
    for(auto i: vehicles) {
        if(i->get_vehicle_type() == type) {
            vehicle = std::move(i);
            vehicles.erase(vehicles.begin() + pos);
            return true;
        }
        ++pos;
    }
    return false;
}

// Search train by id and remove it from the station.
bool Train_station::remove_train(int id) {
    int pos = 0;
    for(auto &i: trains) {
        if(i->get_id() == id) {
            trains.erase(trains.begin() + pos);
            return true;
        }
        ++pos;
    }
    return false;
}

// Sort vehicles by id
void Train_station::sort_vehicle_vector() {
    std::sort(vehicles.begin(), vehicles.end(),
            [](std::shared_ptr<Vehicle> const &lhs, std::shared_ptr<Vehicle> const &rhs){
        return lhs->get_vehicle_id() < rhs->get_vehicle_id();
    });
}