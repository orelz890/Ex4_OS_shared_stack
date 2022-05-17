all: server client
client: client.cpp
	g++ -o client client.cpp

server: server.cpp stack.cpp Stack.hpp
	g++ -pthread -o server server.cpp stack.cpp

clean:
	rm -f *.o *.out server client