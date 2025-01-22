CXX = g++
CXXFLAGS = -g -std=c++17

all: server client

server: server.o cheat_file.o
	$(CXX) -o server server.o cheat_file.o

server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c server.cpp

client: client.o cheat_file.o
	$(CXX) -o client client.o cheat_file.o

client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c client.cpp

# cheat_file: cheat_file.o
# 	$(CXX) $(CXXFLAGS) -o cheat_file.o

cheat_file.o: cheat_file.cpp cheat_file.h
	$(CXX) $(CXXFLAGS) -c cheat_file.cpp

clean:
	rm -f *.o server client server_info
