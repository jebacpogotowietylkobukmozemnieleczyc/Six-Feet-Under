//
// Created by klimas on 10.05.16.
//

#ifndef MPI_PROCESS_H
#define MPI_PROCESS_H

#define ROOT_ID 0
#define SEND_FUNCTIONS 5
#define RECEIVE_FUNCTIONS 5

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
    void send(int tag, int dest);
    void sendAll(int tag);
    void receive();

    bool meFirstFunc(int myClock);
    uint32_t getRandom(uint32_t min, uint32_t max);

    std::array<std::function<void()>,RECEIVE_FUNCTIONS> receiveFunction;
    std::array<std::function<void()>,SEND_FUNCTIONS> sendFunction;

    void printCorpses();

protected:
    MPI_Status status;
    int tid,size,clock,myActionClock,myCorpseClock,myClerkClock;
    int myCorpse = -1;

    int msg[2];
    bool meFirst;

};


#endif //MPI_PROCESS_H
