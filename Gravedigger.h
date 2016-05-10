//
// Created by klimas on 10.05.16.
//

#ifndef MPI_GRAVEDIGGER_H
#define MPI_GRAVEDIGGER_H


#include "Mpi.h"

class Gravedigger : public Process {
public:
    Gravedigger(int tid, int size) : Process(tid, size) {
        receiveFunction[0] = [this]() { corpses.insert(msg[0]); };
    }
    void run() override ;

};


#endif //MPI_GRAVEDIGGER_H
