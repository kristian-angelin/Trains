/*  File name: train_event.h
 *  Author: Kristian Angelin
 *  Date: 2019-05-29
 *  Description: Definition of abstract Train_event class and derived event classes.
 *  Handles the creation of events.
 */

#ifndef DT060G_TRAIN_EVENT_H
#define DT060G_TRAIN_EVENT_H

// Forward declaration
class Simulation;
class Sim_logger;

#include <memory>
#include "sim_time.h"
#include "train.h"
#include "sim_logger.h"

class Train_event {
protected:
    // Data members
    Sim_time event_time;
    std::shared_ptr<Train> the_train;
    std::shared_ptr<Simulation> the_sim;
    std::shared_ptr<Sim_logger> logger;

    // TIME CONSTANTS FOR EVENTS
    const Sim_time READY_TIME = {0,10}; // Time before an assembled train to arrive to plattform.
    const Sim_time RETRY_TIME = {0,10}; // Time for retry after failed assembly.
    const Sim_time DISASSEMBLY_TIME = {0,20};   // Time to remove train vehicles.
    const Sim_time ASSEMBLY_TIME = {0, 30};     // Time before departure a train starts assembly

    // Functions
    void assembly(std::vector<int> missing_carts);

public:
    Train_event(Sim_time time, std::shared_ptr<Train> train, std::shared_ptr<Simulation> sim,
        std::shared_ptr<Sim_logger> e_logger)
        : event_time(time), the_train(train), the_sim(sim), logger(e_logger){}

    // Destrucor
    virtual ~Train_event() {}

    // Get functions
    Sim_time get_event_time() const { return event_time; }
    int get_train_id() const { return the_train->get_id(); }
    std::shared_ptr<Train> get_train() { return the_train; }


    // Virtual functions
    virtual void process_event() = 0;
    virtual Sim_time get_next_event_time(std::string train_status);
    Sim_time calc_arrival_time_and_speed();
    bool is_train_on_time();
};

// Class for comparing train events.
class Event_comparison {
public:
    bool operator() (std::shared_ptr<Train_event> &lhs, std::shared_ptr<Train_event> &rhs) {
        return rhs->get_event_time() < lhs->get_event_time();
    }
};

// -----------------------  DERIVED EVENT CLASSES  ---------------------------

class Assemble_event : public Train_event {
public:
    Assemble_event(Sim_time time, std::shared_ptr<Train> train, std::shared_ptr<Simulation> sim,
            std::shared_ptr<Sim_logger> e_logger)
        : Train_event(time, train, sim, e_logger){}

    void process_event() override;
};

class Retry_assemble_event : public Train_event {
private:
    std::vector<int> missing_carts;
public:
    Retry_assemble_event(Sim_time time, std::shared_ptr<Train> train,
        std::shared_ptr<Simulation> sim, std::shared_ptr<Sim_logger> e_logger,
        std::vector<int> missing)
        : Train_event(time, train, sim, e_logger), missing_carts(missing){}

    void process_event() override;
};

class Ready_event : public Train_event {
public:
    Ready_event(Sim_time time, std::shared_ptr<Train> train,
        std::shared_ptr<Simulation> sim, std::shared_ptr<Sim_logger> e_logger)
        : Train_event(time, train, sim, e_logger){}

    void process_event() override;
};

class Running_event : public Train_event {
public:
    Running_event(Sim_time time, std::shared_ptr<Train> train, std::shared_ptr<Simulation> sim,
                  std::shared_ptr<Sim_logger> e_logger)
        : Train_event(time, train, sim, e_logger){}

    void process_event() override;
};

class Arrive_event : public Train_event {
public:
    Arrive_event(Sim_time time, std::shared_ptr<Train> train, std::shared_ptr<Simulation> sim,
                 std::shared_ptr<Sim_logger> e_logger)
        : Train_event(time, train, sim, e_logger){}

    void process_event() override;
};

class Finish_event : public Train_event {
public:
    Finish_event(Sim_time time, std::shared_ptr<Train> train, std::shared_ptr<Simulation> sim,
                 std::shared_ptr<Sim_logger> e_logger)
        : Train_event(time, train, sim, e_logger){}

    void process_event() override;
};

#endif //DT060G_TRAIN_EVENT_H
