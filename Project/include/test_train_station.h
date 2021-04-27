/*  File name: test_train_station.h
 *  Author: Kristian Angelin
 *  Date: 2019-05-28
 *  Description: 
 */

#ifndef DT060G_TEST_TRAIN_STATION_H
#define DT060G_TEST_TRAIN_STATION_H

#include <vector>
#include "train_station.h"

class Test_train_station {
private:
    std::vector<Train_station> stations;
public:
    void run();
    bool load_stations();
    void print_stations();
};


#endif //DT060G_TEST_TRAIN_STATION_H
