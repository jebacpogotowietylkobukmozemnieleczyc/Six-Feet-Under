//
// Created by klimas on 10.05.16.
//

#ifndef MPI_MANAGEMENT_H
#define MPI_MANAGEMENT_H

#include "Process.h"
#include "unistd.h"

#define NEW_CORPSE 0

#define MURDER_MIN_TIME 200
#define MURDER_MAX_TIME 600

class Management : public Process {
public:
    Management(int tid, int size) : Process(tid, size) { }
    void run() override ;

};

#endif //MPI_MANAGMENT_H
