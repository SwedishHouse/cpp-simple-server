CXX = g++
CXXFLAGS = -g -std=c++17

all: server client

server: server.o
	$(CXX) -o server server.o 

server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c server.cpp

client: client.o
	$(CXX) -o client client.o

client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c client.cpp

clean:
	rm -f *.o server client
