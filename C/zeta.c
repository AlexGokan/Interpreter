
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "defines.h"
#include "scanner.c"

typedef struct ast_var{
  double value;
  char* name;
  struct ast_var* next;
  struct ast_var* prev;
}ast_var;

typedef struct var_table{
  struct ast_var* top;
}var_table;

struct ast_var* new_ast_var(char* name){
  struct ast_var* n = malloc(1*sizeof(ast_var));
  n->name = name;
  n->value = 0.0;
  n->next = NULL;
  n->prev = NULL;
}

void free_var_table(struct var_table* VT){
  struct ast_var* v = VT->top;
  while(v != NULL){
    struct ast_var* tmp = v->next;
    free(v);
    v = tmp;
  }
  free(VT);
}

void print_all_variables(struct var_table* VT){
  print_seperator();
  struct ast_var* v = VT->top;
  while(v != NULL){
    printf("%s : %f\n",v->name,v->value);
    v = v->next;
  }
  print_seperator();
}

struct ast_var* search_for_variable_in_table(struct var_table* VT, char* name){
  struct ast_var* T = VT->top;
  while(T != NULL){
    if(strcmp(T->name,name) == 0){
      return T;
    }
    T = T->next;
  }

  if(T == NULL){
    printf("could not find given variable in table\n");
    return NULL;
  }
}

void assign(struct var_table* VT, struct Token* T, char* name, double value){
  struct ast_var* v = search_for_variable_in_table(VT,name);

  if(v != NULL){
    v->value = value;
  }

  if(v == NULL){
    error_message(T->line,"variable was not declared");
  }
}

double evaluate(struct Token* head){
  struct Token* T = head;
  double result = 0;
  while(head->type != SEMICOLON){
    if(head->type == NUMBER){
      result = head->num_literal;
    }
    else{
      printf("dont know how to handle this yet, will come back to it");
      break;
    }
    head = head->next;
  }
  return result;
}

void parse_for_assignments(struct Scanner* S, struct var_table* VT){
  struct Token* T = S->tokens;

  while(T->type != EOFTOKEN){
    if(T->type == VARIABLE){
      struct ast_var* var_to_search_for = search_for_variable_in_table(VT,T->string_literal);
      if(var_to_search_for == NULL){
        error_message(T->line,"attempting to assign an undeclared variable");
      }
      else{
        if(T->next != NULL & T->next->type == EQUAL){
          double value_to_assign = evaluate(T->next->next);
          assign(VT,T,T->string_literal,value_to_assign);
        }
        // else{
        //   error_message(T->line, "variable name must be followed by an assignment operator");
        // }
        //idk why this error is popping up, even though it is assigning fine
      }
    }

    T = T->next;
  }
}

struct var_table* parse_for_declarations(struct Scanner* S){
  struct Token* T = S->tokens;

  struct var_table* VT = malloc(sizeof(var_table));
  VT->top = NULL;

  while(T->type != EOFTOKEN){
    if(T->type == KEYWORD_VAR){
      if(T->next != NULL & T->next->type == VARIABLE){
          if(T->next->next != NULL & T->next->next->type == SEMICOLON){
            printf("declared a variable %s\n",T->next->string_literal);
            struct ast_var* v = new_ast_var(T->next->string_literal);
            struct ast_var* VT_head = VT->top;

            v->next = VT_head;
            if(VT_head != NULL){
              VT_head->prev = v;
            }
            VT->top = v;
          }
          else{
            error_message(T->line,"missing semicolon");
          }
      }
      else{
        error_message(T->line,"keyword \'var\' must be followed by a variable name");
      }
    }

    T = T->next;
  }

  return VT;

}


void parse_tokens(struct Scanner* S){
  struct Token* T = S->tokens;
  struct Token* curr = T;

  struct var_table* VT = parse_for_declarations(S);

  print_all_variables(VT);

  parse_for_assignments(S,VT);

  print_all_variables(VT);

  free_var_table(VT);


}


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


  parse_tokens(S);


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
