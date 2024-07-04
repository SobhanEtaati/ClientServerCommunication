/*
filename server_ipaddress portno
argv[0] file name
argv[1] server IP address
argv[2] port number
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<time.h>

void error(const char *msg){
        perror(msg);
        exit(1);
}

int main(int argc, char *argv[]){

	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[8192];
	
	if(argc < 3){
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(1);
	}
	
	portno = atoi(argv[2]);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("error opening socket!\n");
	}else{
		printf("socket opening successful!\n");
	}
	

	server = gethostbyname(argv[1]);
	if(server == NULL)
		fprintf(stderr , "no such host!\n");


	bzero((char *) &serv_addr , sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr , (char *) &serv_addr.sin_addr , server->h_length);
	serv_addr.sin_port = htons(portno);
	
	int checking = connect(sockfd , (struct sockaddr *) &serv_addr , sizeof(serv_addr));
	if(checking < 0){
		error("connection error!\n");
		exit(1);
	}else{
		printf("Successfully connected to server!\n");
	}

	while(1){
	
		bzero(buffer , 8192);
		fgets(buffer , 8192 , stdin);
		n =  write(sockfd , buffer , strlen(buffer));
		clock_t time_req = clock();
		if(n < 0)
			error("error on writing!\n");

		bzero(buffer , 8192);
		n = read(sockfd , buffer, 8192);
		if(n < 0)
			error("error on reading.\n");
		
		time_req = clock() - time_req;
		printf("server: %s  time: %ld \n", buffer, time_req);

		int i = strcmp(buffer , "exit");
		if(i == 0){
			printf("Disconnected from the server!\n");
			break;
		}
	}

	close(sockfd);
	return 0;

}
	


