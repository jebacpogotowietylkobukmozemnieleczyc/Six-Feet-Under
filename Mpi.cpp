//
// Created by klimas on 10.05.16.
//

#include <mpi.h>
#include "Mpi.h"
#include "Gravedigger.h"
#include "Management.h"

void Mpi::init(int &argc, char** &argv) {
    int size,tid;
    MPI_Init(&argc, &argv); //Musi być w każdym programie na początku
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Comm_rank( MPI_COMM_WORLD, &tid );
    if ( tid == ROOT_ID) {
        process = std::unique_ptr<Process>(new Management(tid, size));
    }
    else
    {
        process = std::unique_ptr<Process>(new Gravedigger(tid, size));
    }
}

void Mpi::run() {
    process->run();
}

void Mpi::finalize() {
    MPI_Finalize();
}
