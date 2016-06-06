#include <mpi.h>
#include <stdio.h>
#include "Mpi.h"
#include <ctime>


int main(int argc, char **argv)
{
    srand(time(NULL));
    Mpi mpi;
    mpi.init(argc, argv);
    mpi.run();
    mpi.finalize();
}
