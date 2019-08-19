#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>

using namespace std;
void parse_buffer(char *temp_buffer);

struct SBCP_Header
{
	int8_t vrsn;	//version
	int8_t type;	//type
	int16_t len;	//message length
	char payload[512]; //actual chat text

};

SBCP_Header fromclient; 
int type_test;

int main(int argc, char * argv[]){
	if(argc =! 3){
		printf("Please enter ./server (IPsddress)(port number)(Max Client Num");
		exit(0);
	}

	string IP_adr = argv[1];
	int port_num = atoi(argv[2]);
	int max_client = atoi(argv[3]);
	cout << "IP Address: " << IP_adr << endl;
	cout << "Port: " << port_num << endl;
	cout << "Max Clients: " << max_client << endl;

	int sockfd;
	int new_sock;
	int NUM_clients = 0;
	bool cont = true;
	struct sockaddr_in server_addr;
	SBCP_Header msg;
	vector<int> client_numberid;
	vector<string> client_username;
	vector<int>::iterator iter;
	int placeholder = -1;
	//client_username.push_back("abc");
	//client_numberid.push_back(placeholder);
	int pos;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		printf("Error:Open Socket\n");
		return(0);
	}
	printf("Socket Created\n"); 
	
	int opt=1; 
	if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt,sizeof(opt))<0){
		printf("Error: Setting Socket\n");	
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port_num);	
		
	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) <0){
		printf("Error: Bind\n");  
	}
	printf("Socket Bound\n");
	
	struct sockaddr_in client_addr;
	//listen for client
	printf("Socket Listening\n");
	listen(sockfd,max_client);

	fd_set read_client_fds;
	fd_set read_new_client_fds;
	FD_ZERO(&read_client_fds);	
	FD_ZERO(&read_new_client_fds);

	fd_set write_client_fds;
	fd_set write_new_client_fds;
	FD_ZERO(&write_client_fds);	
	FD_ZERO(&write_new_client_fds);

	socklen_t clientlen = sizeof(client_addr);
	int temp = sockfd;
	FD_SET(sockfd,&read_new_client_fds);

	char buffer[512];
	
	//std::vector<string> usernames;
	int buffer_size;
	int buf_len = sizeof(buffer);

	while(cont){

		//memset(buffer,0, sizeof(buffer));

		read_client_fds = read_new_client_fds;
		write_client_fds = write_new_client_fds;

		if(select(temp+1,&read_client_fds,NULL,NULL,NULL)<0){
			printf("Error: Select\n"); 
		}
		//int i = 0;
		for(int i = 0; i <= temp; i++){
		//while(i<=temp){
			if(FD_ISSET(i, &read_client_fds)){
				if(i == sockfd){
						//cout << "Num clients " << NUM_clients << endl;
						//cout << "Max Clients " << max_client << endl;
						new_sock = accept(sockfd,(struct sockaddr *) &client_addr, &clientlen);
						//printf("Client Connected\n");
						//NUM_clients++;
						FD_SET(new_sock, &read_new_client_fds); 
					if(NUM_clients < (max_client)){
						//new_sock = accept(sockfd,(struct sockaddr *) &client_addr, &clientlen);
						//printf("Client Connected\n");
						printf("Client Connected\n");
						NUM_clients++;
						//FD_SET(new_sock, &read_new_client_fds); //use main fds

						if(new_sock>temp)
							temp=new_sock;
						}
					 else {
					 	printf("Too many clients-Please Wait\n");
					 	//close(i);
					 	FD_CLR(new_sock,&read_new_client_fds);
						//FD_CLR(i,&write_new_client_fds);
						close(new_sock);
						NUM_clients--;
					}
				} else{
					memset(buffer,0, sizeof(buffer));
					int check_recv = recv(i,buffer,buffer_size,0);
					if( check_recv ==0 ){//if 0 the clinte is discontected 

						iter = find(client_numberid.begin(),client_numberid.end(),i); //find where this client is in the vector inorder to remove the username
						pos = distance(client_numberid.begin(), iter);

						cout << client_username.at(pos) << " Has left the chat" <<endl;

						//pos = pos + 1; //since abc is in the username vector
						client_username.erase(client_username.begin()+ pos);
						client_numberid.erase(client_numberid.begin());
						FD_CLR(i,&read_new_client_fds);
						FD_CLR(i,&write_new_client_fds);
						close(i);
						NUM_clients--;
						printf("Client Disconected and username cleared\n");
						//remove username
					}
					else if(check_recv < 0){
						FD_CLR(i,&read_new_client_fds);
						FD_CLR(i,&write_new_client_fds);
						close(i);
						NUM_clients--;
						printf("Client Disconected due to RECV error\n");

					}
					else{

						parse_buffer(buffer);
						//cout << "Receieved: " << type_test << endl;

						if(type_test == 50){
							//cout << fromclient.payload << " Has entered the chat" <<endl;
							string name = fromclient.payload;

							if(find(client_username.begin(), client_username.end(), name) != client_username.end()){
								//cout << client_numberid.at(pos) << " is already taken" <<endl;
								printf("username already taken\n");
								printf("Client Disconected\n");
								FD_CLR(i,&read_new_client_fds);
								close(i);
								NUM_clients--;
							}
							else{
								client_numberid.push_back(i);
								client_username.push_back(string(fromclient.payload));
								FD_SET(new_sock, &write_new_client_fds);
								cout << fromclient.payload << " Has entered the chat" <<endl;
								//cout << name << " Has entered the chat" <<endl;
							}

						}
						if(type_test == 52){
							//cout << fromclient.len << "- chat" <<endl;

							iter = find(client_numberid.begin(),client_numberid.end(),i); //find where this client is in the vector inorder to remove the username
							int pos1 = distance(client_numberid.begin(), iter);
							//pos1 = pos1 + 1; //since abc is in the username vector
							//client_username.erase(client_username.begin()+ pos1);
							string unmae = client_username.at(pos1);
							string umsg =  fromclient.payload;
							unmae = unmae + ": ";
							unmae = unmae + umsg;
							strncpy(fromclient.payload, unmae.c_str(), 512);
							char send_buf[512];
							int send_buf_size = sizeof(send_buf);
							fromclient.type = 3;//Forward
							sprintf(send_buf, "%d.%d.%d.%s", fromclient.vrsn, fromclient.len, fromclient.type, fromclient.payload);

							for(int j = 0; j<=temp;j++){
								if(FD_ISSET(j, &read_new_client_fds)){\
									//cout << "test here" << endl;
									if(j != sockfd && j != i){
										//cout << "Sending: " << umsg << endl;
										if(send(j,send_buf,send_buf_size,0)<=0){
											printf("Error: Send\n");
										}
									}
								}
							}
						}
					}

				}
			}
			//else if(FD_ISSET(i, &write_client_fds)){

			//}
			//i++;
		}

	}
}


void parse_buffer(char *temp_buffer){ //parses the message from the client and updates the header

	char temp_version;
	char temp_len;
	char temp_type;
	char temp_payload[512];
	sscanf(temp_buffer,"%[^.].%[^.].%[^.].%[^\t\n]", &temp_version, &temp_len,&temp_type,temp_payload);//parses a char array

	//i could not get a string parser to work so i ended up using the char array method with sscanf
	fromclient.vrsn = temp_version -'0'; 
	fromclient.len = temp_len -'0';
	fromclient.type = temp_type -'0';
	strncpy(fromclient.payload,temp_payload,512);
	type_test = (int)temp_type;
}
