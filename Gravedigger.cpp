//
// Created by klimas on 10.05.16.
//

#include <mpi.h>
#include "Gravedigger.h"

void Gravedigger::run() {
    msg[0] = tid;
    msg[1] = size;
    MPI_Send( msg, 2, MPI_INT, ROOT_ID, MSG_TAG, MPI_COMM_WORLD );
    printf("Wyslalem %d %d do %d\n", msg[0], msg[1], ROOT_ID );
}
