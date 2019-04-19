/*
** server.c -- a stream socket server demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "arq.h"
#include <sys/time.h>

#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold
#define MAXDATASIZE 10000 // max number of bytes we can get at once

struct timeval tv1,tv2;

int sendall(int s, char *buf, int *len){
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n;
    FILE *a;

    while(total < *len) {
      n = send(s, buf+total, bytesleft, 0);
      if (n == -1) { break; }
      total += n;
      bytesleft -= n;
    }
    *len = total; // return number actually sent here
    return n==-1?-1:0; // return -1 on failure, 0 on success
}


void sigchld_handler(int s){
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
      return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int main(void){
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    int numbytes;
    char buf[MAXDATASIZE];
    char frase[1000] = "Login:";
    char frase1[] = "Bem-vindo!\nLogin:";
    int len,i,j;
    int contador;
    char *s_aux;
    FILE *a;
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
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
    }
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
        perror("server: socket");
        continue;
      }
      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
      }
      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
        close(sockfd);
        perror("server: bind");
        continue;
      }
      break;
    }
    freeaddrinfo(servinfo); // all done with this structure
    if (p == NULL)  {
      fprintf(stderr, "server: failed to bind\n");
      exit(1);
    }
    if (listen(sockfd, BACKLOG) == -1) {
      perror("listen");
      exit(1);
    }
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
      perror("sigaction");
      exit(1);
    }
    printf("server: waiting for connections...\n");
    while(1) {  // main accept() loop
      sin_size = sizeof their_addr;
      new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
      if (new_fd == -1) {
        perror("accept");
        continue;
      }
      inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
      printf("server: got connection from %s\n", s);

      if (!fork()) { // this is the child process
        close(sockfd); // child doesn't need the listener
        if (send(new_fd, frase1, strlen(frase1), 0) == -1) // SEND welcome message
          perror("send");

        if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) { // RECEIVE username
          perror("recv");
          exit(1);
        }

        strcat(buf,".txt");
        buf[numbytes + 4] = '\0';
        printf("login de %s\n", buf);

        int flag = -1;
        for (i = 0; i < sizeof(file_array) / sizeof(FileInfo); i += 2){ //CHECKS Login
          if(compare(file_array[i].filename,buf) == 1)
            flag = i;
        }

        if(flag == -1) //Case wrong login
          exit(1);

        char *login;
        login = malloc(strlen(buf)*sizeof(char));
        strcpy(login,buf);

        char *f;
        char *f_aux;

        strcpy(frase,"(0) Sair");
        f = strcat(frase,"\n(1) listar todas as pessoas formadas em um determinado curso");
        f = strcat(frase,"\n(2) listar as habilidades dos perfis que moram em uma determinada cidade");
        f = strcat(f,"\n(3) acrescentar uma nova experiência em um perfil");
        f = strcat(f,"\n(4) dado o email do perfil, retornar sua experiência");
        f = strcat(f,"\n(5) listar todas as informações de todos os perfis");
        f = strcat(f,"\n(6) dado o email de um perfil, retornar suas informações");
        len = strlen(f);

        if (sendall(new_fd, f,&len) == -1) // SENDS menu
          perror("send");

		    int inteiro;
		    int received_int = 0;
		    int loop = 1;

		    while(loop == 1){
		        inteiro = read(new_fd, &received_int, sizeof(received_int)); //RECEIVES operation identifier

		        fprintf(stdout,"%d\n", ntohl(received_int));

	          inteiro = ntohl(received_int);

	          if(inteiro == 0){ //EXIT
			       printf("Encerrando processo...\n");
			       loop--;
	           close(new_fd);
	           exit(0);
	          }

		         else if(inteiro == 1){ // List all people of a specific academic formation
	             char ans[200];
	             char conc[100];
	             char curso[100];

	             if ((numbytes = recv(new_fd, curso, 100, 0)) == -1) { // RECEIVE curso
	              perror("recv");
	              exit(1);
	              }

	              gettimeofday(&tv1, NULL);

	              strcpy(ans,"Pessoas com a formação desejada:\n");

	              for (j = 0; j < sizeof(file_array) / sizeof(FileInfo); j += 2){
	               f_info = &file_array[j];
	               if(!confere_curso(f_info,curso)){
	                 print_email(f_info,conc);
	                 strcat(ans,conc);
	                }
	              }
	              len = strlen(ans);

	              printf("Retornando pessoas com a formação desejada...\n");

	              gettimeofday(&tv2, NULL);

	              if (sendall(new_fd,ans,&len) == -1) // SENDS menu option
	               perror("send");

	              memset(curso,0,strlen(curso));
	              memset(ans,0,strlen(ans));
	              //memset(r,0,strlen(r));
	              memset(conc,0,strlen(conc));

	           }
		         else if(inteiro == 2){ //List the profile habilities of people who lives in a specific city
	              char city[20];
	              char conc1[500];
	              char conc2[500];
	              char answer[10000];

	              if ((numbytes = recv(new_fd, city, 20, 0)) == -1) { // RECEIVE cidade
	               perror("recv");
	               exit(1);
	             }

	              gettimeofday(&tv1,NULL);

	              strcpy(answer,"As pessoas dessa cidade possuem as seguintes habilidades:\n");

	              for (j = 0; j < sizeof(file_array) / sizeof(FileInfo); j += 2){
	               f_info = &file_array[j];
	               if(!confere_cidade(f_info,city)){
	                print_email(f_info,conc1);
	                strcat(answer,conc1);
	                print_h(f_info,conc2);
	                strcat(answer,conc2);
	                strcat(answer,"\n");
	               }
	              }

	              printf("Retornando habilidades...\n");

	              gettimeofday(&tv2, NULL);

	              if (sendall(new_fd,answer,&len) == -1)
	               perror("send");

	              memset(city,0,strlen(city));
	              memset(conc1,0,strlen(conc1));
	              memset(conc2,0,strlen(conc2));
	              memset(answer,0,strlen(answer));
	            }

	            else if(inteiro == 3){ //ADDS new experiencesto a profile
	              char xp[1000];
	              if ((numbytes = recv(new_fd,xp,1000, 0)) == -1) { // RECEIVE experiência a ser adicionada
	               perror("recv");
	               exit(1);
	              }

	              f_info = &file_array[flag];
	              add_xp(f_info,xp);
	              printf("Escrita realizada no arquivo!\n");
	            }

	            else if(inteiro == 4){
	              int flag_aux;
	              char email[100];
	              char resp[100];

	              if ((numbytes = recv(new_fd, email, 100, 0)) == -1) { // RECEIVE username
	               perror("recv");
	               exit(1);
	              }

	              gettimeofday(&tv1,NULL);

	              strcat(email,".txt");
	              buf[numbytes + 4] = '\0';
	              for (i = 0; i < sizeof(file_array) / sizeof(FileInfo); i += 2){
	               if(compare(file_array[i].filename,email) == 1)
	                flag_aux = i;
	              }

	              experiences(&file_array[flag_aux],resp);
	              len = strlen(resp);

	              printf("Retornando experiencias...\n");

	              gettimeofday(&tv2,NULL);

	              if (sendall(new_fd,resp,&len) == -1) // SENDS menu option
	               perror("send");

	              memset(email,0,strlen(email));
	              memset(resp,0,strlen(resp));
	            }

	            else if(inteiro == 5){ //SENDS all the profiles located on server WITHOUT IMAGE
	              char st[5000];
	              char *arq;

	              gettimeofday(&tv1,NULL);

	              for (i = 0; i < sizeof(file_array) / sizeof(FileInfo); i += 2){
	                s_aux = print_all(&file_array[i]);
	                strcat(st,s_aux);
	                memset(s_aux,0,strlen(s_aux));
	               }

	              len = strlen(st);
	              printf("Retornando .txt dos perfis...\n");

	              gettimeofday(&tv2,NULL);

	              if (sendall(new_fd,st,&len) == -1) // SENDS profiles
	               perror("send");
	               /*for (i = 1; i < sizeof(file_array) / sizeof(FileInfo); i += 2){
	                  len = fileb_size(&file_array[i]);
	                  arq = malloc (len * sizeof(unsigned char));
	                  read_image(&file_array[i],arq,len);
	                  if (sendall(new_fd,arq,&len) == -1) // SENDS respective profile
	                   perror("send");
	                  memset(arq,0,len);
	                  free(arq);
	               }*/

	           	  memset(st,0,strlen(st));
	           }

	            else if(inteiro == 6){ // SENDS a specific profile WITHOUT IMAGE
	              char email[100];
	              int flag_aux;
	              char *arq;

	              if ((numbytes = recv(new_fd, email, 100, 0)) == -1) { // RECEIVE username
	               perror("recv");
	               exit(1);
	              }

	              gettimeofday(&tv1,NULL);

	              strcat(email,".txt");
	              buf[numbytes + 4] = '\0';

	              printf("Retornando perfil de %s\n", email);

	              for (i = 0; i < sizeof(file_array) / sizeof(FileInfo); i += 2){
	               if(compare(file_array[i].filename,email) == 1)
	                flag_aux = i;
	              }

	              s_aux = print_all(&file_array[flag_aux]);
	              len = strlen(s_aux);

	              gettimeofday(&tv2,NULL);

	              if (sendall(new_fd,s_aux,&len) == -1) // SENDS respective profile
	               perror("send");

	          /*    flag_aux++;
	              len = fileb_size(&file_array[flag_aux]);
	              int converted_number = htonl(len);
	              write(new_fd, &converted_number, sizeof(converted_number));
	              arq = malloc (len * sizeof(unsigned char));

	              read_image(&file_array[flag_aux],arq,len);

	              gettimeofday(&tv2,NULL);

	              if (sendall(new_fd,s_aux,&len) == -1) // SENDS respective profile
	               perror("send");
*/
	           	  memset(email,0,strlen(email));
	            }
		         else{
	             close(new_fd);
	             exit(0);
	           }

	          double tempo = (tv2.tv_sec - tv1.tv_sec) + ((tv2.tv_usec - tv1.tv_usec)/1000000.0);
	          printf("Tempo de atualização: %lfs\n", tempo);
		    }
	      close(new_fd);
	      exit(0);
	     }
	    close(new_fd);  // parent doesn't need this
	    }
	  return 0;
	}
