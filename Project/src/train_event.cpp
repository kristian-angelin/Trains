/*  File name: train_event.cpp
 *  Author: Kristian Angelin
 *  Date: 2019-05-29
 *  Description: Implementation of class Train_event.
 */

#include <algorithm>
#include "simulation.h"
#include "train_event.h"
#include "test_event_processing.h"

void Assemble_event::process_event() {
    auto missing_carts = the_train->get_required_vehicle_types();
    assembly(missing_carts);
}

void Retry_assemble_event::process_event() {
    assembly(missing_carts);
}

// Try to assemble the train.
void Train_event::assembly(std::vector<int> missing_carts) {
    bool assembly_complete = true;
    std::vector<int> temp_carts = missing_carts;

    // Check if all carts are available
    for(auto &c: missing_carts) {

        std::shared_ptr<Vehicle> temp_vehicle;
        // If we can find the vehicle type in station pool add it to train.
        if(the_train->get_current_station()->get_vehicle_by_type(c, temp_vehicle)) {
            logger->log_vehicle_event(event_time, temp_vehicle, the_train);
            the_train->add_vehicle(std::move(temp_vehicle));

            // Look at desired trains and remove the one added to train
            auto it = std::find_if(temp_carts.begin(), temp_carts.end(), [c](int num){
                return num == c;
            });
            temp_carts.erase(it);
        }
        else {
            assembly_complete = false;
        }

    }
    if(assembly_complete) {
        the_train->set_status("ASSEMBLED");
        // Check so that we dont add more events after simulation end.
        if((the_train->get_start_time() + the_train->get_start_delay() - READY_TIME)
        <= the_sim->get_end_time()) {
            the_sim->add_event(std::shared_ptr<Ready_event>(std::make_shared<Ready_event>(
                    (the_train->get_start_time() + the_train->get_start_delay() - READY_TIME),
                    the_train, the_sim, logger)));
        }
    }
    else {
        the_train->set_status("INCOMPLETE");
        // Set delays
        the_train->set_start_delay(the_train->get_start_delay() + RETRY_TIME);
        the_train->set_arrival_delay(the_train->get_arrival_delay() + RETRY_TIME);
        // Check so that we dont add more events after simulation end.
        if(event_time + RETRY_TIME <= the_sim->get_end_time()) {
            // Create retry event
            the_sim->add_event(std::shared_ptr<Retry_assemble_event>
                    (std::make_shared<Retry_assemble_event>(
                            (event_time + RETRY_TIME), the_train, the_sim, logger, temp_carts)));
        }
    }
}

void Ready_event::process_event() {
    the_train->set_status("READY");

    // Check so that we dont add more events after simulation end.
    if((get_event_time() + READY_TIME) <= the_sim->get_end_time()) {
        // Create next event for train
        the_sim->add_event(std::shared_ptr<Running_event>(std::make_shared<Running_event>(
                get_event_time() + READY_TIME, the_train, the_sim, logger)));
    }
}

void Running_event::process_event() {
    the_train->set_status("RUNNING");
    if(the_train->get_current_station()->remove_train(the_train->get_id())) {
        the_train->set_current_station(nullptr);
    }
    // Calculate arrival time.
    Sim_time arrival_time = calc_arrival_time_and_speed();

    // Create next event for train
    the_sim->add_event(std::shared_ptr<Arrive_event> (std::make_shared<Arrive_event>(
            arrival_time, the_train, the_sim, logger)));
}

void Arrive_event::process_event() {
    the_train->set_status("ARRIVED");

    // Set the train station for train.
    std::shared_ptr<Train_station> temp_station;
    the_sim->get_train_station(the_train->get_end_station(), temp_station);
    the_train->set_current_station(temp_station);

    // Add train to the train station.
    the_train->get_current_station()->add_train(the_train);

    // Create next event for train
    the_sim->add_event(std::shared_ptr<Finish_event> (std::make_shared<Finish_event>(
            (get_event_time() + DISASSEMBLY_TIME), the_train, the_sim, logger)));
}

void Finish_event::process_event() {
    the_train->set_status("FINISHED");
    // Go through all connected vehicles and add to train pool at station
    // and add to vehicle history log.
    for(auto i: the_train->get_train_vehicles()) {
        logger->log_vehicle_event(event_time, i, the_train->get_current_station());
        the_train->get_current_station()->add_vehicle(std::move(i));
    }
    // Sort vehicle pool after id.
    the_train->get_current_station()->sort_vehicle_vector();
    the_train->remove_vehicles();
}

Sim_time Train_event::calc_arrival_time_and_speed() {
    // Check the trains max speed
    double max_speed = the_train->get_max_speed();
    double engine_max_speed = the_train->get_engine_max_speed();
    if(engine_max_speed < max_speed) {
        max_speed = engine_max_speed;
    }

    int distance = the_sim->get_route_distance(the_train->get_start_station(), the_train->get_end_station());
    double route_standard_speed = distance / (the_train->get_arrival_time().convert_to_decimal()
                                            - the_train->get_start_time().convert_to_decimal());
    double time_in_decimal;

    // Check if the train is capable of running the intended speed of route.
    if(max_speed < route_standard_speed) {
        the_train->set_current_speed(max_speed);
        time_in_decimal = distance / max_speed;
    }
    // If train can run, check if it is on time.
    else if(is_train_on_time()) {
        the_train->set_current_speed(route_standard_speed);
        return the_train->get_arrival_time();
    }
    // If train is delayed, we try to go faster
    else {
        // To avoid division by zero or negative number we check that the current
        // time hasn't passed the planned arrival time.
        if(the_train->get_arrival_time() <= (the_train->get_start_time() + the_train->get_start_delay())) {
            the_train->set_current_speed(max_speed);
            time_in_decimal = distance / max_speed;
        }
        else {
            // Check if the train is able to travel at speed needed to be on time.
            // If not, go at max speed.
            Sim_time time_to_planned_arrival = the_train->get_arrival_time() - (the_train->get_start_time() + the_train->get_start_delay());
            double arrive_on_time_speed = distance / time_to_planned_arrival.convert_to_decimal();
            if(arrive_on_time_speed < max_speed) {
                the_train->set_current_speed(arrive_on_time_speed);
                time_in_decimal = distance / arrive_on_time_speed;
            }
            else {
                the_train->set_current_speed(max_speed);
                time_in_decimal = distance / max_speed;
            }
        }

    }
    Sim_time temp_time(time_in_decimal);
    the_train->set_arrival_delay((event_time + temp_time) - the_train->get_arrival_time());
    return event_time + temp_time;
}

bool Train_event::is_train_on_time() {
    Sim_time on_time{0,0};
    return the_train->get_start_delay() == on_time;
}

// A function for calculating the next event time.
Sim_time Train_event::get_next_event_time(std::string train_status) {
    Sim_time event_time;
    if(train_status == "NOT ASSEMBLED"){
        event_time = get_train()->get_start_time() - ASSEMBLY_TIME;
    }
    else if(train_status == "INCOMPLETE") {
        event_time = get_event_time() + RETRY_TIME;
    }
    else if(train_status == "ASSEMBLED") {
        event_time = the_train->get_start_time() + the_train->get_start_delay() - READY_TIME;
    }
    else if(train_status == "READY") {
        event_time = get_event_time() + READY_TIME;
    }
    else if(train_status == "RUNNING") {
        event_time = calc_arrival_time_and_speed();

    }
    else if(train_status == "ARRIVED") {
        event_time = get_event_time() + DISASSEMBLY_TIME;
    }
    return event_time;
}