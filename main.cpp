#include <mpi.h>
#include <stdio.h>
#include "Mpi.h"

int main(int argc, char **argv)
{
    Mpi mpi;
    mpi.init(argc, argv);
    mpi.run();
    mpi.finalize();
}
