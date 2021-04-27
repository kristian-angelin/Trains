/* File name: sim_time.h
 * Author: Kristian Angelin
 * Date: 2019-05-26
 * Description: Definition of Simulation_menu class. Handles the time of the simulation.
 */

#ifndef DT060G_SIM_TIME_H
#define DT060G_SIM_TIME_H

#include <iostream>

class Sim_time {
private:
    int hour;
    int minutes;
public:
    Sim_time() : hour(0), minutes(0) {}
    Sim_time(int h, int min) : hour(h), minutes(min){}
    Sim_time(double time_in_dec);

    // Set functions
    void set_hours(int h) { hour = h; }
    void set_minutes(int m) { minutes = m; }

    // Get functions
    int get_hours() const { return hour; }
    int get_minutes() const { return minutes; }

    // Converting functions
    double convert_to_decimal() const { return hour + static_cast<double>(minutes) / 60; }
    Sim_time to_24h_time();

    // Operator overloading
    Sim_time &operator++();
    Sim_time operator++(int);
    friend std::ostream &operator<<(std::ostream &os, const Sim_time &time);
    friend std::istream &operator>>(std::istream &is, Sim_time &time);
    friend bool operator<(const Sim_time &lhs, const Sim_time &rhs);
    friend bool operator<=(const Sim_time &lhs, const Sim_time &rhs);
    friend bool operator==(const Sim_time &lhs, const Sim_time &rhs);
    friend bool operator!=(const Sim_time &lhs, const Sim_time &rhs);
    friend Sim_time operator+(const Sim_time &lhs, const Sim_time &rhs);
    friend Sim_time operator-(const Sim_time &lhs, const Sim_time &rhs);
};

#endif //DT060G_SIM_TIME_H
