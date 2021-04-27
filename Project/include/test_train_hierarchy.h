/* File name: test_train_hierarchy.h
 * Author: Kristian Angelin
 * Date: 2019-05-26
 * Description: Program for testing train hierarchy for project trains.
 */

#ifndef DT060G_TEST_TRAIN_HIERARCHY_H
#define DT060G_TEST_TRAIN_HIERARCHY_H

#include <vector>
#include "train.h"

class Test_train_hierarchy {
private:
    std::vector<Train> trains;
public:
    void run();
    bool load_trains();
    void print_train_list();
};


#endif //DT060G_TEST_TRAIN_HIERARCHY_H
