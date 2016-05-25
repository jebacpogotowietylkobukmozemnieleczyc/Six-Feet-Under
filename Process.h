//
// Created by klimas on 10.05.16.
//

#ifndef MPI_PROCESS_H
#define MPI_PROCESS_H

#define ROOT_ID 0
#define SEND_FUNCTIONS 5
#define RECEIVE_FUNCTIONS 5
#define MAX_CORPSE_LIST_SIZE 10

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
    void sendAll(int tag, int tabSize = 2);
    void receive();

    bool meFirstFunc(int myClock);
    uint32_t getRandom(uint32_t min, uint32_t max);

    std::array<std::function<void()>,RECEIVE_FUNCTIONS> receiveFunction;

protected:
    MPI_Status status, statusProbe;
    int tid,size,clock,myCorpseClock,myClerkClock;
    int myCorpse = -1;

    int msg[MAX_CORPSE_LIST_SIZE];
    bool meFirst;

};


#endif //MPI_PROCESS_H
