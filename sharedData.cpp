#include "sharedData.h"

// Default values for optional command line arguments as well as other constants
#define DEF_PRODUCTION_LIMIT 100
#define DEF_TIME_TO_SLEEP_PIZZAPRODUCER 0
#define DEF_TIME_TO_SLEEP_SANDWICHPRODUCER 0
#define DEF_TIME_TO_CONSUME_A 0
#define DEF_TIME_TO_CONSUME_B 0
#define MAX_NUM_OF_SANDWICH_REQUESTS 8
#define BROKER_LIMIT 20

// The data that will be shared between producer and consumer threads
// This class initializes all default values for production limit and thread sleep durations. These may be changed at runtime.
// Requests counts for producers and consumers are initialized to tell them how long to run for
SharedData::SharedData() {
    production_limit = DEF_PRODUCTION_LIMIT;
    timeToProducePizzaRequest = DEF_TIME_TO_SLEEP_PIZZAPRODUCER;
    timeToProduceSandwichRequest = DEF_TIME_TO_SLEEP_SANDWICHPRODUCER;
    timeToConsumeRequestA = DEF_TIME_TO_CONSUME_A;
    timeToConsumeRequestB = DEF_TIME_TO_CONSUME_B;

    finishedRequests = 0;
    producedRequests = 0;
    maxNumOfSandwichRequests = MAX_NUM_OF_SANDWICH_REQUESTS;
    brokerLimit = BROKER_LIMIT;

    // These arrays are created to keep track of how many pizza and sandwich requests are produced over the duration of execution. This is used for logging.
    request.inBrokerQueue = new unsigned int[2];
    request.inBrokerQueue[Pizza] = 0;
    request.inBrokerQueue[Sandwich] = 0;
    request.produced = new unsigned int[2];
    request.produced[Pizza] = 0;
    request.produced[Sandwich] = 0;

    // This section keeps track of how many pizza and sandwich requests delivery service A consumed. This is for logging purposes.
    removedRequestA.inBrokerQueue = request.inBrokerQueue;
    removedRequestA.consumer = DeliveryServiceA;
    removedRequestA.consumed = new unsigned int[2];
    removedRequestA.consumed[0] = 0;
    removedRequestA.consumed[1] = 0;

    // This section keeps track of how many pizza and sandwich requests delivery service B consumed. This is for logging purposes.
    removedRequestB.inBrokerQueue = request.inBrokerQueue;
    removedRequestB.consumer = DeliveryServiceB;
    removedRequestB.consumed = new unsigned int[2];
    removedRequestB.consumed[0] = 0;
    removedRequestB.consumed[1] = 0;

    // This is an array of pointers that will store the final statistics of each delivery services performances. This is for logging purposes.
    consumerStats = new unsigned int*[2];
    consumerStats[0] = removedRequestA.consumed;
    consumerStats[1] = removedRequestB.consumed;

    // Initialization of semaphores that will be used for locking, barriers, and buffer access
    if (sem_init(&mutex_lock, 0, 1) == -1) {
        cout << "Lock semaphore creation failed." << endl;
    }
    if (sem_init(&barrier, 0, 0) == -1) {
        cout << "Barrier semaphore creation failed." << endl;
    }
    if (sem_init(&unconsumedRequests, 0, 0) == -1) {
        cout << "Unconsumed requests semaphore creation failed." << endl;
    }
    if (sem_init(&availableBrokerSlots, 0, 20) == -1) {
        cout << "Available broke slots semaphore creation failed." << endl;
    }
    if (sem_init(&availableSandwichRequests, 0, 8) == -1) {
        cout << "Available sandwich requests semaphore creation failed." << endl;
    }
}