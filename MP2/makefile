all: server.o client.o
	g++ -std=c++11 -g server.o -o server
	g++ -std=c++11 -g client.o -o client

server.o: server.cpp
	g++ -std=c++11 -c -g server.cpp

client.o: client.cpp
	g++ -std=c++11 -c -g client.cpp
	
clean:
	rm -rf *.o server client fifo*
