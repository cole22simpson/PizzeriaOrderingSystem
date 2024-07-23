#include <pthread.h>
#include <iostream>
#include <queue>
#include <semaphore.h>
#include "log.h"
#include "fooddelivery.h"

using namespace std;

#ifndef SHAREDDATA_H
#define SHAREDDATA_H

struct SharedData {
    public:
        sem_t unconsumedRequests, availableBrokerSlots, availableSandwichRequests, mutex_lock, barrier;
        queue<RequestType> brokerQueue;
        int producedRequests;
        int finishedRequests;
        int brokerLimit;
        int maxNumOfSandwichRequests;

        int production_limit;
        int timeToProducePizzaRequest;
        int timeToProduceSandwichRequest;
        int timeToConsumeRequestA;
        int timeToConsumeRequestB;

        RequestAdded request;
        RequestRemoved removedRequestA;
        RequestRemoved removedRequestB;

        unsigned int**consumerStats;

        SharedData();
};

#endif // SHAREDDATA_H