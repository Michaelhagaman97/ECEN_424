all: MP3.o 
	g++ -std=c++11 -g MP3.o -o MP3

server.o: MP3.cpp
	g++ -std=c++11 -c -g MP3.cpp
	
clean:
	rm -rf *.o MP3 fifo*
