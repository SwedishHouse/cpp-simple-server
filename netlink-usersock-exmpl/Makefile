CXX = gcc
CXXFLAGS = -g -std=c11

all: server client

server: server.o
	$(CXX) -o server server.o 

server.o: server.c
	$(CXX) $(CXXFLAGS) -c server.c

client: client.o
	$(CXX) -o client client.o

client.o: client.c
	$(CXX) $(CXXFLAGS) -c client.c


clean:
	rm -f *.o server client