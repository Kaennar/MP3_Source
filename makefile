# makefile

all: dataserver client

demo: dataserver simpleclient

bounded_buffer.o: bounded_buffer.H bounded_buffer.C
	g++ -c -g bounded_buffer.C

reqchannel.o: reqchannel.H reqchannel.C
	g++ -c -g reqchannel.C

dataserver: dataserver.C reqchannel.o 
	g++ -g -o dataserver dataserver.C reqchannel.o -lpthread

client: client.C bounded_buffer.o reqchannel.o 
	g++ -g -o client client.C bounded_buffer.o reqchannel.o

simpleclient: simpleclient.C reqchannel.o
	g++ -g -o simpleclient simpleclient.C reqchannel.o
