#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(char *st1,char *st2){
  for(int i = 2;i < strlen(st1);i++){
    if(st1[i] != st2[i - 2])
      return 0;
  }
  return 1;
}

int main(){
  char s1[20];
  char s2[20];
  int answer;

  scanf("%s", s1);
  scanf("%s", s2);

  printf("%s\n", s1);
  printf("%s\n", s2);

  answer = compare(s1,s2);
  printf("answer = %d\n", answer);
  return 0;
}
