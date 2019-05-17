
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "defines.h"
#include "scanner.c"

void run_file(char* filename){
  FILE* fp = fopen(filename,"r");

  fseek(fp,0,SEEK_END);
  int file_size_bytes = ftell(fp);
  rewind(fp);

  char* source = malloc(file_size_bytes * sizeof(char));

  for(int i=0; i<file_size_bytes; i++){
    char c = fgetc(fp);
    source[i] = c;
  }

  struct Scanner* S = new_scanner(source);
  S->num_chars_in_input = file_size_bytes;




  struct Token* token_list = scan_tokens(S);
  // token_list = S->tokens;

  print_all_tokens(S);

  free_tokens(S);
  free(source);
  free(S);
  fclose(fp);
}

void main(int argc, char* argv[]){
  if(argc != 2){
    printf("enter ONE input file");
    return;
  }

  char* filename = argv[1];

  run_file(filename);

  return;

}
