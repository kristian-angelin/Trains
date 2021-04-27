/*  File name: vehicle_engine.cpp
 *  Author: Kristian Angelin
 *  Date: 2019-05-22
 *  Description: Implementation of class Vehicle_engine.
 */

#include "vehicle_engine.h"

// Print vehicle info for electrical engine
std::ostream& Electrical_engine::print(std::ostream &os) const {
    os << '[' << convert_vehicle_type(get_vehicle_type()) << ']'
        << " ID: " << get_vehicle_id()
        << ", Max speed: " << get_max_speed() << " km/h"
        << ", Fuel consumption: " << get_power_consumption() << " kw";
    return os;
}

// Print vehicle info for diesel engine
std::ostream& Diesel_engine::print(std::ostream &os) const {
    os << '[' << convert_vehicle_type(get_vehicle_type()) << ']'
        << " ID: " << get_vehicle_id()
        << ", Max speed: " << get_max_speed() << " km/h"
        << ", Power: " << get_fuel_consumption() << " l/h";
    return os;
}