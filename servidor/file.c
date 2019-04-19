#include <stdio.h>
#include <stdlib.h>

typedef struct FileInfo {
  char *filename;
  FILE *fp;
  char *file_contents;
} FileInfo;

int file_size(FILE *a){
  char *s;
  int len;

  fseek(a,0,SEEK_END);
  len = ftell(a);
   fclose(a);

   return len;
}

void do_stuff_with_image(FileInfo *f_info){
  int size, read_size, stat, packet_index;

  packet_index = 1;
  printf("%s\n", f_info->filename);
  f_info->fp = fopen(f_info->filename, "rb");
  if (f_info->fp == NULL) {
    perror("fopen");
}
  printf("Getting Picture Size\n");

  if(f_info->fp == NULL) {
      printf("Error Opening Image File");
    }

  fseek(f_info->fp, 0, SEEK_END);
  size = ftell(f_info->fp);
  rewind(f_info->fp);
  printf("Total Picture size: %i\n",size);

}

char* do_stuff_with_file(FileInfo *f_info){
  f_info->fp = fopen(f_info->filename, "r");
  if (f_info->fp == NULL) {
    perror("fopen");
  }
  //printf("abriu\n");
  char *p;
  int l;
  /*do{
    c = getc(f_info->fp);
    if(c != EOF)
      printf("%c", c);
  }
  while(c != EOF);*/
  l = file_size(f_info->fp);
  p = malloc(l * sizeof(char));
  fread(&p,sizeof(char),l,f_info->fp); //le um bloco de dados, no caso de tamanho 10000. Sem o rewind apaga o conteudo do arquivo
  return p;
}

void close_file(FileInfo *f_info){
  if (f_info->fp != NULL) fclose(f_info->fp);
}

int main(void){
  int i;
  char *st;
  FileInfo file_array[] = {
    {"./ana@gmail.com", NULL, NULL},
    {"./pics/ana@gmail.com.jpg", NULL, NULL},
    {"./jpsoubihe@gmail.com", NULL, NULL},
    {"./pics/jpsoubihe@gmail.com.jpg", NULL, NULL},
    {"./julia@gmail.com", NULL, NULL},
    {"./julia@gmail.com.jpg", NULL, NULL},
    {"./maria_silva@gmail.com", NULL, NULL},
    {"./pics/joana@gmail.com.jpg", NULL, NULL},
    {"./silva@gmail.com", NULL, NULL},
    {"./pics/silva@gmail.com.jpg", NULL, NULL},
    {"./ze@gmail.com", NULL, NULL},
    {"./pics/ze@gmail.com.jpg", NULL, NULL}
  };



  //FILE jpg nao e lida como txt
  for (i = 0; i < sizeof(file_array) / sizeof(FileInfo); i += 2) {
     printf("%s\n", file_array[i].filename);
   }

 for (i = 0; i < sizeof(file_array) / sizeof(FileInfo); i += 2) {
    st = do_stuff_with_file(&file_array[i]);
    printf("%s", st);
  }

  for (i = 0; i < sizeof(file_array) / sizeof(FileInfo); i += 2) {
    close_file(&file_array[i]);
  }

  /*for (i = 1; i < sizeof(file_array) / sizeof(FileInfo); i += 2) {
    do_stuff_with_image(&file_array[i]);
    printf("%d\n", i);
  }

  for (i = 1; i < sizeof(file_array) / sizeof(FileInfo); i += 2) {
    close_file(&file_array[i]);
  }*/


  return 0;
}
