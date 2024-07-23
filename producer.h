#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "log.h"
#include "fooddelivery.h"
#include "sharedData.h"

using namespace std;

#ifndef PRODUCER_H
#define PRODUCER_H

struct Producer {
    RequestType requestType;
    SharedData& commonData;

    Producer(RequestType type, SharedData& sharedData);
};

string getRequestType(RequestType type);
void* produceRequests(void* producerType);

#endif // PRODUCER_H