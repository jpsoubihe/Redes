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
#include "cadastro.h"

#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold
#define MAXDATASIZE 10000 // max number of bytes we can get at once

int sendall(int s, char *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n;
    while(total < *len) {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }
    *len = total; // return number actually sent here
    return n==-1?-1:0; // return -1 on failure, 0 on success
}


void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    int conta_c = 0;
    struct arch a;
    int numbytes;
    char buf[MAXDATASIZE];
    char n[30] = "joao";
    char se[12] = "1234";
    char frase[1000] = "Login:";
    char frase1[] = "Bem-vindo!";
    int len;

    strcpy(a.nome,n);
    strcpy(a.senha,se);

    printf("nome da struct = %s\nsenha = %s\n", a.nome,a.senha);

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
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
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
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener

            if (send(new_fd, frase1, 13, 0) == -1) // SEND welcome
                perror("send");

            while(strcmp(buf,a.nome) != 0){ // CHECKS username
              if (send(new_fd, frase, 13, 0) == -1) // ASKS for login
                perror("send");

              if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) { // RECEIVE username
                perror("recv");
                exit(1);
              }
            }

            buf[numbytes] = '\0';
            printf("%s\n",buf);
            strcpy(frase,"Senha: ");

            while (strcmp(buf,a.senha) != 0) { //CHECKS password
              if (send(new_fd, frase, 13, 0) == -1) // RE-ASKS for password
                perror("send");
              if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) { // RECEIVE password
                perror("recv");
                exit(1);
              }

              buf[numbytes] = '\0';
              printf("%s\n",buf);
            }



            strcpy(frase,"Senha correta\n");
            if (send(new_fd, frase, 13, 0) == -1) // CONFIRM password
              perror("send");


            char *f;
            char *f_aux;

            strcpy(frase,"(1) listar todas as pessoas formadas em um determinado curso");
            f = strcat(frase,"\n(2) listar as habilidades dos perfis que moram em uma determinada cidade");
            f = strcat(f,"\n(3) acrescentar uma nova experiência em um perfil");
            f = strcat(f,"\n(4) dado o email do perfil, retornar sua experiência");
            f = strcat(f,"\n(5) listar todas as informações de todos os perfis");
            f = strcat(f,"\n(6) dado o email de um perfil, retornar suas informações");

            len = strlen(f);
            if (sendall(new_fd, f,&len) == -1) // SENDS menu option
              perror("send");

            if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) { // RECEIVE menu option
              perror("recv");
              exit(1);
              }
            buf[numbytes] = '\0';


            if(strcmp(buf,"1") == 0){
              printf("UM\n");
            }

            else if(strcmp(buf,"2") == 0){
              printf("DOIS\n");
            }

            else if(strcmp(buf,"3") == 0){
              printf("TRES\n");
            }

            else if(strcmp(buf,"4") == 0){
              printf("QUATRO\n");
            }

            else if(strcmp(buf,"5") == 0){
              printf("CINCO\n");
            }

            else if(strcmp(buf,"6") == 0){
              printf("SEIS\n");
            }

            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }
    return 0;
}
