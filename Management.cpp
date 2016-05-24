//
// Created by klimas on 10.05.16.
//

#include "Management.h"

void Management::run() {   
    unsigned int i = 999;

    while (true) {
        msg[0] = ++i;

        int random = getRandom(0, MAX_CORPSE_LIST_SIZE);

        for (int j = 2; j <random+2; ++j) {
            msg[j]=++i;
        }
        Process::sendAll(random+12);
        usleep(getRandom(MURDER_MIN_TIME, MURDER_MAX_TIME)*1000);
    }
}
