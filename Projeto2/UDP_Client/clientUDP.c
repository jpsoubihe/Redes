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


#define SERVERPORT "4950" // the port users will be connecting to
#define MAXBUFLEN 100000


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
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	char nome[MAXBUFLEN];
	char nome_img[MAXBUFLEN];
	FILE *arq;
	FILE *arqI;
	socklen_t addr_len;


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
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

	strcpy(nome,argv[2]);
	printf("nome = %s\n", nome);

	if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
		p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

	printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);

	printf("talker: waiting to recvfrom...\n");
	addr_len = sizeof their_addr;

	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	buf[numbytes] = '\0';

	if(strcmp(buf,"Wrong") == 0){
		close(sockfd);
		printf("Erro\n");
	}

	else
		printf("%s\n", buf);

	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
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
	//printf("%s\n", buf);

	int num = atoi(buf);
	printf("num = %d\n", num);

	memset(buf,0,strlen(buf));

	if ((numbytes = recvfrom(sockfd, buf,num , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}
	int i = 0;
	char str[MAXBUFLEN];

	strcat(nome_img,".jpeg");
	arqI = fopen(nome_img,"wb"); //WRITES a file to save the image sent from the server
	//buf[numbytes] = '\0';

	while(i < num){
		fprintf(arqI,"%c",buf[i]);
		i++;
	}

	fclose(arqI);
	
	printf("tamanho da imagem = %d\n", i);
	
	printf("tamanho do arquivo de imagem = %d\n", fileb_size(arqI,nome_img));
	//printf("%s\n",buf); // PRINTS in the client screen
	memset(buf,0,strlen(buf));

	freeaddrinfo(servinfo);

	close(sockfd);
	return 0;
}
