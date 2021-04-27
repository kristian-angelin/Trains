/*  File name: train_route.h
 *  Author: Kristian Angelin
 *  Date: 2019-06-03
 *  Description: Definition of train_route class. Stores a train route.
 */

#ifndef DT060G_TRAIN_ROUTE_H
#define DT060G_TRAIN_ROUTE_H

#include <string>

class Train_route {
private:
    std::string start_station;
    std::string end_station;
    int distance = 0; // in km
public:
    Train_route() = default;

    // Set functions
    void set_start_station(std::string start) { start_station = std::move(start); }
    void set_end_station(std::string end) { end_station = std::move(end); }
    void set_distance(int dist) { distance = dist; }

    // Get functions
    std::string get_start_station() const { return start_station; }
    std::string get_end_station() const { return end_station; }
    int get_distance() const { return distance; }

    // Compare two Train_route objects.
    friend bool operator==(const Train_route &lhs, const Train_route &rhs) {
        return rhs.get_start_station() == lhs.get_start_station() &&
               rhs.get_end_station() == lhs.get_end_station();
    }
};


#endif //DT060G_TRAIN_ROUTE_H
