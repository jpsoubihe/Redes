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
#define MAXDATASIZE 10000 // max number of bytes we can get at once
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
  char nome[30];
  char senha[12];
  int len;
  FILE *arq;



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
    break;
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

  if (send(sockfd,nome,strlen(nome), 0) == -1) //SENDS username
    perror("send");


  if ((numbytes = recv(sockfd, menu, MAXDATASIZE-1, 0)) == -1) { //RECEIVES menu
    perror("recv");
 		exit(1);
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
      //FILE *arq;
      char str[100];
      strcpy(str,"PerfisDe");
      scanf("%*[^\n]"); scanf("%*c");
      scanf("%[^\n]", buf);

      gettimeofday(&tv1,NULL);

      if (send(sockfd,buf,strlen(buf), 0) == -1) //SENDS curso
      perror("send");
      strcat(str,buf);
      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) { //RECEIVES profiles
        perror("recv");
        exit(1);
      }

      gettimeofday(&tv2,NULL);

      strcat(str,".txt");
      arq = fopen(str,"w"); //WRITES a file to save the answer from the server
      buf[numbytes] = '\0';
      fprintf(arq,"%s",buf);
      rewind(arq);
      fclose(arq);
      printf("%s",buf); // PRINTS in the client screen
      memset(buf,0,strlen(buf));
    }

      else if(op == 2){
        //FILE *arq_aux;
        char city[40];
        strcpy(city,"HabilidadesEm");
        scanf("%*[^\n]"); scanf("%*c");
        memset(buf,0,strlen(buf));
        scanf("%[^\n]", buf);

        gettimeofday(&tv1,NULL);

        if (send(sockfd,buf,strlen(buf), 0) == -1) //SENDS city
          perror("send");

        strcat(city,buf);
        memset(buf,0,strlen(buf));
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) { //RECEIVES habilities
          perror("recv");
          exit(1);
        }

        gettimeofday(&tv2, NULL);
        strcat(city,".txt");
        arq = fopen(city,"w");
        //buf[numbytes] = '\0';
        fprintf(arq,"%s",buf); //WRITES a file to save the answer from the server
        rewind(arq);
        fclose(arq);
        printf("%s",buf);
        memset(buf,0,strlen(buf));
        memset(city,0,strlen(city));
      }

      else if(op == 3){
        scanf("%*[^\n]"); scanf("%*c");
        scanf("%[^\n]", buf);

        if (send(sockfd,buf,strlen(buf), 0) == -1) //SENDS experience
          perror("send");

        memset(buf,0,strlen(buf));
      }

      else if(op == 4){
        //FILE *arq1;
        char str[40];
        strcpy(str,"ExperienciasDe");
        scanf("%*[^\n]"); scanf("%*c");
        scanf("%[^\n]", buf);

        gettimeofday(&tv1,NULL);

        if (send(sockfd,buf,strlen(buf), 0) == -1) //SENDS email
          perror("send");

        strcat(str,buf);

        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) { //RECEIVES experiences
          perror("recv");
          exit(1);
        }

        gettimeofday(&tv2,NULL);

        strcat(str,".txt");
        arq = fopen(str,"w");
        buf[numbytes] = '\0';
        fprintf(arq,"%s",buf);
        rewind(arq);
        fclose(arq);
        printf("%s",buf);
        memset(buf,0,strlen(buf));
        memset(str,0,strlen(str));
      }

      else if(op == 5){
        //FILE *arq2;
        char str[40];
        strcpy(str,"LogDePerfis");

        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) { //RECEIVES everything
          perror("recv");
          exit(1);
        }

        gettimeofday(&tv2,NULL);

        strcat(str,".txt");
        arq = fopen(str,"w");
        buf[numbytes] = '\0';
        fprintf(arq,"%s",buf);
        rewind(arq);
        fclose(arq);
        printf("%s",buf);
      }

      else if(op == 6){
        char str[40];
        char d[40];
        strcpy(str,"PerfilDe");
        strcpy(d,"FotoDe");
        scanf("%s", buf);

        gettimeofday(&tv1,NULL);

        if (send(sockfd,buf,strlen(buf), 0) == -1) //SENDS email
          perror("sendz");

        strcat(str,buf); // nome do arquivo que guarda o .txt de ze

        strcat(d,buf); //nome do arquivo que guarda o .jpg de ze


        numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0);

        strcat(str,".txt");
        arq = fopen(str,"w");
        buf[numbytes] = '\0';
        fprintf(arq,"%s",buf);
        rewind(arq);
        fclose(arq);
        printf("%s",buf); //imprime o perfil no terminal cliente
        memset(buf,0,strlen(buf));
        gettimeofday(&tv2,NULL);

          //IMAGEM

        /* tamanho = read(sockfd, &received_int, sizeof(received_int));
          tamanho = ntohl(received_int);
          printf("tamanho da imagem = %d\n", tamanho);
          if ((numbytes = recv(sockfd, buf, tamanho, 0)) == -1) { //RECEIVES profile picture
            perror("recv");
            exit(1);
          }

          printf("numbytes img = %d", numbytes);

          gettimeofday(&tv2,NULL);

          strcat(d,".jpg");

          img = fopen(d,"wb");

          //buf[numbytes] = '\0';

          fprintf(img,"%u",buf);

          //rewind(img);
          printf("\nabriu o arquivo");
          fclose(img);

          memset(buf,0,strlen(buf));*/
        }
    printf("\n");
    double tempo = (tv2.tv_sec - tv1.tv_sec) + ((tv2.tv_usec - tv1.tv_usec)/1000000.0);
    printf("Tempo de consulta: %lf\n", tempo);
  }
  close(sockfd);
    return 0;
}
