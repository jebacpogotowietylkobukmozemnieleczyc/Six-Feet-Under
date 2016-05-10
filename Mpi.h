//
// Created by klimas on 10.05.16.
//

#ifndef MPI_ENTITY_H
#define MPI_ENTITY_H

#include <memory>
#include "Process.h"


class Mpi {

private:
    std::unique_ptr<Process> process;
public:
    void init(int& argc,char** & argv);
    void run();
    void finalize();
};


#endif //MPI_ENTITY_H
