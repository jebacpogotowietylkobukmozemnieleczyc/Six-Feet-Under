//
// Created by klimas on 10.05.16.
//

#include "Management.h"

void Management::run() {   
    unsigned int i = 999;

    while (true) {
        msg[0] = ++i;

        random = rand()%MAX_CORPSE_LIST_SIZE;
        if(random > MAX_CORPSE_LIST_SIZE) {
            printf("COŚ SIĘ POPSUŁO: random=%d\n", random);
            random = MAX_CORPSE_LIST_SIZE;
        }

        for (int j = 2; j <MAX_CORPSE_LIST_SIZE+2; ++j) {
            (j < (random+2)) ? msg[j]=++i : msg[j]=0;
        }
        Management::printMsg();
        Process::sendAll(random+12, MAX_CORPSE_LIST_SIZE+2);
        usleep((rand()%MURDER_MAX_TIME+MURDER_MIN_TIME)*1000);
    }
}

void Management::printMsg(){
    printf("Management %d - My corpses list to send: random=%d:\n", tid, random);
    for (int i = 0; i < MAX_CORPSE_LIST_SIZE + 2; ++i){
        printf("#%d\n", msg[i]);
    }
    printf("FIN --------\n");
}
