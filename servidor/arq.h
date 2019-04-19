#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FileInfo {
  char *filename;
  FILE *fp;
  char *file_contents;
} FileInfo;


int fileb_size(FileInfo *f);//GIVES the size of a binary file

int file_size(FileInfo *f); //GIVES the size of the file

void read_image(FileInfo *f,unsigned char *s,int len);

char* email_all(FILE *a); // ANALYZES all emails (for login)

char* list_all(FILE *a); //LOADS all information in a string

int compare(char *st1,char *st2); //COMPARES the relative path and the email for Login

void experiences(FileInfo *f,char *q);//LOOKS for the experiences of a certain profile

int confere_curso(FileInfo *f,char *st);//CHECKS if the course of a profile matches the course we are looking for

void print_email(FileInfo *f, char *s); //SAVES the profile email in a string

int confere_cidade(FileInfo *f,char *s); //CHECKS if the city of a profile matches the city we are looking for

void print_h(FileInfo *f,char *s);//SAVES the habilities of a profile in a string

void add_xp(FileInfo *f,char *s);// ADDS an experience to a profile

char* print_all(FileInfo *f);//SAVES all the profile in a string

int conta_xp(FileInfo *f); //COUNTS the number of experiences the profile already has
