/*
** talker.c -- a datagram "client" demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>



#define SERVERPORT "4950" // the port users will be connecting to
#define MAXBUFLEN 100000

struct timeval tv1,tv2;


int fileb_size(FILE *a,char *nome){
  int len;

  a = fopen(nome,"rb");
  fseek(a,0,SEEK_END);
  len = ftell(a);
  fseek(a,0,SEEK_SET);
  fclose(a);
  return len;
}

int main(int argc, char *argv[]){
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv,n;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	char nome[MAXBUFLEN];
	char nome_img[MAXBUFLEN];
	fd_set readfds;
	struct timeval tv;
	FILE *arq;
	FILE *arqI;
	socklen_t addr_len;

	FD_ZERO(&readfds);

	if (argc != 3) {
		fprintf(stderr,"usage: talker hostname message\n");
		exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}
		FD_SET(sockfd, &readfds);
		break;
	}

	n = sockfd + 1;
	tv.tv_sec = 10;
	tv.tv_usec = 500000;


	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

	strcpy(nome,argv[2]);
        
        gettimeofday(&tv1,NULL);
  
	if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

	printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);

	printf("talker: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;

	rv = select(n, &readfds, NULL, NULL, &tv);

	if (rv == -1) {
		perror("select"); // error occurred in select()
	}
	else if (rv == 0) {
		printf("Timeout occurred! No data after 10.5 seconds.\n");
	}
	else{
		if (FD_ISSET(sockfd, &readfds)){
			if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
				perror("recvfrom");
				exit(1);
				}
		}
	}

	buf[numbytes] = '\0';

	if(strcmp(buf,"Wrong") == 0){
		close(sockfd);
		printf("Erro\n");
	}

	else
		printf("%s\n", buf);

	rv = select(n, &readfds, NULL, NULL, &tv);

	if (rv == -1) {
		perror("select"); // error occurred in select()
	}
	else if (rv == 0) {
		printf("Timeout occurred! No data after 10.5 seconds.\n");
	}
	else{
		if (FD_ISSET(sockfd, &readfds)){
			if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
				perror("recvfrom");
				exit(1);
			}
		}
	}




	buf[numbytes] = '\0';
	strcpy(nome_img,nome);


	strcat(nome,".txt");
	arq = fopen(nome,"w"); //WRITES a file to save the answer from the server
	buf[numbytes] = '\0';
	fprintf(arq,"%s",buf);
	fclose(arq);
	printf("%s\n",buf); // PRINTS in the client screen
	memset(buf,0,strlen(buf));

	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}
	buf[numbytes] = '\0';

	int num = atoi(buf);

	memset(buf,0,strlen(buf));

	rv = select(n, &readfds, NULL, NULL, &tv);

	if (rv == -1) {
		perror("select"); // error occurred in select()
	}
	else if (rv == 0) {
		printf("Timeout occurred! No data after 10.5 seconds.\n");
	}
	else{
		if (FD_ISSET(sockfd, &readfds)){
			if ((numbytes = recvfrom(sockfd, buf,num , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
				perror("recvfrom");
				exit(1);
			}
		}
	}
        
        gettimeofday(&tv2,NULL);
        
	int i = 0;

	strcat(nome_img,".jpeg");
	arqI = fopen(nome_img,"wb"); //WRITES a file to save the image sent from the server
	//buf[numbytes] = '\0';

	while(i < num){
		fprintf(arqI,"%c",buf[i]);
		i++;
	}

	fclose(arqI);

	memset(buf,0,strlen(buf));

	freeaddrinfo(servinfo);

	close(sockfd);
        
        double tempo = (tv2.tv_sec - tv1.tv_sec) + ((tv2.tv_usec - tv1.tv_usec)/1000000.0);
        printf("Tempo de atualização no cliente: %lfs\n", tempo);
	return 0;
}
