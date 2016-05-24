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

void Gravedigger::addNewCorpse() {
    if(msg[0]>=999)
    corpses.push_back(msg[0]);
    for (int i = 2; i < status.MPI_TAG - 10; ++i) {
        if(msg[i]>=999)
        corpses.push_back(msg[i]);
    }
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
    printf("Id: %d | Clock: %d | %d buried Corpse: #%d\n", tid, clock, tid, myCorpse);
    usleep(getRandom(BURIAL_MIN_TIME, BURIAL_MAX_TIME)*1000);

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
    usleep(getRandom(PAPERWORK_MIN_TIME, PAPERWORK_MAX_TIME)*1000);

    Gravedigger::makeQueueEmpty();
    Gravedigger::chooseRandomCorpse();
}

void Gravedigger::makeQueueEmpty(){
    while(!paperworkQueue.empty()){
        Process::send(CLERK_ACK, paperworkQueue.front());
        paperworkQueue.pop();
    }
}

void Gravedigger::run() {
    while (true){
        Process::receive();
    }
}

