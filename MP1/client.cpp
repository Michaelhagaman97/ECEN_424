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
ssize_t readline(int fd, char *Vpointer, size_t temp);

int main(int argc, char * argv[]){
	if(argc =! 2){
		printf("Please enter ./echos (IP)(port number)");
		return(0);
	}
	int port = atoi(argv[2]);
	string IP = argv[1];
	int status;
	int sockfd;
	char in_buffer[32];
	char out_buffer[32];
	string temp_string;
	bool exit1 = false;
	struct addrinfo hints;
	struct addrinfo *servinfo;  

	memset(&hints, 0, sizeof hints); 
	hints.ai_family = AF_UNSPEC;     
	hints.ai_socktype = SOCK_STREAM; 

	if ((status = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
    	fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    	return(0);
	}
	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	if(connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)<0){
	cout << "Failed to connect" << endl;}

	//while(!exit1){
	while(fgets(out_buffer, 32, stdin) != NULL){ 
		//printf("Type a message to the server:\n");
		writeIN(sockfd, out_buffer, 32);
		if(readline(sockfd, in_buffer, 32)==0){
			printf("Server Terminated");
			exit1 = true;
	}
		fputs(in_buffer, stdout);

	}

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

ssize_t readline(int fd, char *Vpointer, size_t temp){

	ssize_t n;
	ssize_t rc;
	char c;
	char *pointer;

	pointer = Vpointer;

	for(n=1;n<temp;n++){
		again:
		 if((rc = read(fd, &c, 1)) == 1){
		 *pointer++ = c;
		 if (c == '\n')
		 break;
		 }
		 else if (rc == 0){
		 *pointer = 0;
		 return(n-1);
		 }
		 else{
		 if(errno == EINTR)
		 	goto again;

		return(-1);
		 }
	}
	*pointer = 0;
	return(n);

}
