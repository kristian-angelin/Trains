/*  File name: test_event_processing.h
 *  Author: Kristian Angelin
 *  Date: 2019-05-29
 *  Description: 
 */

#ifndef DT060G_TEST_EVENT_PROCESSING_H
#define DT060G_TEST_EVENT_PROCESSING_H

#include <vector>
#include <queue>
#include <memory>
#include "train_event.h"
#include "train.h"

class Test_event_processing {
private:
    std::vector<Train> trains;
    std::priority_queue<std::shared_ptr<Train_event>, std::vector<std::shared_ptr<Train_event>>, Event_comparison> event_queue;
public:
    const Sim_time RETRY_ASSEMBLY_TIME = {0, 10};
    const Sim_time START_ASSEMBLY_FOR_DEP = {0, 30};



    void run();
    bool load_trains();
    void print_train_list();
    void load_events();
    void run_event_list();
};


#endif //DT060G_TEST_EVENT_PROCESSING_H
