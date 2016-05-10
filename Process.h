//
// Created by klimas on 10.05.16.
//

#ifndef MPI_PROCESS_H
#define MPI_PROCESS_H

#define ROOT_ID 0
#define MSG_TAG 100

#include <mpi.h>

class Process {
public:
    virtual ~Process() {
        printf("Finalize %d from %d\n",tid, size);
    }

    Process(int tid, int size) : tid(tid), size(size) {
        printf("Start %d from %d\n",tid, size);
    }
    virtual void run() = 0;

protected:
    MPI_Status status;
    int tid,size;

    int msg[2];

};


#endif //MPI_PROCESS_H
