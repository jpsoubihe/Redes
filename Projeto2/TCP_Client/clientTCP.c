/*
** client.c -- a stream socket client demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#define PORT "3490" // the port client will be connecting to
#define MAXDATASIZE 100000 // max number of bytes we can get at once
// get sockaddr, IPv4 or IPv6:

struct timeval tv1,tv2;

void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
      return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int main(int argc, char *argv[]){
  int sockfd, numbytes;
  char buf[MAXDATASIZE];
  char menu[MAXDATASIZE];
  struct addrinfo hints, *servinfo, *p;
  int rv;
  char s[INET6_ADDRSTRLEN];
  char nome[50];
  char nome_aux[50];
  char nome_img[50];
  char senha[12];
  int len,i;
  FILE *arq;
  FILE *arqI;




  if (argc != 2) {
    fprintf(stderr,"usage: client hostname\n");
    exit(1);
  }

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and connect to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    }
    break;  char nome_aux[50];

  }

  if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }

  inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
  printf("client: connecting to %s\n", s);
  freeaddrinfo(servinfo); // all done with this structure

  if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) { // server asks for login
    perror("recv");
    exit(1);
  }

  buf[numbytes] = '\0';
  printf("%s\n",buf);

  scanf("%s", nome);
  strcpy(nome_img,nome);
  strcat(nome_img,".jpeg");

  if (send(sockfd,nome,strlen(nome), 0) == -1) //SENDS username
    perror("send");


  if ((numbytes = recv(sockfd, menu, MAXDATASIZE-1, 0)) == -1) { //RECEIVES menu
    perror("recv");
 		exit(1);
  }

  if(strcmp(menu,"Wrong") == 0){
    close(sockfd);
  }

  int loop = 1;
  int converted_number;
  int op;

  while(loop == 1){
    printf("%s\n",menu);
    scanf("%d", &op); //OP is the flag for the next operation
    converted_number = htonl(op); //PREPARES the integer to be sent through the socket
    write(sockfd, &converted_number, sizeof(converted_number)); //SENDS the operation integer to the server
    gettimeofday(&tv1,NULL); //captures the initial time of the requisition

    if(op == 0){ //SHUT DOWN the connection
      loop--;
      close(sockfd);
      return 0;
    }

    if(op == 1){

      gettimeofday(&tv1,NULL);

      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) { //RECEIVES data
        perror("recv");
        exit(1);
      }

      gettimeofday(&tv2,NULL);

      strcpy(nome_aux,nome);
      strcat(nome_aux,".txt");
      arq = fopen(nome_aux,"w"); //WRITES a file to save the answer from the server
      buf[numbytes] = '\0';
      fprintf(arq,"%s",buf);
      fclose(arq);
      printf("%s\n",buf); // PRINTS in the client screen
      memset(buf,0,strlen(buf));
      memset(nome_aux,0,strlen(nome_aux));


      //FALTA IMAGEM!!!!!!!!!!!!!!!!!

      if ((numbytes = recv(sockfd, buf, 10, 0)) == -1) { //RECEIVES data
        perror("recv");
        exit(1);
      }

      buf[numbytes] = '\0';
      int num = atoi(buf);
      printf("num = %d\n", num);
      memset(buf,0,num);

      if ((numbytes = recv(sockfd, buf, num, 0)) == -1) { //RECEIVES image
        perror("recv");
        exit(1);
      }

      i = 0;

    	arqI = fopen(nome_img,"wb"); //WRITES a file to save the image sent from the server
    	buf[numbytes] = '\0';

    	while(i < num){
    		fprintf(arqI,"%c",buf[i]);
    		i++;
    	}
      printf("tamanho da imagem = %d\n", i);

    	fclose(arqI);

    }

    double tempo = (tv2.tv_sec - tv1.tv_sec) + ((tv2.tv_usec - tv1.tv_usec)/1000000.0);
    printf("Tempo de consulta: %lf\n", tempo);
  }
  close(sockfd);
  return 0;
}
