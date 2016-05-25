//
// Created by klimas on 10.05.16.
//

#include "Management.h"

void Management::run() {   
    unsigned int i = 999;

    while (true) {
        msg[0] = ++i;

        random = getRandom(0, MAX_CORPSE_LIST_SIZE);
        if(random > MAX_CORPSE_LIST_SIZE) {
            printf("COŚ SIĘ POPSUŁO: random=%d\n", random);
            //random = MAX_CORPSE_LIST_SIZE;
        }

        for (int j = 2; j <random+2; ++j) {
            msg[j]=++i;
        }
        Management::printMsg();
        Process::sendAll(random+12, random+2);
        usleep(getRandom(MURDER_MIN_TIME, MURDER_MAX_TIME)*1000);
    }
}

void Management::printMsg(){
    printf("Management %d - My corpses list to send: random=%d:\n", tid, random);
    for (int i = 0; i < random + 2; ++i){
        printf("#%d\n", msg[i]);
    }
    printf("FIN --------\n");
}
