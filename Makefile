all: server client
client: client.cpp
	g++ -o client client.cpp

server: server.cpp Stack2.cpp Stack2.hpp
	g++ -pthread -o server server.cpp Stack2.cpp

clean:
	rm -f *.o *.out server client
