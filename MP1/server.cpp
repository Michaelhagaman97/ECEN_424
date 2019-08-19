#include <netdb.h> 
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <string> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

ssize_t writeIN(int fd, const char *Vpointer, size_t temp);
int main(int argc, char * argv[]){
	if(argc =! 2){
		printf("Please enter ./echos (port number)");
		return(0);
	}
	int port = atoi(argv[1]);
	cout << "Port: "<< port << endl;
	socklen_t addr_size;
	int sockfd;
	int status;
	pid_t childpid;
	size_t temp;
	char buffer[32];
	bool exit = false;
	struct sockaddr_storage user_addr;
	struct addrinfo hints;
	struct addrinfo *servinfo;

	memset(&hints, 0, sizeof hints); 
	hints.ai_family = AF_UNSPEC;     
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;    

	if ((status = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
    	fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    	return(0);
	}
	void *addr;
	//char ipstr[INET_ADD];
	char ipstr[INET_ADDRSTRLEN];
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)servinfo->ai_addr;
        addr = &(ipv4->sin_addr);
	inet_ntop(servinfo->ai_family, addr, ipstr, sizeof ipstr);
        printf("IP: %s\n", ipstr);


	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
	//listen
	int backlog = 5;
	if(listen(sockfd,backlog)==-1){
	cout << "ERROR Listen" << endl;	
	}
	//accept
	//cout << " here 0" << endl;	
	addr_size = sizeof user_addr;
	int new_fd = accept(sockfd, (struct sockaddr *)&user_addr, &addr_size);
	if(new_fd == -1){
	cout << "Error accepting" << endl;}
	//wait for client
	//cout << " here12" << endl;
	sleep(5);
	while(!exit){

	//if((childpid == fork()) == 0){
	//close(new_fd)
	//sleep(5);
	read(sockfd, buffer,32);
	cout << " INTHE LOOP 1" << endl;
	printf("buffer:%s",buffer);
	cout << " INTHE LOOP 2" << endl;
	write(sockfd, buffer, 32);
	}
	//exit(0)} kill child
	//cout << " out of the LOOP" << endl;
	freeaddrinfo(servinfo);

}

ssize_t writeIN(int fd, const char *Vpointer, size_t temp){

	size_t bits_remaining;
	ssize_t bits_written;
	const char *pointer;

	pointer = Vpointer;
	bits_remaining = temp;
	while(bits_remaining > 0){
		if((bits_written = write(fd, pointer, bits_remaining))<=0){
			if(bits_written < 0 && errno == EINTR)
				bits_written = 0;
			else
				return(-1);
		}
		bits_remaining -= bits_written;
		pointer += bits_written;
	}
	return(temp);
}
