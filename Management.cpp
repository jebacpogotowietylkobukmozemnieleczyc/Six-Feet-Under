//
// Created by klimas on 10.05.16.
//

#include "Management.h"


void Management::run() {
    do {
        msg[0] = getRandom(0, MAX_ID);
    } while (corpses.count(msg[0]));
    corpses.insert(msg[0]);
    for (int i = 0; i < size - 1; ++i) {
        dest = i + 1;
        Process::send(0);
    }
}
