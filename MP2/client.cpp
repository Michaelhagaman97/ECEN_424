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


struct SBCP_Header
{
	int8_t version;
	int8_t type;	//type
	int16_t len;
	char payload[512]; //actual chat text
};
void parse_buffer(char *temp_buffer);
SBCP_Header fromserver; 
int type_test;

int main(int argc, char * argv[]){
	if(argc =! 3){
		printf("Please enter ./client  (username)(IP)(port number)");
		return(0);
	}
	string username = argv[1];
	int port_num = atoi(argv[3]);
	string IP_adr = argv[2];

	cout << "username: " << username << endl;
	cout << "port: " << port_num << endl;
	cout << "IP Address: " << IP_adr << endl;

	int sockfd;
	int connfd;
	struct sockaddr_in servaddr;
	struct sockaddr_in clientaddr;
	bool cont = true;
	char temp_buffer[1000];
	char firstmsg[1000];
	int temp_buffer_size; 

	sockfd = socket(AF_INET, SOCK_STREAM,0);
	if(sockfd < 0){
		printf("Error: Socket not created\n");
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port_num);	

	if(connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) != 0){
		printf("Error: Connection failed\n");
		exit(0);
	}
	strcpy(temp_buffer, username.c_str());
	SBCP_Header msg;
	temp_buffer_size = username.size();

	msg.version = 3;
	msg.type = 2;//join
	msg.len = temp_buffer_size;
	for(int i = 0; i < temp_buffer_size; i++){
		msg.payload[i] = temp_buffer[i];
		//cout << temp_buffer[i]<< " ";
	}
	sprintf(firstmsg, "%d.%d.%d.%s", msg.version, msg.len, msg.type, msg.payload);
	//send message and test if it works
	if(send(sockfd, firstmsg, sizeof(firstmsg),0)<0){
		printf("Error: Send");
	}


	fd_set clientfd;
	FD_ZERO(&clientfd);
	FD_SET(0,&clientfd);
	FD_SET(sockfd,&clientfd);

	//fd_set temp_clientfd;
	//FD_ZERO(&temp_clientfd);
	//FD_SET(sockfd,&temp_clientfd);
	int temp;

	//cout << "herer" << endl;
	while(cont){
	//cout << "herer in loop" << endl;
		temp = sockfd;
	//	clientfd = temp_clientfd;
		if(select(temp+1,&clientfd,NULL,NULL,NULL)<0){
			printf("Error: Select\n"); 
			exit(0);
		}
		else{
			//cout << "here else one" << endl;
			//for i < socketfd
			//for(int i = 0; i < sockfd; i++){

						if(FD_ISSET(sockfd, &clientfd)){ //from server recv
				//cout << "here if one" << endl;
				int check = recv(sockfd,temp_buffer,1000,0);
				if(check<0){
					printf("Error: RECV\n");
					exit(0);
				}
				if(check = 0){
					printf("Disconnected from server: RECV\n");
					exit(0);

				}
				parse_buffer(temp_buffer);
				cout << fromserver.payload << endl;
				memset(temp_buffer,0, sizeof(temp_buffer));
				//handle message 
				//print message
			}
			if(FD_ISSET(0, &clientfd)){ //send
				//configure header
				//send message
				//cout << "here if two" << endl;
				char chat[1000];
				string message;

				//memset(temp_buffer,0, sizeof(temp_buffer));
				//memset(send_msg.payload,0, sizeof(send_msg.payload));
				SBCP_Header send_msg;
				getline(cin, message);
				send_msg.version = 3;
				send_msg.type = 4;
				send_msg.len = message.size()+4;
				strcpy(temp_buffer, message.c_str());
				for(int i = 0; i < message.size(); i++){
					send_msg.payload[i] = temp_buffer[i];
					//cout << temp_buffer[i]<< " ";
				}
				
				sprintf(chat, "%d.%d.%d.%s", send_msg.version, send_msg.len, send_msg.type, send_msg.payload);
				//send message and test if it works
				if(send(sockfd, chat, sizeof(chat),0)<0){
					printf("Error: Send");
				
				}
				memset(send_msg.payload,0, sizeof(send_msg.payload));
				memset(temp_buffer,0, sizeof(temp_buffer));



			}
			FD_SET(0,&clientfd);
			FD_SET(sockfd,&clientfd);
		//}
		}
	}
	close(sockfd);
	return 0;
}
void parse_buffer(char *temp_buffer){ //parses the message from the client and updates the header

	char temp_version;
	char temp_len;
	char temp_type;
	char temp_payload[512];
	sscanf(temp_buffer,"%[^.].%[^.].%[^.].%[^\t\n]", &temp_version, &temp_len,&temp_type,temp_payload);//parses a char array

	//i could not get a string parser to work so i ended up using the char array method with sscanf
	//cout << temp_payload << " Does it work";
	fromserver.version = temp_version -'0'; 
	fromserver.len = temp_len -'0';
	fromserver.type = temp_type -'0';
	strncpy(fromserver.payload,temp_payload,512);
	type_test = (int)temp_type;

}
