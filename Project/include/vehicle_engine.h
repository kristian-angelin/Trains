/*  File name: vehicle_engine.h
 *  Author: Kristian Angelin
 *  Date: 2019-05-22
 *  Description: Definition of the derived abstract class Vehicle_engine and derived
 *  Vehicle_engine classes. Engines are vehicles included in trains.
 */

#ifndef DT060G_VEHICLE_ENGINE_H
#define DT060G_VEHICLE_ENGINE_H

#include <ostream>
#include "vehicle.h"

// -----------------------  DERIVED VEHICLE ABSTRACT CLASS  --------------------

class Vehicle_engine : public Vehicle {
public:
    // Constructor
    Vehicle_engine (int id, int type, int speed)
        : Vehicle(id, type) { set_max_speed(speed); }
    // Virtual destructor
    virtual ~Vehicle_engine() {}

    // Pure virtual function
    virtual bool has_engine() = 0;
};

// -----------------------  DERIVED ENGINE CLASSES  ---------------------------

class Electrical_engine : public Vehicle_engine {
private:
    // Data member
    int power_consumption; //  power kW

public:
    // Constructor
    Electrical_engine (int id, int type, int speed, int power_comp)
        : Vehicle_engine(id, type, speed), power_consumption(power_comp) {}

    // Functions
    int get_power_consumption() const { return power_consumption; }

    // Override functions
    bool has_engine() override { return true; }
    std::ostream& print(std::ostream &os) const override; // Print vehicle info
};

class Diesel_engine : public Vehicle_engine {
private:
    // Data member
    int fuel_consumption; // liters/h

public:
    // Constructor
    Diesel_engine (int id, int type, int speed, int fuel_comp)
        : Vehicle_engine(id, type, speed), fuel_consumption(fuel_comp) {}

    // Functions
    int get_fuel_consumption() const { return fuel_consumption; }

    // Override functions
    bool has_engine() override { return true; }
    std::ostream& print(std::ostream &os) const override; // Print vehicle info
};

#endif //DT060G_VEHICLE_ENGINE_H
