cmake_minimum_required(VERSION 3.27)
project(PA4_2)

set(CMAKE_CXX_STANDARD 17)

add_executable(PA4_2 fooddelivery.cpp
        fooddelivery.h
        log.h
        log.cpp
        sharedData.h
        producer.cpp
        consumer.cpp
        sharedData.cpp)
