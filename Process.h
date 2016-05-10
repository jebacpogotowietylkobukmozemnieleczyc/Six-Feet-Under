//
// Created by klimas on 10.05.16.
//

#ifndef MPI_PROCESS_H
#define MPI_PROCESS_H

#define ROOT_ID 0
#define SEND_FUNCTIONS 1
#define RECEIVE_FUNCTIONS 1
#define MAX_ID 2000

#include <mpi.h>
#include <unordered_set>
#include <functional>


class Process {
public:
    virtual ~Process() {
        printf("%d : %d : Finalize \n",tid, clock);
    }

    Process(int tid, int size) : tid(tid), size(size),clock(0) {
        printf("%d : %d : Start \n",tid, clock);
    }
    virtual void run() = 0;
    void send(int tag);
    void receive();
    uint32_t getRandom(uint32_t min, uint32_t max);

    std::array<std::function<void()>,RECEIVE_FUNCTIONS> receiveFunction;
    std::array<std::function<void()>,SEND_FUNCTIONS> sendFunction;

    void printCorpses();

protected:
    MPI_Status status;
    int tid,size,clock;
    std::unordered_set<int> corpses;

    int msg[2];

    int dest;

};


#endif //MPI_PROCESS_H
