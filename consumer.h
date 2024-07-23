#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "producer.h"
#include "fooddelivery.h"
#include "sharedData.h"

using namespace std;

#ifndef CONSUMER_H
#define CONSUMER_H

struct Consumer {
    ConsumerType consumerType;
    SharedData& commonData;

    Consumer(ConsumerType type, SharedData& sharedData);
};

string getConsumerType(ConsumerType type);
void* consumeRequests(void* service);

#endif // CONSUMER_H