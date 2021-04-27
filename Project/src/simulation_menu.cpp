/*  File name: simulation_menu.cpp
 *  Author: Kristian Angelin
 *  Date: 2019-05-30
 *  Description: Implementation of class Simulation_menu.
 */

#include <iostream>
#include <exception>
#include <limits>
#include <sstream>
#include "simulation_menu.h"
#include "train.h"
#include "train_station.h"


void Simulation_menu::run() {
    menu_start();
}

// Logic for start menu
void Simulation_menu::menu_start() {
    bool run_menu = true;
    do {
        print_menu_start();
        Sim_time time_change;
        switch (get_user_input()) {
            case 1: time_change = change_time();
                    // Check that new start time is before end time.
                    if(time_change < the_sim->get_end_time()){
                        the_sim->set_start_time(time_change);
                        break;
                    }
                    std::cout << std::endl << "[ERROR]: Start time must be before end time!" << std::endl;
                    break;

            case 2: time_change = change_time();
                    // Check that new end time is after start time.
                    if(the_sim->get_start_time() < time_change){
                        the_sim->set_end_time(time_change);
                        break;
                    }
                    std::cout << std::endl << "[ERROR]: End time must be after start time!" << std::endl;
                    break;

            case 3: menu_simulation();
                    run_menu = false;
                    break;

            case 0: run_menu = false;
                    break;
            default: std::cout << std::endl << "[ERROR]: Invalid input" << std::endl;
        }
    } while (run_menu);
}

// Logic for simulation menu
void Simulation_menu::menu_simulation() {
    load_files();
    the_sim->load_starting_events(the_sim);
    the_sim->run_sim(the_sim->get_start_time(), true);
    bool run_menu = true;
    do {
        print_menu_simulation();
        switch (get_user_input()) {
            case 1: the_sim->set_interval_time(change_time());
                    break;
            case 2: run_menu = the_sim->run_sim(the_sim->get_current_time() + the_sim->get_interval_time());
                    std::cout << std::endl << "**** [INTERVAL DONE] ****" << std::endl
                                <<"[press ENTER to continue]" << std::endl;
                    std::cin.get();
                    break;
            case 3: run_menu = the_sim->go_to_next_event();
                    std::cout << "[press ENTER to continue]" << std::endl;
                    std::cin.get();
                    break;
            case 4: run_menu = the_sim->run_sim(the_sim->get_end_time());
                    break;
            case 5: menu_log_level();
                    break;
            case 6: menu_train();
                    break;
            case 7: menu_station();
                    break;
            case 8: menu_vehicle();
                    break;
            case 0: run_menu = false;
                    break;
            default: std::cout << std::endl << "[ERROR]: Invalid input" << std::endl;
        }
    } while (run_menu);
    std::cout << std::endl << "****** [SIMULATION ENDED] *******" << std::endl << std::endl
                << "[press ENTER to show statistics]" << std::endl;
    std::cin.get();
    print_statistics_screen();
}

// Logic for log level menu
void Simulation_menu::menu_log_level() {
    bool run_again = true;
    do {
        print_menu_log_level();
        switch (get_user_input()) {
            case 1: logger->set_log_level("LOW");
                    run_again = false;
                    break;
            case 2: logger->set_log_level("HIGH");
                    run_again = false;
                    break;
            case 0: run_again = false;
                    break;
            default: std::cout << std::endl << "[ERROR]: Invalid input" << std::endl;
        }
    } while (run_again);
}

// Logic for train menu
void Simulation_menu::menu_train() {
    bool run_menu = true;
    std::shared_ptr<Train> temp_train;
    do {
        print_menu_train();
        switch (get_user_input()) {
            case 1: if(the_sim->get_train_by_number(get_user_input(), temp_train)) {
                    std::cout << std::endl << logger->print_train(temp_train).str()
                                << std::endl << std::endl;
                    }
                    else {
                        std::cout << std::endl << "[ERROR]: No train by that number!" << std::endl;
                    }
                    break;
            case 2: if(the_sim->get_train_by_vehicle_id(get_user_input(), temp_train)) {
                    std::cout << std::endl << logger->print_train(temp_train).str()
                                << std::endl << std::endl;
                    }
                    else {
                    std::cout << std::endl << "[ERROR]: No vehicle attached to a train by that id!" << std::endl;
                    }
                    break;
            case 3: menu_log_level();
                    break;
            case 0: run_menu = false;
                    break;
            default: std::cout << std::endl << "[ERROR]: Invalid input" << std::endl;
        }
    } while (run_menu);
}

// Logic for station menu
void Simulation_menu::menu_station() {
    bool run_menu = true;
    do {
        print_menu_station();
        switch (get_user_input()) {
            case 1: print_station_list();
                    break;
            case 2:
                search_station_by_name();
                    break;
            case 3: menu_log_level();
                    break;
            case 0: run_menu = false;
                    break;
            default: std::cout << std::endl << "[ERROR]: Invalid input" << std::endl;
        }
    } while (run_menu);
}

// Logic for vehicle menu
void Simulation_menu::menu_vehicle() {
    bool run_menu = true;
    std::shared_ptr<Vehicle> temp_vehicle;
    std::string temp_string;
    do {
        print_menu_vehicle();
        switch (get_user_input()) {
            case 1: if(the_sim->get_vehicle_by_id(get_user_input(), temp_vehicle)) {
                        std::cout << std::endl << logger->print_vehicle(temp_vehicle).str()
                                    << std::endl << std::endl;
                    }
                    else {
                        std::cout << std::endl <<  "[ERROR]: No vehicle with that id!" << std::endl;
                    }
                    break;
            case 2: if(the_sim->get_vehicle_location(get_user_input(), temp_string)) {
                    std::cout << temp_string << std::endl << std::endl;
                    }
                    else {
                        std::cout << std::endl << "[ERROR]: No vehicle with that id!" << std::endl;
                    }
                    break;
            case 3: if(the_sim->get_vehicle_by_id(get_user_input(), temp_vehicle)) {
                        std::cout << std::endl << logger->print_vehicle_history(temp_vehicle).str()
                                    << std::endl << std::endl;
                    }
                    else {
                        std::cout << std::endl << "[ERROR]: No vehicle with that id!" << std::endl;
                    }
                    break;
            case 4: menu_log_level();
                    break;
            case 0: run_menu = false;
                    break;
            default: std::cout << std::endl << "[ERROR]: Invalid input" << std::endl;
        }
    } while (run_menu);
}

void Simulation_menu::print_station_list() {
    std::vector<std::string> station_names = the_sim->get_station_names();
    std::cout << std::endl << "Station names: " << std::endl;
    for(auto &i: station_names) {
        std::cout << i << std::endl;
    }
}

void Simulation_menu::search_station_by_name() {
    std::cout << "Please enter station name: ";
    std::string name;
    std::getline(std::cin, name);
    std::shared_ptr<Train_station> station;
    if(the_sim->get_train_station(name, station)) {
        std::cout << std::endl << logger->print_station(station).str() << std::endl;
    }
    else {
        std::cout << std::endl << "[ERROR]: Could not find station by that name!" << std::endl;
    }
}

// Screen output for start menu
void Simulation_menu::print_menu_start() {
    std::cout << std::endl
                <<  "********** START MENU ***********" << std::endl
                << "1. Change start time [" << the_sim->get_start_time() << "]" << std::endl
                << "2. Change end time [" << the_sim->get_end_time() << "]" << std::endl
                << "3. Start simulation" << std::endl
                << "0. Exit" << std::endl;
}

// Screen output for simulation menu
void Simulation_menu::print_menu_simulation() {
    std::cout << std::endl
                << "******** SIMULATION MENU ********" << std::endl
                << "Current time [" << the_sim->get_current_time().to_24h_time() << "] "
                << "ends [" << the_sim->get_end_time()  << "]" << std::endl
                << "*********************************" << std::endl
                << "1. Change interval [" << the_sim->get_interval_time() << "]" << std::endl
                << "2. Run next interval" << std::endl
                << "3. Run next event" << std::endl
                << "4. Finish (Complete simulation)" << std::endl
                << "5. Change log level [" << logger->get_log_level() << "]" << std::endl
                << "6. Train menu" << std::endl
                << "7. Station menu" << std::endl
                << "8. Vehicle menu" << std::endl
                << "0. Exit" << std::endl;
}

// Screen output for log level menu
void Simulation_menu::print_menu_log_level() {
    std::cout << std::endl
                << "******** LOG LEVEL MENU *********" << std::endl
                << "Level [" << logger->get_log_level() << "]" << std::endl
                << "*********************************" << std::endl
                << "1. Set to LOW level" << std::endl
                << "2. Set to HIGH level" << std::endl
                << "0. Return" << std::endl;
}

// Screen output for train menu
void Simulation_menu::print_menu_train() {
    std::cout << std::endl
                << "********** TRAIN MENU ***********" << std::endl
                << "1. Search train by number" << std::endl
                << "2. Search train by attached vehicle id" << std::endl
                << "3. Change log level [" << logger->get_log_level() << "]" << std::endl
                << "0. Return" << std::endl;
}

// Screen output for station menu
void Simulation_menu::print_menu_station() {
    std::cout << std::endl
                << "********* STATION MENU **********" << std::endl
                << "1. Show all station names" << std::endl
                << "2. Show station info by name" << std::endl
                << "3. Change log level [" << logger->get_log_level() << "]" << std::endl
                << "0. Return" << std::endl;
}

// Screen output for vehicle menu
void Simulation_menu::print_menu_vehicle() {
    std::cout << std::endl
                << "********* VEHICLE MENU **********" << std::endl
                << "1. Show vehicle by id" << std::endl
                << "2. Show vehicle location by id" << std::endl
                << "3. Show vehicle movement history by id" << std::endl
                << "4. Change log level [" << logger->get_log_level() << "]" << std::endl
                << "0. Return" << std::endl;
}

// Screen output for statistics screen
void Simulation_menu::print_statistics_screen() {
    std::cout << std::endl << "***** [SIMULATION STATISTICS] *****" << std::endl << std::endl
              << "End time for simulation: " << the_sim->get_end_time().to_24h_time() << std::endl << std::endl
                << logger->print_statistics(the_sim->get_all_trains()).str();
    std::cout << std::endl;
    std::cout << "***** [STATISTICS PRINTED] ******" << std::endl << std::endl
                << "[press ENTER to exit]";
    std::cin.get();
}

// Screen output and logic for loading files
void Simulation_menu::load_files() {
    std::cout << std::endl << "***** Loading files ***** " << std::endl;
    if(the_sim->load_stations_from_file()) {
        std::cout << "Train stations loaded successfully!" << std::endl;
    }
    else {
        std::cout << "[ERROR]: Train stations didn't load!" << std::endl;
    }
    if(the_sim->load_train_routes_from_file()) {
        std::cout << "Train routes loaded successfully!" << std::endl;
    }
    else {
        std::cout << "[ERROR]: Couldn't load Train routes!" << std::endl;
    }
    if(the_sim->load_trains_from_file()) {
        std::cout << "Trains loaded successfully!" << std::endl;
    }
    else {
        std::cout << "[ERROR]: Couldn't load Trains!" << std::endl;
    }
}

// Screen output and logic for user time input.
Sim_time Simulation_menu::change_time() {
    bool retry;
    Sim_time temp_time;
    do {
        retry = false;
        std::cout << std::endl << "Please enter time in format [hh:mm]" << std::endl;
        int hours;
        int minutes;
        std::cin.exceptions(std::ios::failbit);
        try {
            std::cout << std::endl << "Enter hours: ";
            std::cin >> hours;
            check_valid_h(hours);
            std::cout << std::endl << "Enter minutes: ";
            std::cin >> minutes;
            check_valid_min(minutes);
            temp_time = {hours, minutes};
            std::cin.get();
        }
        catch (const std::out_of_range &e) {
            std::cout << std::endl << "[EXCEPTION]: " << e.what() << std::endl;
            std::cout << "Please start over!" << std::endl;
            retry = true;
        }
        catch (const std::ios_base::failure &e) {
            std::cout << std::endl << "[EXCEPTION]: Must enter a numeric value!" << std::endl;
            std::cout << "Please start over!" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            retry = true;
        }
    } while (retry);
    return temp_time;
}

// Screen output and logic for user integer input.
int Simulation_menu::get_user_input() {
    bool retry;
    int user_input = 0;
    do{
        retry = false;
        std::cin.exceptions(std::ios::failbit);
        try {
            std::cout << std::endl << "Please input number:  ";
            std::cin >> user_input;
            std::cin.get();
        }
        catch (const std::ios_base::failure &e) {
            std::cout << std::endl << "[EXCEPTION]: Must enter a numeric value!" << std::endl;
            std::cout << "Please start over!" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            retry = true;
        }
    }while (retry);

    return user_input;
}

// Clock hour range check
void Simulation_menu::check_valid_h(int data) {
    if (data < 0 || data > 23){
        throw std::out_of_range("Hours can only go from 0 to 23!");
    }
}

// Clock minutes range check
void Simulation_menu::check_valid_min(int data) {
    if (data < 0 || data > 59) {
        throw std::out_of_range("Minutes can only go from 0 to 59!");
    }
}