//
// Created by klimas on 10.05.16.
//

#ifndef MPI_MANAGEMENT_H
#define MPI_MANAGEMENT_H


#include "Process.h"

class Management : public Process {
public:
    Management(int tid, int size) : Process(tid, size) {
        sendFunction[0] = [](){};
    }
    void run() override ;

};

#endif //MPI_MANAGMENT_H
