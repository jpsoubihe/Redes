#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FileInfo {
char *filename;
FILE *fp;
char *file_contents;
} FileInfo;

// // int confere_cidade(FileInfo *f,char *s){
// //   int contador = 3;
// //   int i;
// //   char r[20];
// //   char c;
// //   FILE *a;

// //   a = fopen(f->filename, "r");
// //   if (a == NULL){
// //     perror("fopen");
// //     exit(1);
// //   }
// //   while(contador > 0){
// //     c = getc(a);
// //     if(c == '\n')
// //       contador--;
// //   }

// //   fseek(a,12,SEEK_CUR);
// //   //fgets(r,20,a);
// //   c = getc(a);
// //   i = 0;
// //   while(c != '\n'){
// //     r[i] = c;
// //     i++;
// //     c = getc(a);
// //   }
// //   r[i] = '\0';

// //   rewind(a);
// //   fclose(a);

// //   return strcmp(r,s);
// // }

// // void print_email(FileInfo *f, char *s){
// //   FILE *a;
// //   a = fopen(f->filename, "r");
// //   if (a == NULL){
// //     perror("fopen");
// //     exit(1);
// //   }

// //   fseek(a,7,SEEK_SET);
// //   fgets(s,32,a);
// //   rewind(a);
// //   fclose(a);
// // } // imprime o email de um dado perfil

// int confere_curso(FileInfo *f,char *st){
//   int contador = 4;
//   int i;
//   char r[32];
//   char c;
//   FILE *a;
//   printf("entrou\n");
//   a = fopen(f->filename, "r");
//   if (a == NULL){
//     perror("fopen");
//     exit(1);
//   }
//   while(contador > 0){
//     c = getc(a);
//     if(c == '\n')
//       contador--;
//   }

//   fseek(a,20,SEEK_CUR);

//   c = getc(a);
//   i = 0;
//   while(c != '\n'){
//     r[i] = c;
//     i++;
//     c = getc(a);
//   }
//   r[i] = '\0';
//   rewind(a);
//   fclose(a);
//   return strcmp(st,r);
// } //recebe FileInfo (com o file descriptor fechado) e a string representando o curso a ser buscado

// void print_h(FileInfo *f,char *s){
//   FILE *a;
//   char aux[1000];
//   a = fopen(f->filename, "r");
//   if (a == NULL){
//     perror("fopen");
//     exit(1);
//   }

//   fgets(aux,1000,a);
//   fgets(aux,1000,a);
//   fgets(aux,1000,a);
//   fgets(aux,1000,a);
//   fgets(aux,1000,a);
//   fseek(a,13,SEEK_CUR);
//   fgets(s,500,a);
// }

// int conta_xp(FileInfo *f){
//   FILE *a;
//   char c;
//   int cont = 6;

//   a = fopen(f->filename, "r");
//   if (a == NULL){
//     perror("fopen");
//     exit(1);
//   }

//   c = getc(a);
//   while(cont > 0){
//     c = getc(a);
//     if(c == '\n')
//       cont--;
//   }

//   while (c != EOF) {
//     c = getc(a);
//     if(c == '\n')
//       cont++;
//   }
//   return cont;
// }

// int compare(char *st1,char *st2){
//   for(int i = 2;i < strlen(st1);i++){
//     if(st1[i] != st2[i - 2])
//       return 0;
//   }
//   return 1;
// }

// void add_xp(FileInfo *f,char *s){
//   FILE *a;
//   char c;
//   char aux[1000];
//   a = fopen(f->filename, "a+");
//   if (a == NULL){
//     perror("fopen");
//     exit(1);
//   }

//   //fseek(a,-2,SEEK_END);
//   fprintf(a,"%s\n",s);
//   rewind(a);
//   fclose(a);
// }

// void read_image(FileInfo *f,unsigned char *s,int len){
//   FILE *a;

//   a = fopen(f->filename, "rb");
//   fread(s,len,1,a);
//   rewind(a);
//   fclose(a);
// }

// int file_size(FileInfo *f){

//   FILE *a;
//   int len;

//   a = fopen(f->filename, "rb");
//   fseek(a,0,SEEK_END);
//   len = ftell(a);
//   fseek(a,0,SEEK_SET);
//   //fread(s,len,1,a);
//   fclose(a);

//    return len;
// }

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
  printf("q = %s\n", q);
  rewind(a->fp);
  //return q;
}

int main(void){
  char xp[100];
  char st[] = "julia@gmail.com.jpg";
  unsigned char aux[100000];
  char *c;
  int i,tam;
  FileInfo *f;
  FileInfo file_array[] = {
    {"./ana@gmail.com.txt", NULL, NULL},
    {"./pics/ana@gmail.com.jpg", NULL, NULL},
    {"./jpsoubihe@gmail.com.txt", NULL, NULL},
    {"./pics/jpsoubihe@gmail.com.jpg", NULL, NULL},
    {"./julia@gmail.com.txt", NULL, NULL},
    {"./julia@gmail.com.jpg", NULL, NULL},
    {"./maria_silva@gmail.com.txt", NULL, NULL},
    {"./pics/joana@gmail.com.jpg", NULL, NULL},
    {"./silva@gmail.com.txt", NULL, NULL},
    {"./pics/silva@gmail.com.jpg", NULL, NULL},
    {"./ze@gmail.com.txt", NULL, NULL},
    {"./pics/ze@gmail.com.jpg", NULL, NULL}
  };

  //scanf ("%[^\n]%*c", xp);

  /*fgets(xp,100,stdin);
  fflush(stdin);
  tam = strlen(xp);
  tam--;
  aux = malloc(tam * sizeof(char));
  for(int j = 0;j < tam;j++)
    aux[j] = xp[j];*/

  for (i = 0; i < sizeof(file_array) / sizeof(FileInfo); i += 2){
    f = &file_array[i];

    //printf("ACHOU i = %d\n", i);
   // tam = file_size(f);
   // c = malloc(tam * sizeof(unsigned char));
   experiences(f,xp);
    printf("xp = %s\n", xp);

  }
  return 0;
}
