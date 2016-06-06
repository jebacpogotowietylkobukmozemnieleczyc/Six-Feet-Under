//

// Created by klimas on 10.05.16.
//

#include "Gravedigger.h"


int Gravedigger::myOrderingPos(){
    int result = 0;
    for(int i = 1; i < size; ++i){
        if(i == tid) continue;
        if(ordering[i][0] == 1){
            if(ordering[i][1] == myCorpseClock){
                if(i < tid) result++;
            } else {
                if(ordering[i][1] < myCorpseClock) result++;
            }
        }
    }
    return (result >= corpses.size()) ? -1 : result;
}


void Gravedigger::chooseRandomCorpse(){
    corpseAckSet.clear();
    if(!corpses.empty()){
        int tmp = rand()%corpses.size();
        printf("Process %d, - My random corpse number: %d is #%d\n", tid, tmp, corpses.at(tmp));
        //Gravedigger::printCorpseList();
        myCorpse = corpses[tmp];
        corpses[tmp] = std::move(corpses.back());
        corpses.pop_back();
        msg[0] = myCorpse;
        myCorpseClock = clock+1;
        Process::sendAll(CORPSE_REQ);
    } else {
        myCorpse = -1;       
    }

}

void Gravedigger::chooseCorpse(){
    corpseAckSet.clear();
    myCorpse = -1;
    if(!corpses.empty()){
        int tmp = myOrderingPos();
        printf("Process %d, - My corpse number: %d\n", tid, tmp);
        if(tmp != -1){
            myCorpse = corpses[tmp];
            corpses[tmp] = std::move(corpses.back());
            corpses.pop_back();
            msg[0] = myCorpse;
            myCorpseClock = clock+1;
            Process::sendAll(CORPSE_REQ);
        } else {
            myCorpse = -1;
        }
    }
}

void Gravedigger::printCorpseList(){
    printf("Process %d - My corpses list:\n", tid);
    for (int i = 0; i < corpses.size(); ++i){
        printf("#%d\n", corpses[i]);
    }
    printf("FIN --------\n");
}

void Gravedigger::addNewCorpse() {
    //printf("Process %d - got list with corpses size: %d\n", tid, status.MPI_TAG-10);
    corpses.push_back(msg[0]);
    for (int i = 2; i < MAX_CORPSE_LIST_SIZE+2; ++i) {
         //printf("Process %d - msg[%d]=%d\n", tid, i, msg[i]);
         if(msg[i]!=0) corpses.push_back(msg[i]);
    }
    //Gravedigger::printCorpseList();
    if (myCorpse == -1) {
        Gravedigger::chooseRandomCorpse(); //to w normalu
        //Gravedigger::chooseCorpse();
    }
}

void Gravedigger::corpseRequest(){
    if(msg[0] != myCorpse || !waitingForCorpse){
        corpses.erase(std::remove(corpses.begin(), corpses.end(), msg[0]),corpses.end());
        Process::send(CORPSE_ACK, status.MPI_SOURCE);
    }

    if((msg[0] == myCorpse) && (myCorpse != -1) && (waitingForCorpse)){
        if(meFirst){
            Gravedigger::corpseAcknowledge();
        }
        else {
            corpses.erase(std::remove(corpses.begin(), corpses.end(), myCorpse),corpses.end());
            Gravedigger::chooseRandomCorpse(); //to w normalu
            //Gravedigger::chooseCorpse();
        }
    }
}

void Gravedigger::corpseAcknowledge(){
        if(msg[0] == myCorpse && waitingForCorpse){
            corpseAckSet.insert(status.MPI_SOURCE);
            if(corpseAckSet.size() == (size-2)){
                corpseAckSet.clear();
                waitingForCorpse = false;
                Gravedigger::makeBurial();
            }
        }
}

void Gravedigger::makeBurial(){
    printf("Id: %d | Clock: %d | %d started burying Corpse: #%d\n", tid, clock, tid, myCorpse);
    Gravedigger::answerWhileWaiting(rand()%BURIAL_MAX_TIME+BURIAL_MIN_TIME);
    printf("Id: %d | Clock: %d | %d buried Corpse: #%d\n", tid, clock, tid, myCorpse);
    myClerkClock = clock + 1;
    Process::sendAll(CLERK_REQ);
}

void Gravedigger::clerkRequest(){
    //ordering[status.MPI_SOURCE][0] = 0;
    if(!meFirst || waitingForCorpse) Process::send(CLERK_ACK, status.MPI_SOURCE);

    if(meFirst){
        paperworkQueue.push(status.MPI_SOURCE);
        Gravedigger::clerkAcknowledge();
    }
}

void Gravedigger::clerkAcknowledge(){
    //ordering[status.MPI_SOURCE][0] = 1;
    if(!waitingForCorpse){
        clerkAckSet.insert(status.MPI_SOURCE);
        if(clerkAckSet.size() == (size-2)){
            clerkAckSet.clear();
            waitingForCorpse = true;
            Gravedigger::makePaperwork();
        }
    }
}

void Gravedigger::makePaperwork(){
    printf("Id: %d | Clock: %d | %d started paperwork about Corpse: #%d\n", tid, clock, tid, myCorpse);
    Gravedigger::answerWhileWaiting(rand()%PAPERWORK_MAX_TIME+PAPERWORK_MIN_TIME);
    printf("Id: %d | Clock: %d | %d done paperwork about Corpse: #%d\n", tid, clock, tid, myCorpse);

    Gravedigger::makeQueueEmpty(); //to w normalu
    //Process::sendAll(CLERK_ACK, 2);
    Gravedigger::chooseRandomCorpse(); //to w normalu
    //Gravedigger::chooseCorpse();
}

void Gravedigger::makeQueueEmpty(){
    while(!paperworkQueue.empty()){
        Process::send(CLERK_ACK, paperworkQueue.front());
        paperworkQueue.pop();
    }
}

void Gravedigger::answerWhileWaiting(int howLong){
    int flag;
    for(int i = 0; i < howLong; ++i){
        usleep(1000);
        if(MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &statusProbe))Process::receive();
    }
}

void Gravedigger::run() {
    while (true){
        Process::receive();
    }
}

