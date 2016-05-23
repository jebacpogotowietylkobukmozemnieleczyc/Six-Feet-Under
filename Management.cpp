//
// Created by klimas on 10.05.16.
//

#include "Management.h"

void Management::run() {   
    unsigned int i = 1000;

    while (true) {
        msg[0] = i;
        Process::sendAll(NEW_CORPSE);
        usleep(getRandom(MURDER_MIN_TIME, MURDER_MAX_TIME)*1000);
        i++;
    }
}
