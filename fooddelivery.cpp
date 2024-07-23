#include <iostream>
#include <unistd.h>
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include "producer.h"
#include "consumer.h"
#include "log.h"
#include "fooddelivery.h"
#include "sharedData.h"

#define NORMAL_EXIT 0

using namespace std;

// EXAMPLE CLI INPUT
// ./fooddelivery -n 110 -p 10 -s 10 -a 12 -b 18
int main(int argc, char *argv[]) {
    if (argc < 1) {
        cout << "Invalid number of arguments." << endl;
        exit(NORMAL_EXIT);
    }

    SharedData shared;

    // This section parses through the optional arguments that may have been passed into the command line upon execution of the program.
    int option;
    while((option = getopt(argc, argv, "-n:-p:-s:-a:-b:")) != -1) {
        switch (option) {
            case 'n':
                if (stoi(optarg) < 0) {
                    cout << "Production limit must be positive" << endl;
                    exit(NORMAL_EXIT);
                }
                shared.production_limit = stoi(optarg);
                break;
            case 'p':
                if (stoi(optarg) < 0) {
                    cout << "Time to sleep for the pizza producer must be positive" << endl;
                    exit(NORMAL_EXIT);
                }
                shared.timeToProducePizzaRequest = stoi(optarg);
                break;
            case 's':
                if (stoi(optarg) < 0) {
                    cout << "Time to sleep for the sandwich producer must be positive" << endl;
                    exit(NORMAL_EXIT);
                }
                shared.timeToProduceSandwichRequest = stoi(optarg);
                break;
            case 'a':
                if (stoi(optarg) < 0) {
                    cout << "Time to sleep for the consumer A must be positive" << endl;
                    exit(NORMAL_EXIT);
                }
                shared.timeToConsumeRequestA = stoi(optarg);
                break;
            case 'b':
                if (stoi(optarg) < 0) {
                    cout << "Time to sleep for consumer B must be positive" << endl;
                    exit(NORMAL_EXIT);
                }
                shared.timeToConsumeRequestB = stoi(optarg);
                break;
        }
    }

    pthread_t PizzaProd; // Pizza request producer thread
    pthread_t SandwichProd; // Sandwich request producer thread
    pthread_t DeliveryA; // Request consumer
    pthread_t DeliveryB; // Request consumer

    // Producers and consumers that will be passed into the threads so that they have knowledge of what actions they are performing and what shared data they are modifying.
    Producer PizzaProducer(Pizza, shared);
    Producer SandwichProducer(Sandwich, shared);
    Consumer ConsumerA(DeliveryServiceA, shared);
    Consumer ConsumerB(DeliveryServiceB, shared);

    // Thread creation
    if (pthread_create(&PizzaProd, nullptr, produceRequests, &PizzaProducer)) {
        cout << "Pizza production thread created unsuccessfully." << endl;
        exit(NORMAL_EXIT);
    };
    if (pthread_create(&SandwichProd, nullptr, produceRequests,  &SandwichProducer)) {
        cout << "Sandwich production thread created unsuccessfully." << endl;
        exit(NORMAL_EXIT);
    };
    if (pthread_create(&DeliveryA, nullptr, consumeRequests,  &ConsumerA)) {
        cout << "Delivery service A thread created unsuccessfully." << endl;
        exit(NORMAL_EXIT);
    };
    if (pthread_create(&DeliveryB, nullptr, consumeRequests,  &ConsumerB)) {
        cout << "Delivery service B thread created unsuccessfully." << endl;
        exit(NORMAL_EXIT);
    };

    // This barrier will prevent the main thread from exiting until the production limit is reached, then final stats will be submitted.
    sem_wait(&shared.barrier);
    log_production_history(shared.request.produced, shared.consumerStats);
    return 0;
}