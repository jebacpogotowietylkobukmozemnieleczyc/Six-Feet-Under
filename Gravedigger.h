//
// Created by klimas on 10.05.16.
//

#ifndef MPI_GRAVEDIGGER_H
#define MPI_GRAVEDIGGER_H

#include "Process.h"
#include "unistd.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>

#define NEW_CORPSE 0
#define CORPSE_REQ 1
#define CORPSE_ACK 2
#define CLERK_REQ 3
#define CLERK_ACK 4

#define BURIAL_MIN_TIME 1000
#define BURIAL_MAX_TIME 2000

#define PAPERWORK_MIN_TIME 2000
#define PAPERWORK_MAX_TIME 4000

class Gravedigger : public Process {
public:
    Gravedigger(int tid, int size) : Process(tid, size) {
        for(int i = 1; i < size+1; ++i) { ordering[i][0] = 1; ordering[i][1] = 0; }
        receiveFunction[0] = [&]() { addNewCorpse();};
        receiveFunction[1] = [&]() { corpseRequest();};
        receiveFunction[2] = [&]() { corpseAcknowledge();};
        receiveFunction[3] = [&]() { clerkRequest();};
        receiveFunction[4] = [&]() { clerkAcknowledge();};
    }

    void chooseRandomCorpse();
    void chooseCorpse();
    void addNewCorpse();
    void corpseRequest();
    void corpseAcknowledge();
    void makeBurial();
    void clerkRequest();
    void clerkAcknowledge();
    void makePaperwork();
    void makeQueueEmpty();
    void answerWhileWaiting(int howLong);
    void run() override ;

    void printCorpseList();

    int myOrderingPos();

protected:
    std::vector<int> corpses;
    std::queue<int> paperworkQueue;
    std::unordered_set<int> corpseAckSet;
    std::unordered_set<int> clerkAckSet;

    bool waitingForCorpse = true;
};


#endif //MPI_GRAVEDIGGER_H
