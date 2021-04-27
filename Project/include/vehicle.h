/*  File name: vehicle.h
 *  Author: Kristian Angelin
 *  Date: 2019-05-22
 *  Description: Definition of abstract class Vehicle and derived Vehicle classes. Vehicles are
 *  building blocks for trains.
 */

#ifndef DT060G_VEHICLE_H
#define DT060G_VEHICLE_H

#include <ostream>
#include <string>
#include <vector>

class Vehicle {
private:
    // Data members
    int vehicle_id;
    int vehicle_type;
    int max_speed = 0;
    std::vector<std::string> event_history;

public:
    // Constructor
    Vehicle (int id, int type)
        : vehicle_id(id), vehicle_type(type) {}
    // Destructor
    virtual ~Vehicle() {}

    // Get functions
    int get_vehicle_id() const { return vehicle_id; }
    int get_vehicle_type() const {return vehicle_type; }
    int get_max_speed() const { return max_speed; }
    std::vector<std::string> get_event_history() const { return event_history; }

    void set_max_speed(int speed) { max_speed = speed; }
    void add_event_to_history(std::string event) { event_history.emplace_back(event); }

    // Virtual functions
    virtual bool has_engine() = 0;  // Pure virtual

    // Operator overloading for printing vehicle info.
    // Helper enables derived classes to override what is printed.
    friend std::ostream &operator<<(std::ostream &os, const Vehicle &vehicle) { return vehicle.print(os); }
    virtual std::ostream& print(std::ostream &os) const { return os; }     // Helper

    // Converts the stored int to a string
    std::string convert_vehicle_type(int type) const;
};

// -----------------------  DERIVED VEHICLE CLASSES  --------------------------


class Coach : public Vehicle {
private:
    // Data members
    int number_of_seats;
    int has_internet;

public:
    // Constructor
    Coach (int id, int type, int seats, int internet)
        : Vehicle(id, type), number_of_seats(seats), has_internet(internet) {}

    // Get functions
    int get_number_of_seats() const { return number_of_seats; }
    int get_coach_internet() const { return has_internet; }

    // Override functions
    bool has_engine() override { return false; }
    std::ostream& print(std::ostream &os) const override; // Print vehicle info
};

class Sleeping_car : public Vehicle {
private:
    // Data member
    int number_of_beds;

public:
    // Constructor
    Sleeping_car (int id, int type, int beds)
        : Vehicle(id, type), number_of_beds(beds) {}

    // Get functions
    int get_number_of_beds() const { return number_of_beds; }

    // Override functions
    bool has_engine() override { return false; }
    std::ostream& print(std::ostream &os) const override; // Print vehicle info
};

class Open_freight_car : public Vehicle {
private:
    // Data members
    int cargo_capacity;     // Cargo capacity in tons.
    int floor_area;         // m^2

public:
    // Constructor
    Open_freight_car (int id, int type, int capacity, int area)
        : Vehicle(id, type), cargo_capacity(capacity), floor_area(area) {}

    // Functions
    int get_cargo_capacity() const { return cargo_capacity; }
    int get_floor_area() const { return floor_area; }

    // Override functions
    bool has_engine() override { return false; }
    std::ostream& print(std::ostream &os) const override; // Print vehicle info
};

class Covered_freight_car : public Vehicle {
private:
    // Data member
    int cargo_capacity;     // m^3

public:
    // Constructor
    Covered_freight_car (int id, int type, int capacity)
        : Vehicle(id, type), cargo_capacity(capacity) {}

    // Functions
    int get_cargo_volume() const { return cargo_capacity; }

    // Override functions
    bool has_engine() override { return false; }
    std::ostream& print(std::ostream &os) const override; // Print vehicle info
};

#endif //DT060G_VEHICLE_H