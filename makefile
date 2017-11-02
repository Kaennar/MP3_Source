# makefile
all: dataserver client

demo: dataserver simpleclient

bounded_buffer.o: bounded_buffer.H bounded_buffer.cpp 
	g++ -std=c++11 -c -g bounded_buffer.cpp 

semaphore.o: semaphore.cpp semaphore.H
	g++ -std=c++11 -c -g semaphore.cpp

reqchannel.o: reqchannel.H reqchannel.cpp
	g++ -std=c++11 -c -g reqchannel.cpp

dataserver: dataserver.cpp reqchannel.o 
	g++ -std=c++11 -g -o dataserver dataserver.cpp reqchannel.o -lpthread

client: client.cpp bounded_buffer.o reqchannel.o semaphore.o 
	g++ -std=c++11 -g -o client client.cpp bounded_buffer.o reqchannel.o semaphore.o -lpthread

simpleclient: simpleclient.C reqchannel.o
	g++ -std=c++11 -g -o simpleclient simpleclient.C reqchannel.o

testclient: test.cpp bounded_buffer.o semaphore.o reqchannel.o
	g++ -std=c++11 -g -o test test.cpp bounded_buffer.o semaphore.o reqchannel.o -lpthread
