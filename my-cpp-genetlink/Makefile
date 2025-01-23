CXX = g++

CXXFLAGS = -g -std=c++17 -Imy-cpp-genetlink

all: server client

server: server.o request_processing.o pid_manager.o
	$(CXX) -o server server.o request_processing.o pid_manager.o

server.o: server.cpp 
	$(CXX) $(CXXFLAGS) -c server.cpp

client: client.o request_processing.o pid_manager.o
	$(CXX) -o client client.o request_processing.o pid_manager.o

client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c client.cpp

request_processing.o: request_processing.cpp
	$(CXX) $(CXXFLAGS) -c request_processing.cpp

pid_manager.o: pid_manager.cpp
	$(CXX) $(CXXFLAGS) -c pid_manager.cpp

clean:
	rm -f *.o server client pid_info
