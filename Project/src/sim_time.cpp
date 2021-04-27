/* File name: sim_time.cpp
 * Author: Kristian Angelin
 * Date: 2019-05-26
 * Description: Implementation of class Sim_time.
 */

#include <sstream>
#include <iomanip>
#include "sim_time.h"

Sim_time::Sim_time(double time_in_dec) {
    hour = static_cast<int>(time_in_dec);
    minutes = (time_in_dec - hour) * 60;
}

// Convert time object to 24h clock version
Sim_time Sim_time::to_24h_time() {
    Sim_time time(hour, minutes);
    time.hour = time.hour % 24;
    return time;
}

// Overloading operators
Sim_time& Sim_time::operator++() {
    minutes = (minutes + 1) % 60;
    if(minutes == 0) {
        hour = (hour + 1);
    }
    return *this;
}

Sim_time Sim_time::operator++(int) {
    Sim_time t_temp(*this);
    ++(*this);
    return t_temp;
}

std::ostream &operator<<(std::ostream &os, const Sim_time &time) {
    os << std::setfill('0');
    os << std::setw(2) << time.get_hours() << ':' << std::setw(2) << time.get_minutes();
    os << std::setfill(' ');
    return os;
}

std::istream &operator>>(std::istream &is, Sim_time &time) {
    std::string temp_string;
    is >> temp_string;
    temp_string.replace(2, 1, " ");
    std::istringstream iss(temp_string);
    int temp_int;
    iss >> temp_int;
    time.set_hours(temp_int);
    iss >> temp_int;
    time.set_minutes(temp_int);
    return is;
}

bool operator<(const Sim_time &lhs, const Sim_time &rhs) {
    if(lhs.get_hours() != rhs.get_hours()){
        return lhs.get_hours() < rhs.get_hours();
    }
    else {
        return lhs.get_minutes() < rhs.get_minutes();
    }
}

bool operator<=(const Sim_time &lhs, const Sim_time &rhs) {
    return lhs == rhs || lhs < rhs;
}

bool operator==(const Sim_time &lhs, const Sim_time &rhs) {
    return lhs.get_hours() == rhs.get_hours() &&
            lhs.get_minutes() == rhs.get_minutes();
}

bool operator!=(const Sim_time &lhs, const Sim_time &rhs) {
    return lhs.get_hours() != rhs.get_hours() ||
           lhs.get_minutes() != rhs.get_minutes();
}

Sim_time operator+(const Sim_time &lhs, const Sim_time &rhs) {
    Sim_time time;

    // Calculate total minutes
    int lhs_total = lhs.get_hours()*60 + lhs.get_minutes();
    int rhs_total = rhs.get_hours()*60 + rhs.get_minutes();

    // Set hour and minutes
    time.set_minutes((lhs_total + rhs_total) % 60);
    time.set_hours((lhs_total + rhs_total) / 60);

    return time;
}

Sim_time operator-(const Sim_time &lhs, const Sim_time &rhs) {
    Sim_time time;
    // Calculate total minutes
    int lhs_total = lhs.get_hours()*60 + lhs.get_minutes();
    int rhs_total = rhs.get_hours()*60 + rhs.get_minutes();

    // Set hour and minutes
    time.set_minutes((lhs_total - rhs_total) % 60);
    time.set_hours((lhs_total - rhs_total) / 60);

    return time;
}