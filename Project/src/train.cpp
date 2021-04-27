/* File name: train.cpp
 * Author: Kristian Angelin
 * Date: 2019-05-26
 * Description: Implementation of class Train.
 */

#include "train.h"

// Print train info
std::ostream& Train::print(std::ostream &os) const {
    os << "Train " << get_id() << " [" << get_status() << "] "
        << get_start_station() << " " << get_start_time().to_24h_time()
        << " (" << (get_start_time() + get_start_delay()).to_24h_time()
        << ") to " << get_end_station() << " " << get_arrival_time().to_24h_time()
        << " (" << (get_arrival_time() + get_arrival_delay()).to_24h_time() << ") "
        << "Delay (" << get_arrival_delay().to_24h_time() << ") ";
    return os;
}

// Get train engine(s) max speed, it's the slowest engine attached.
int Train::get_engine_max_speed() {
    int speed = 0;
    for(auto &i: vehicles) {
        if(i->has_engine()) {
            if(speed == 0) {
                speed = i->get_max_speed();
            }
            // Store the slowest vehicle speed.
            else if(speed > i->get_max_speed()) {
                speed = i->get_max_speed();
            }
        }
    }
    return speed;
}