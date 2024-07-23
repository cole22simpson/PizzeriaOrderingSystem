CXX=g++

CXXFLAGS=-std=c++11 -Wall -g3 -c

OBJS = fooddelivery.o producer.o consumer.o log.o sharedData.o

PROGRAM = fooddelivery

$(PROGRAM) : $(OBJS)
	$(CXX) -o $(PROGRAM) $(OBJS)

fooddelivery.o : fooddelivery.h fooddelivery.cpp
	$(CXX) $(CXXFLAGS) fooddelivery.cpp

producer.o : producer.h producer.cpp
	$(CXX) $(CXXFLAGS) producer.cpp

consumer.o : consumer.h consumer.cpp
	$(CXX) $(CXXFLAGS) consumer.cpp

log.o : log.h log.cpp
	$(CXX) $(CXXFLAGS) log.cpp

sharedData.o : sharedData.h sharedData.cpp
	$(CXX) $(CXXFLAGS) sharedData.cpp

all : $(PROGRAM)

clean :
	rm -f *.o $(PROGRAM)

