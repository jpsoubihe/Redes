#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FileInfo {
  char *filename;
  FILE *fp;
} FileInfo;

void read_image(FileInfo *f,unsigned char *s,int len){
  FILE *a;
  a = fopen(f->filename, "rb");
  fread(s,len,1,a);
  rewind(a);
  fclose(a);
}

int fileb_size(FileInfo *f){
  FILE *a;
  int len;

  a = fopen(f->filename, "rb");
  fseek(a,0,SEEK_END);
  len = ftell(a);
  fseek(a,0,SEEK_SET);
  fclose(a);
  return len;
}

int file_size(FileInfo *f){
  FILE *a;
  int len;

  a = fopen(f->filename, "r");
  fseek(a,0,SEEK_END);
  len = ftell(a);
  fseek(a,0,SEEK_SET);
  fclose(a);
  return len;
}

int compare(char *st1,char *st2){
  for(int i = 2;i < strlen(st1);i++){
    if(st1[i] != st2[i - 2])
      return 0;
  }
  return 1;
}

void experiences(FileInfo *f,char *q){
  char c;
  int contador = 5;
  FileInfo *a;
  int i;

  a = f;
  a->fp = fopen(a->filename, "r");
  if (a->fp == NULL) {
    perror("fopen");
    exit(1);
  }

  while(contador > 0){
    c = getc(a->fp);
    if(c == '\n')
      contador--;
  }
  for(i = 0;i < 13;i++){
    c = getc(a->fp);
  }

  i = 0;

  c = getc(a->fp);
  while(c != EOF){
    q[i] = c;
    i++;
    c = getc(a->fp);
  }
  rewind(a->fp);
  //return q;
}

int confere_curso(FileInfo *f,char *st){
  int contador = 3;
  int i;
  char r[32];
  char c;
  FILE *a;
  a = fopen(f->filename, "r");
  if (a == NULL){
    perror("fopen");
    exit(1);
  }
  while(contador > 0){
    c = getc(a);
    if(c == '\n')
      contador--;
  }

  fseek(a,20,SEEK_CUR);

  c = getc(a);
  i = 0;
  while(c != '\n'){
    r[i] = c;
    i++;
    c = getc(a);
  }
  r[i] = '\0';




  rewind(a);
  fclose(a);
  return strcmp(st,r);
} //recebe FileInfo (com o file descriptor fechado) e a string representando o curso a ser buscado

void print_email(FileInfo *f, char *s){
  FILE *a;
  a = fopen(f->filename, "r");
  if (a == NULL){
    perror("fopen");
    exit(1);
  }

  fseek(a,7,SEEK_SET);
  fgets(s,32,a);

  rewind(a);
  fclose(a);
}

int confere_cidade(FileInfo *f,char *s){
  int contador = 2;
  int i;
  char r[20];
  char c;
  FILE *a;

  a = fopen(f->filename, "r");
  if (a == NULL){
    perror("fopen");
    exit(1);
  }
  while(contador > 0){
    c = getc(a);
    if(c == '\n')
      contador--;
  }
  fseek(a,12,SEEK_CUR);
  //fgets(r,20,a);
  c = getc(a);
  i = 0;
  while(c != '\n'){
    r[i] = c;
    i++;
    c = getc(a);
  }
  r[i] = '\0';
  rewind(a);
  fclose(a);

  return strcmp(r,s);
}

void print_h(FileInfo *f,char *s){
  FILE *a;
  char aux[1000];
  char c;
  a = fopen(f->filename, "r");
  if (a == NULL){
    perror("fopen");
    exit(1);
  }

  fgets(aux,1000,a);
  fgets(aux,1000,a);
  fgets(aux,1000,a);
  fgets(aux,1000,a);
  fseek(a,13,SEEK_CUR);
  int i = 0;
  c = getc(a);
  while (c != '\n') {
    s[i] = c;
    c = getc(a);
    i++;
  }
  //fread(s,1000,1,a);
  s[i] = '\0';
  rewind(a);
  fclose(a);
}

int conta_xp(FileInfo *f){
    FILE *a;
    char c;
    int cont = 5;

    a = fopen(f->filename, "r");
    if (a == NULL){
      perror("fopen");
      exit(1);
    }

    c = getc(a);
    while(cont > 0){
      c = getc(a);
      if(c == '\n')
        cont--;
    }

    while (c != EOF) {
      c = getc(a);
      if(c == '\n')
        cont++;
    }

    rewind(a);
    fclose(a);

    return cont + 1;
  }

void add_xp(FileInfo *f,char *s){
    FILE *a;
    char c;
    char s_p[1000];
    char aux[1000];
    memset(s_p,0,1000);

    s_p[0] = '(';
    s_p[1] = conta_xp(f) + '0';
    s_p[2] = ')';
    s_p[3] = ' ';

    strcat(s_p,s);

    fflush(stdin);
    printf("s_p = %s\n", s_p);

    a = fopen(f->filename, "a+");
    
    if (a == NULL){
      perror("fopen");
      exit(1);
    }

    //fprintf(a,"%s",c);
    fprintf(a,"%s\n",s_p);
    //fprintf(a," %s\n",s);
    rewind(a);
    //free(s_p);
    fclose(a);
  }

char* print_all(FileInfo *f){
  FILE *a;
  char c;
  int i = 1;
  char *aux;
  int tam;


  tam = file_size(f);

  a = fopen(f->filename, "r");
  if (a == NULL){
    perror("fopen");
    exit(1);
  }

  aux = malloc(tam * sizeof(char));
  fread(aux,tam,1,a);

  return aux;
}

void getNome(FileInfo *f,char *s){
  FILE *a;
  char aux[1000];

  a = fopen(f->filename, "r");
  if (a == NULL){
    perror("fopen");
    exit(1);
  }

  fgets(aux,1000,a);
  fseek(a,6,SEEK_CUR);
  fscanf(a,"%s",aux);
  strcpy(s,aux);
  fscanf(a,"%s",aux);
  fscanf(a," %s",aux);
  strcat(s," ");
  strcat(s,aux);

}
