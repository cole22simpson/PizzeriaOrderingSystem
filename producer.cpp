#include "producer.h"

// Producer constructor
Producer::Producer(RequestType type, SharedData& sharedData) : requestType(type), commonData(sharedData) {}

// Accessor for retrieving the request type. Intended for console output purposes.
string getRequestType(RequestType type) {
    switch (type) {
        case Pizza:
            return "Pizza";
        case Sandwich:
            return "Sandwich";
        default:
            return "Unknown";
    }
};

/*
 * Threads will run this function with different types of requests being produced. The function uses semaphores to
 * keep track of available resources. One semaphore will only allow 20 requests to be in the queue at any time
 * and another semaphore will make sure that there is a maximum of 8 sandwiches in the queue at any time to
 * encourage pizza sales. Production of a request is simulated by making the thread sleep. Sleep durations
 * are decided by optional command line arguments.
 */
void* produceRequests(void *producerType) {
    Producer* producer = static_cast<Producer*>(producerType);
    int microToMilli = 1000; // 1000 microseconds in a millisecond
    while(producer->commonData.producedRequests < producer->commonData.production_limit-1) {
        if (producer->requestType == Pizza) {
            usleep(microToMilli * producer->commonData.timeToProducePizzaRequest);
        } else {
            usleep(microToMilli * producer->commonData.timeToProduceSandwichRequest);
            sem_wait(&producer->commonData.availableSandwichRequests);
        }
        sem_wait(&producer->commonData.availableBrokerSlots);
        sem_wait(&producer->commonData.mutex_lock);
        producer->commonData.brokerQueue.push(producer->requestType);
        producer->commonData.producedRequests++;
        producer->commonData.request.type = producer->requestType;
        producer->commonData.request.inBrokerQueue[producer->requestType]++;
        producer->commonData.request.produced[producer->requestType]++;
        log_added_request(producer->commonData.request);
        sem_post(&producer->commonData.mutex_lock);
        sem_post(&producer->commonData.unconsumedRequests);
    }
    return nullptr;
};