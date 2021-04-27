/*  File name: vehicle.cpp
 *  Author: Kristian Angelin
 *  Date: 2019-05-22
 *  Description: Implementation of class Vehicle_engine.
 */

#include "vehicle.h"

// Print vehicle info for coach
std::ostream& Coach::print(std::ostream &os) const {
    os << '[' << convert_vehicle_type(get_vehicle_type()) << ']'
        << " ID: " << get_vehicle_id()
        << ", Seats: " << get_number_of_seats();
    if(this->get_coach_internet() == 1) {
        os << ", Internet on board";
    }
    else {
        os << ", No internet on board";
    }
    return os;
}

// Print vehicle info for sleeping car
std::ostream& Sleeping_car::print(std::ostream &os) const {
    os << '[' << convert_vehicle_type(get_vehicle_type()) << ']'
        << " ID: " << get_vehicle_id()
        << ", Beds: " << get_number_of_beds();
    return os;
}

// Print vehicle info for open freight car
std::ostream& Open_freight_car::print(std::ostream &os) const {
    os << '[' << convert_vehicle_type(get_vehicle_type()) << ']'
        << " ID: " << get_vehicle_id()
        << ", Cargo capacity: " << get_cargo_capacity() << " ton"
        << ", Floor area: " << get_floor_area() << " m^2";
    return os;
}

// Print vehicle info for covered freight car
std::ostream& Covered_freight_car::print(std::ostream &os) const {
    os << '[' << convert_vehicle_type(get_vehicle_type()) << ']'
        << " ID: " << get_vehicle_id()
        << ", Cargo volume: " << get_cargo_volume() << " m^3";
    return os;
}

// Function to convert vehicle type from an int to string
std::string Vehicle::convert_vehicle_type(int type) const {
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