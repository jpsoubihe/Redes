/*
** listener.c -- a datagram sockets "server" demo
*/
// CHECAR ESSE SITE https://devarea.com/linux-io-multiplexing-select-vs-poll-vs-epoll/#.XMr94sZ7mV4
// Arrumar select();
/*Pensar no caso do cliente mandar a requisição, o servidor receber, mas a resposta se perder no caminho ao cliente
Primitiva select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,struct timeval *timeout); - tem no beej*/

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
#include "arq.h"

#define MYPORT "4950"

// the port users will be connecting to
#define MAXBUFLEN 100000

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(void){
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	int addr_len,tam_img;
	char s[INET6_ADDRSTRLEN];
	char img[MAXBUFLEN];
	FILE *a;
	fd_set rset;
	int i;
    FileInfo *f_info;
    FileInfo file_array[] = {
      {"./ana@gmail.com.txt", NULL},
      {"./ana@gmail.com.jpg", NULL},
      {"./jpsoubihe@gmail.com.txt", NULL},
      {"./jpsoubihe@gmail.com.jpg", NULL},
      {"./julia@gmail.com.txt", NULL},
      {"./julia@gmail.com.jpg", NULL},
      {"./maria_silva@gmail.com.txt", NULL},
      {"./maria_silva@gmail.com.jpg", NULL},
      {"./silva@gmail.com.txt", NULL},
      {"./silva@gmail.com.jpg", NULL},
      {"./ze@gmail.com.txt", NULL},
      {"./ze@gmail.com.jpg", NULL}
    };

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);
	while(1){
		FD_ZERO(&rset);
		FD_SET(sockfd,&rset);
		printf("listener: waiting to recvfrom...\n");
		addr_len = sizeof their_addr;

		if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
			perror("recvfrom");
			exit(1);
		}

		printf("listener: got packet from %s\n",
		inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s));

		printf("listener: packet is %d bytes long\n", numbytes);

		buf[numbytes] = '\0';
		printf("listener: packet contains \"%s\"\n", buf);

		strcat(buf,".txt");
	  buf[numbytes + 4] = '\0';
	  printf("login de %s\n", buf);

	  int flag = -1;
	  for (i = 0; i < sizeof(file_array) / sizeof(FileInfo); i += 2){ //CHECKS Login
	  	if(compare(file_array[i].filename,buf) == 1){
	  		f_info = &file_array[i];
	  		flag = i;
	  	}
	  }

	  if(flag == -1){//Case wrong login
			strcpy(buf,"Wrong");

			if ((numbytes = sendto(sockfd, buf, strlen(buf), 0,(struct sockaddr *)&their_addr,addr_len)) == -1) {
				perror("listener: sendto");
				exit(1);
			}

			exit(1);
	   	free(f_info);
	   	close(sockfd);
		}

		strcpy(buf,"Bem Vindo!!");


		if ((numbytes = sendto(sockfd, buf, strlen(buf), 0,(struct sockaddr *)&their_addr,addr_len)) == -1) {
			perror("listener: sendto");
			exit(1);
		}

		printf("listener: sent %d bytes to client\n", numbytes);

		getNome(f_info,buf);

		if ((numbytes = sendto(sockfd, buf, strlen(buf), 0,(struct sockaddr *)&their_addr,addr_len)) == -1) {
			perror("listener: sendto");
			exit(1);
		}

			//FALTA IMPLEMENTAR SEND DA IMAGEM!!!!!!!!!!!!!!!!!!!!!
		f_info = &file_array[flag + 1];
		tam_img = fileb_size(f_info);


		sprintf(buf,"%d",tam_img);


		if ((numbytes = sendto(sockfd, buf,tam_img, 0,(struct sockaddr *)&their_addr,addr_len)) == -1) { //SENDS size of image
			perror("listener: sendto");
			exit(1);
		}

		a = fopen(f_info->filename,"rb");
		int i = 0;
		while(i < tam_img){
			buf[i] = fgetc(a);
			i++;
		}



		printf("tamanho da imagem = %d\n", i);
		printf("tamanho da imagem = %ld\n", strlen(buf));



		if ((numbytes = sendto(sockfd, buf,tam_img, 0,(struct sockaddr *)&their_addr,addr_len)) == -1) {
			perror("listener: sendto");
			exit(1);
		}
	}




	close(sockfd);
	return 0;
}
