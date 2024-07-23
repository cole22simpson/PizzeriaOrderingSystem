#include "consumer.h"

// Consumer constructor
Consumer::Consumer(ConsumerType type, SharedData& sharedData) : consumerType(type), commonData(sharedData) {}

// This function will return the consumer type as a string for logging purposes
string getConsumerType(ConsumerType type) {
    switch (type) {
        case DeliveryServiceA:
            return "DeliveryServiceA";
        case DeliveryServiceB:
            return "DeliveryServiceB";
        default:
            return "Unknown";
    }
};

/*
 * This function will be run by different threads to consume requests from the buffer. It will consume requests while the other threads are producing them
 * and when the producer hits the production limit, the consumers will make sure to continue extracting requests until the buffer is empty. This will ensure that
 * requests are not left unconsumed. The function utilizes semaphores for locking as well as buffer access. It waits for requests to be produced before
 * accessing the buffer. Consumption of a request is simulated by making the thread sleep. Sleep durations are decided at run time.
 */
void* consumeRequests(void* service) {
    Consumer* consumer = static_cast<Consumer*>(service);
    int microToMilli = 1000; // 1000 microseconds in a millisecond
    while(consumer->commonData.producedRequests < consumer->commonData.production_limit-1 || consumer->commonData.producedRequests > consumer->commonData.finishedRequests) {
        sem_wait(&consumer->commonData.unconsumedRequests);
        sem_wait(&consumer->commonData.mutex_lock);
        RequestType front = consumer->commonData.brokerQueue.front();
        consumer->commonData.brokerQueue.pop();
        consumer->commonData.finishedRequests++;
        /*
         * This section formats the stats for the final report. It keeps track of how many of each request type the
         * consumer is dealing with. it separates stats between the two delivery services.
         */
        if (consumer->consumerType == DeliveryServiceA) {
            consumer->commonData.removedRequestA.type = front;
            consumer->commonData.removedRequestA.consumed[front]++;
            consumer->commonData.removedRequestA.inBrokerQueue[front]--;
            log_removed_request(consumer->commonData.removedRequestA);
        } else {
            consumer->commonData.removedRequestB.type = front;
            consumer->commonData.removedRequestB.consumed[front]++;
            consumer->commonData.removedRequestB.inBrokerQueue[front]--;
            log_removed_request(consumer->commonData.removedRequestB);
        }
        sem_post(&consumer->commonData.mutex_lock);
        sem_post(&consumer->commonData.availableBrokerSlots);
        if (consumer->consumerType == DeliveryServiceA) {
            usleep(microToMilli * consumer->commonData.timeToConsumeRequestA);
        } else {
            usleep(microToMilli * consumer->commonData.timeToConsumeRequestB);
        }
        if (getRequestType(front) == "Sandwich") {
            sem_post(&consumer->commonData.availableSandwichRequests);
        }
    }
    // Once all requests are finally consumed, the barrier is notified and lifted
    sem_post(&consumer->commonData.barrier);
    return nullptr;
};