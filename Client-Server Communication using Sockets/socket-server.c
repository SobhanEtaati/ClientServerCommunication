#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

void error(const char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]){

	if(argc < 2){
		fprintf(stderr , "Port number not provided!\n");
		exit(1);
	}
	
	int sockfd , newsockfd , portno , n;
	int client_num = 0;
	char buffer[8192];
	char * listening_address;
	pid_t childpid;
	struct sockaddr_in serv_addr , cli_addr;
	socklen_t clilen;

	//AF_INET is for IPv4 Domain,  SOCK_STREAM is defining that we are using TCP, 0 is for protocol
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("Error opening socket!\n");
	}else{
		printf("opening socket successful!\n");
		printf("%s\n",argv[1]);
		printf("%s\n",argv[2]);
	}

	//clearing the data in n bytes of the memory starting at the location pointed to
	bzero((char *) &serv_addr , sizeof(serv_addr));
	
	portno = atoi(argv[2]);
	listening_address = argv[1];
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = inet_addr(listening_address);
	
	int checking = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if( checking < 0){
		error("Binding failed!\n");
		exit(1);
	}else{
		printf("binding successful!\n");
	}
	
	// 5 is for the maximum limit of clients that can connect to server at the same time
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	
	while(1){
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

		if(newsockfd < 0){
			error("Error on accepting!\n");
			exit(1);
		}else {
			printf("Successfully accepted!\n");
			client_num++;
			printf("There are %d clients connected to the server!\n", client_num);
		}
		
		if((childpid = fork()) == 0){
			close(sockfd);
			
			while(1){
			bzero(buffer , 8192);
			n = read(newsockfd , buffer , 8192);
			if(n < 0){
				error("error on reading!\n");
			}else{
				int i = strcmp(buffer , "bye");
				if(i == 0){
					printf("Disconnected!\n");
					break;
				}else{
					printf("client : %s\n" , buffer);
					bzero(buffer , 8192);
					
					strcpy(buffer , "accepted!\n");
					printf("-> %s\n" , buffer);
					
					n = write(newsockfd , buffer, strlen(buffer));
					if(n < 0)
						error("error on writing!\n");
				}
				
			}
			
			
			}
		}
		
		
		
		
	}

	close(newsockfd);
	close(sockfd);
	return 0;
	
}
