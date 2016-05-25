//
// Created by klimas on 10.05.16.
//

#include "Gravedigger.h"
#include "Process.h"
#include "pcg_random.hpp"
#include <random>

void Process::send(int tag, int dest) {
    msg[1] = ++clock;
    MPI_Send( msg, 2, MPI_INT, dest, tag, MPI_COMM_WORLD );
    //printf("Id: %d | Clock: %d | Send from %d to %d | Tag: %d Corpse: #%d | myCorpse #%d\n", tid, msg[1], tid, dest ,tag, msg[0], myCorpse);
}

void Process::sendAll(int tag, int tabSize){
    msg[1] = ++clock;
    for( int i = 1; i < size; ++i){
        if (i != tid){
            MPI_Send( msg, tabSize, MPI_INT, i, tag, MPI_COMM_WORLD );
            //printf("Id: %d | Clock: %d | Send from %d to %d | Tag: %d Corpse: #%d | myCorpse #%d\n", tid, msg[1], tid, i ,tag, msg[0], myCorpse);
        }
    }
}

void Process::receive() {
    int whichFunction;
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &statusProbe);
    //printf("Process %d - ProbeStatus[Source: %d, Tag(-10): %d]\n", tid, statusProbe.MPI_SOURCE, statusProbe.MPI_TAG-10);
    if(statusProbe.MPI_SOURCE == 0){
        MPI_Recv(msg, (statusProbe.MPI_TAG-10), MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        whichFunction=0;
    }
    else{
        MPI_Recv(msg, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == 1) meFirst = meFirstFunc(myCorpseClock);
        if (status.MPI_TAG == 3) meFirst = meFirstFunc(myClerkClock);
        whichFunction=status.MPI_TAG;
    }
    clock = std::max(clock, msg[1]) + 1;
    //printf("Id: %d | Clock: %d | %d received from %d | Tag: %d Corpse: #%d | myCorpse #%d\n", tid, clock, tid, status.MPI_SOURCE,status.MPI_TAG, msg[0], myCorpse);
    receiveFunction.at(whichFunction)();
}

bool Process::meFirstFunc(int myClock){
    return myClock == msg[1] ? tid < status.MPI_SOURCE : myClock < msg[1];
}

//todo random
uint32_t Process::getRandom(uint32_t min, uint32_t max){
    std::uniform_int_distribution<std::mt19937::result_type > udist(min, max);
    std::mt19937 rand;
    std::random_device randomDevice;
    std::mt19937::result_type const seedval = randomDevice(); // get this from somewhere
    rand.seed(seedval);

    std::mt19937::result_type random_number = udist(rand);


//    pcg_extras::seed_seq_from<std::random_device> seed_source;

    // Make a random number engine
//    pcg32 rng(seed_source);

    // Choose a random mean between 1 and 6
//    std::uniform_int_distribution<uint32_t > uniform_dist(min, max);
//     uniform_dist(rng);
    return  random_number;
}
