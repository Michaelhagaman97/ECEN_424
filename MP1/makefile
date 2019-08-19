all: server.o client.o
	g++ -std=c++14 -g server.o -o echos
	g++ -std=c++14 -g client.o -o echo

server.o: server.cpp
	g++ -std=c++14 -c -g server.cpp

client.o: client.cpp
	g++ -std=c++14 -c -g client.cpp
	
clean:
	rm -rf *.o echos echo fifo*