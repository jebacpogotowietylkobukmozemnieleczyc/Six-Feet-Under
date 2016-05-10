//
// Created by klimas on 10.05.16.
//

#include "Gravedigger.h"
#include "Process.h"
#include <random>

void Process::send(int tag) {
    msg[1] = ++clock;
    receiveFunction.at(tag);
    MPI_Send( msg, 2, MPI_INT, dest, tag, MPI_COMM_WORLD );
    printf("%d : %d : Send from %d to %d. Tag: %d\n", tid, msg[1], tid, dest ,tag);
}

void Process::receive() {
    MPI_Recv(msg, 2, MPI_INT, ROOT_ID , MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    clock = std::max(clock, msg[1])+1;
    printf("%d : %d : Receive %d from %d. Tag: %d\n", tid, clock, tid, status.MPI_SOURCE,status.MPI_TAG);
    receiveFunction.at(status.MPI_TAG)();
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


void Process::printCorpses() {
    for (int element : corpses) {
        printf("%d : %d : Corpses %d \n", tid, clock, element);
    }

}