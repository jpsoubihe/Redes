else if(op == 5){
  FILE *arq;
  char str[40];
  strcpy(str,"LogDePerfis");

  gettimeofday(&tv1,NULL);

  write(sockfd, &converted_number, sizeof(converted_number));



  //strcat(str,buf);

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


else if(inteiro == 5){
  char st[5000];
  char *arq;

  gettimeofday(&tv1,NULL);

  for (i = 0; i < sizeof(file_array) / sizeof(FileInfo); i += 2){
     s_aux = print_all(&file_array[i]);
     strcat(st,s_aux);
  }

  len = strlen(st);
  printf("Retornando .txt dos perfis...\n");
  if (sendall(new_fd,st,&len) == -1) // SENDS menu option
   perror("send");
 printf("Retornando .txt dos perfis...\n");
 for (i = 1; i < sizeof(file_array) / sizeof(FileInfo); i += 2){
    len = file_size(&file_array[i]);
    arq = malloc (len * sizeof(unsigned char));
    read_image(&file_array[i],arq,len);
    if (sendall(new_fd,arq,&len) == -1) // SENDS respective profile
     perror("send");
    free(arq);
 }

  gettimeofday(&tv2,NULL);
  printf("%0.9f\n", tv2.tv_sec - tv1.tv_sec);

  //rewind(file_array[flag].fp);
}
