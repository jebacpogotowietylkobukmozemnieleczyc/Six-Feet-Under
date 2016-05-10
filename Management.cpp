//
// Created by klimas on 10.05.16.
//

#include "Management.h"

void Management::run() {
    for(int i=0;i<size-1;++i){
        MPI_Recv(msg, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("Otrzymalem %d %d od %d\n", msg[0], msg[1], status.MPI_SOURCE);
    }
}
