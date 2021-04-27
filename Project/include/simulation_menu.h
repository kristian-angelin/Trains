/*  File name: simulation_menu.h
 *  Author: Kristian Angelin
 *  Date: 2019-05-30
 *  Description: Definition of Simulation_menu class. Handles the menu for the simulation.
 */

#ifndef DT060G_SIMULATION_MENU_H
#define DT060G_SIMULATION_MENU_H

#include <memory>
#include "sim_time.h"
#include "simulation.h"
#include "sim_logger.h"

class Simulation_menu {
private:
    // Data members
    std::shared_ptr<Simulation> the_sim;
    std::shared_ptr<Sim_logger> logger;

    // Logic for menus
    void menu_start();
    void menu_simulation();
    void menu_log_level();
    void menu_train();
    void menu_station();
    void menu_vehicle();

    // Screen output for menus
    void print_menu_start();
    void print_menu_simulation();
    void print_menu_log_level();
    void print_menu_train();
    void print_menu_station();
    void print_menu_vehicle();

    // Functions for getting info on train stations.
    void print_station_list();
    void search_station_by_name();

    // Print statistics at end
    void print_statistics_screen();


    void load_files();

    // Functions for taking user input
    Sim_time change_time();
    int get_user_input();

    // Help functions for exception handling
    void check_valid_h(int data);
    void check_valid_min(int data);

public:
    Simulation_menu() : logger(std::make_shared<Sim_logger>())
                        {the_sim = std::make_shared<Simulation>(logger);}

    // Start of the simulation menu
    void run();
};


#endif //DT060G_SIMULATION_MENU_H
