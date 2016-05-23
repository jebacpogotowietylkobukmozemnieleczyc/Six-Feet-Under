//
// Created by klimas on 10.05.16.
//

#include "Gravedigger.h"
#include "Process.h"
#include <random>

void Process::send(int tag, int dest) {
    msg[1] = ++clock;
    MPI_Send( msg, 2, MPI_INT, dest, tag, MPI_COMM_WORLD );
    //printf("Id: %d | Clock: %d | Send from %d to %d | Tag: %d Corpse: #%d | myCorpse #%d\n", tid, msg[1], tid, dest ,tag, msg[0], myCorpse);
}

void Process::sendAll(int tag){
    msg[1] = ++clock;
    for( int i = 1; i < size; ++i){
        if (i != tid){
            MPI_Send( msg, 2, MPI_INT, i, tag, MPI_COMM_WORLD );
            //printf("Id: %d | Clock: %d | Send from %d to %d | Tag: %d Corpse: #%d | myCorpse #%d\n", tid, msg[1], tid, i ,tag, msg[0], myCorpse);
        }
    }
}

void Process::receive() {
    MPI_Recv(msg, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    if(status.MPI_TAG == 1) meFirst = meFirstFunc(myCorpseClock);
    if(status.MPI_TAG == 3) meFirst = meFirstFunc(myClerkClock);
    clock = std::max(clock, msg[1])+1;
    //printf("Id: %d | Clock: %d | %d received from %d | Tag: %d Corpse: #%d | myCorpse #%d\n", tid, clock, tid, status.MPI_SOURCE,status.MPI_TAG, msg[0], myCorpse);
    receiveFunction.at(status.MPI_TAG)();
}

bool Process::meFirstFunc(int myClock){
    if(myClock == msg[1]){
        if(tid < status.MPI_SOURCE) return true;
        else return false;
    } else {
        if(myClock < msg[1]) return true;
        else return false;
    }
}

//todo random
uint32_t Process::getRandom(uint32_t min, uint32_t max){
    std::uniform_int_distribution<std::mt19937::result_type > udist(min, max);
    std::mt19937 rand;
    std::random_device randomDevice;
    std::mt19937::result_type const seedval = randomDevice(); // get this from somewhere
    rand.seed(seedval);

    std::mt19937::result_type random_number = udist(rand);

    return random_number;
}
