#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>


using namespace std;
enum msg_type {
     RRQ=1,
     WRQ,
     DATA,
     ACK,
     ERROR
};

struct msg_tftp{
	uint16_t msg_type;
	uint8_t DATA[514];
};

struct tftp_data{
	uint16_t msg_type;
	uint16_t packetNUM;
	uint8_t data[512];
	
};

struct tftp_ACK{
	uint16_t msg_type;
	uint16_t packetNUM;	
};

typedef union{

	msg_tftp request; 
	tftp_data data_tranfer;
	tftp_ACK acknow;

}tftp_message;


bool file_exists(string file){
	if(ifstream(file)){
		return true;
	}
	else{
		return false;
	}
};

void handle_child(tftp_message *temp, ssize_t msg, struct sockaddr_in *clientsock, socklen_t clientlen){

	int newsockfd;
	uint16_t mess_type;
	FILE *fd;
	fstream fs;

	if((newsockfd = socket(AF_INET, SOCK_DGRAM, 0))==-1){
		printf("ERROR SOCKET CHILD");
		exit(1);
	}
	printf("filename: ");

	char *filename = ((char*) temp->request.DATA);
	mess_type = ntohs(temp->request.msg_type);
	char *transfertype = strchr(filename, '\0') + 1;
	string ttstring(transfertype);
	string filestring(filename);
	cout<<filestring;
	printf(" Type : ");
	cout<<ttstring<<endl;

	if(!file_exists(filestring)){
		printf("FILE NOT FOUND");
		exit(1);
	}
	if (ttstring.compare("netascii")==0){
		//fopen
	}

	fd = fopen(filename, "r");
	if(fd == NULL){
		printf("ERROR: fopen");
		exit(1);
	}

	if(mess_type == RRQ){
		tftp_message inboundmsg;
		//tftp_message outboundmsg;
		ssize_t read_length;
		int tempp;
		uint8_t out_data[512]; 
		uint16_t NUMpacket = 1;
		bool data_remaining = true;

		while(data_remaining){
			tftp_message outboundmsg;
			
			//cout << "before read" << endl;
			read_length = fread(out_data,1,sizeof out_data, fd);
			//printf("Out data: %s\n", out_data);
			cout << "Read lenght" << read_length << endl;
			if (read_length < 512){
				data_remaining = false;
			}
			outboundmsg.data_tranfer.msg_type = htons(DATA);
			outboundmsg.data_tranfer.packetNUM = htons(NUMpacket);
			NUMpacket++;
			memcpy(outboundmsg.data_tranfer.data, out_data, read_length);
			//cout << "IN loop iter:" << NUMpacket << endl;
			//cout << "data_remaining "<< data_remaining << endl;
			int attempts=0;
			//cout << "attempts" << attempts << endl;
			while(attempts<9){
				printf("HERE");
				//tempp = sendto(newsockfd ,&outboundmsg, (4+read_length) ,0, (struct sockaddr *)&clientsock, clientlen);
				if((tempp = sendto(newsockfd ,&outboundmsg, (4+read_length) ,0, (struct sockaddr *)&clientsock, clientlen)) <0){
				 	printf("ERROR sendto in RRQ loop\n");
				 	//exit(0);
				// 	//continue;
				 }
				if((tempp = recvfrom(newsockfd, &inboundmsg, sizeof inboundmsg, 0, (struct sockaddr*)&clientsock, &clientlen))< 0){
					printf("ERROR: RECV in RRQ loop\n");
				}
        		attempts++;
        		//printf("inboundmsg: ");
        		//printf("Received req: %d and %s \n", htons(inboundmsg.acknow.msg_type), htons(inboundmsg.acknow.packetNUM));

			}
			//cout << "After loop" << endl;

		}



	}
	else if(mess_type == WRQ){
		//lol i would get my partner to do this but he does nothing
	}
	//printf("Transfer done");
	//fclose(fd);
	//close(newsockfd);
	//exit(0);
}

int main(int argc, char *argv[])
{	
	if(argc =! 2){
		printf("Please enter ./server (port number)");
		exit(0);
	}

	int port_num = atoi(argv[1]);

	int sockfd;
	struct sockaddr_in serveraddr;
	int temp;



	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0){
		printf("ERROR: Creation of Socket failed");
		exit(1);
	}
	
	memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port_num);

    if(bind(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr))<0){
    	printf("ERROR: Socket Bind failed");
		exit(1);	
    }
    printf("Server is listening\n");


	while(true){
		struct sockaddr_in clientsock;
		socklen_t clientlen = sizeof(clientsock);

		tftp_message message;
		int16_t msg_type;
		ssize_t msg;

		if((msg = recvfrom(sockfd, &message, sizeof message, 0, (struct sockaddr*)&clientsock, &clientlen))< 0){
			printf("ERROR: RECV");
		}
		////cout << "message value " << msg << endl;
		//printf("Received req: %d and %s\n", htons(message.request.msg_type), message.request.DATA);
		msg_type = ntohs(message.request.msg_type);
		if (msg_type == RRQ || msg_type == WRQ) {
               if (fork() == 0) {
                    handle_child(&message, msg, &clientsock, clientlen);
                    exit(0);
               }
          }
          else {
               printf("ERROR: msg_type \n");
          }

	}

}