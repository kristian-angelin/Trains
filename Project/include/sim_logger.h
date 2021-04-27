/*  File name: sim_logger.h
 *  Author: Kristian Angelin
 *  Date: 2019-06-01
 *  Description: Definition of Sim_logger class. Handles the output of events, trains, vehicles to
 *  screen and output to log file.
 */

#ifndef DT060G_SIM_LOGGER_H
#define DT060G_SIM_LOGGER_H

// Forward declaration
class Train_event;

#include <string>
#include <vector>
#include "sim_time.h"
#include "train_event.h"

class Sim_logger {
private:
    // Data members
    std::string log_level = "HIGH";         // Level of detail
    const std::string LOG_FILE = "Trainsim.log"; // File to output log data.

public:
    // Constructor
    Sim_logger();

    // Set and get log level.
    std::string const get_log_level() { return log_level; }
    void set_log_level(std::string level) { log_level = std::move(level); }

    // Log event to screen and file
    void log_event(std::shared_ptr<Train_event> event);
    void print_event_info(std::shared_ptr<Train_event> event);
    void save_event_to_log_file(std::shared_ptr<Train_event> event);

    // Print outs to be used on screen or to save file.
    std::stringstream print_train_event(std::shared_ptr<Train_event> event);
    std::stringstream print_train(std::shared_ptr<Train> train);
    std::stringstream print_train_vehicles(std::shared_ptr<Train> train);
    std::stringstream print_vehicle(std::shared_ptr<Vehicle> vehicle);
    std::stringstream print_vehicle_history(std::shared_ptr<Vehicle> vehicle);
    std::stringstream print_station(std::shared_ptr<Train_station> station);
    std::stringstream print_station_trains(std::shared_ptr<Train_station> station);
    std::stringstream print_station_vehicles(std::shared_ptr<Train_station> station);
    std::stringstream print_event_message(std::shared_ptr<Train_event> event);

    // Statistics for print to screen.
    std::stringstream print_statistics(std::vector<std::shared_ptr<Train>> &trains);
    std::stringstream print_summary(std::vector<std::shared_ptr<Train>> &trains);
    std::stringstream print_trains_on_time(std::vector<std::shared_ptr<Train>> &trains);
    std::stringstream print_delayed_trains(std::vector<std::shared_ptr<Train>> &trains);
    std::stringstream print_non_departing_trains(std::vector<std::shared_ptr<Train>> &trains);

    // Log a vehicle event in vehicle history.
    // Using overloading to log a connected to vehicle event or disconnect at station event.
    void log_vehicle_event(Sim_time event_time, std::shared_ptr<Vehicle> vehicle, std::shared_ptr<Train> train);
    void log_vehicle_event(Sim_time event_time, std::shared_ptr<Vehicle> vehicle, std::shared_ptr<Train_station> station);

    // Help function for converting vehicle type from int to string
    std::string convert_int_to_vehicle_type(int type);
};


#endif //DT060G_SIM_LOGGER_H
