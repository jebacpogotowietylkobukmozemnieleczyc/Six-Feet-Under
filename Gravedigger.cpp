//

// Created by klimas on 10.05.16.
//

#include "Gravedigger.h"



void Gravedigger::eraseCorpse(int corpse){
    for(int i = 0; i < corpses.size(); ++i) if(corpses[i] == corpse) { corpses.erase(corpses.begin()+i); break; }
}

void Gravedigger::chooseRandomCorpse(){
    corpseAckSet.clear();
    if(!corpses.empty()){
        int tmp = getRandom(0, corpses.size()-1);
        //printf("Process %d, - My random corpse number: %d t is #%d\n", tid, tmp, corpses.at(tmp));
        //Gravedigger::printCorpseList();
        myCorpse = corpses.at(tmp);
        corpses[tmp] = std::move(corpses.back());
        corpses.pop_back();
        msg[0] = myCorpse;
        myCorpseClock = clock+1;
        Process::sendAll(CORPSE_REQ);
    } else {
        myCorpse = -1;       
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
    for (int i = 2; i < (status.MPI_TAG - 10); ++i) {
         //printf("Process %d - msg[%d]=%d\n", tid, i, msg[i]);
         corpses.push_back(msg[i]);
    }
    //Gravedigger::printCorpseList();
    if (myCorpse == -1) {
        Gravedigger::chooseRandomCorpse();
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
            Gravedigger::chooseRandomCorpse();
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
    Gravedigger::answerWhileWaiting(getRandom(BURIAL_MIN_TIME, BURIAL_MAX_TIME));
    printf("Id: %d | Clock: %d | %d buried Corpse: #%d\n", tid, clock, tid, myCorpse);
    myClerkClock = clock + 1;
    Process::sendAll(CLERK_REQ);
}

void Gravedigger::clerkRequest(){
    if(!meFirst || waitingForCorpse) Process::send(CLERK_ACK, status.MPI_SOURCE);

    if(meFirst){
        paperworkQueue.push(status.MPI_SOURCE);
        Gravedigger::clerkAcknowledge();
    }
}

void Gravedigger::clerkAcknowledge(){
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
    printf("Id: %d | Clock: %d | %d done paperwork about Corpse: #%d\n", tid, clock, tid, myCorpse);
    Gravedigger::answerWhileWaiting(getRandom(PAPERWORK_MIN_TIME, PAPERWORK_MAX_TIME));

    Gravedigger::makeQueueEmpty();
    Gravedigger::chooseRandomCorpse();
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

