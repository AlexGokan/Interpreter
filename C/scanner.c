

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "defines.h"

typedef struct Token{
  int type;
  char* lexeme;
  int line;

  char* string_literal;
  int string_length;
  double num_literal;
  struct Token* next;//to make a linked list of tokens
  struct Token* prev;
  //oh god what have I done
}Token;

typedef struct Scanner{
  int start;
  int current;
  int line;

  char* source;
  struct Token* tokens;
  struct Token* tail;
  int num_chars_in_input;
}Scanner;

void print_all_tokens(struct Scanner* S){
  printf("--------------------------------------------------------------------\n");
  struct Token* t = S->tokens;
  while(t != NULL){
    printf("Token of type: %d     ",t->type);
    if(t->type == NUMBER){
      printf("%f",t->num_literal);
    }
    if(t->type == STRING){
      for(int i=0; i<t->string_length; i++){
        printf("%c",(t->string_literal)[i]);
      }
    }
    if(t->type == VARIABLE){
      printf("(var) ");
    }
    if((t->type <=116) & (t->type >=100)){
      for(int i=0; i<t->string_length; i++){
        printf("%c",(t->string_literal)[i]);
      }
    }

    printf("\n");
    t = t->next;
  }
    printf("--------------------------------------------------------------------\n");
}

bool is_alphabetic(char c){
  if((c>=97 & c<=122) | (c >=65 & c<=90) | (c==95)){
    return true;
  }
  return false;
}

bool is_digit(char c){
  if(c>=48 & c<=57){
    return true;
  }
  return false;
}

bool is_alphanumeric(char c){
  return (is_digit(c) | is_alphabetic(c));
}

struct Token* new_token(int type, char* lexeme, int line, char* string_literal, int string_length, double num_literal){
  struct Token* T = malloc(1*sizeof(Token));
  T->type = type;
  T->lexeme = lexeme;
  T->line = line;
  T->string_literal = string_literal;
  T->string_length = string_length;
  T->num_literal = num_literal;
  T->next = NULL;
  T->prev = NULL;
  return T;
}

struct Scanner* new_scanner(char* source){
  struct Scanner* S = calloc(1,sizeof(Scanner));
  S->source = source;
  S->start = 0;
  S->current = 0;
  S->line = 1;
  return S;
}

void error_message(int line, char* message){
  printf("ERROR : line %d : %s\n",line,message);
}


bool is_at_end(struct Scanner* S){
  return(S->current >= S->num_chars_in_input);
}

void free_tokens(struct Scanner* S){
  struct Token* T = S->tokens;
  while(T != NULL){
    struct Token* tmp = T->next;
    free(T->lexeme);
    free(T->string_literal);
    free(T);
    T = tmp;
  }
}

void tokens_append(struct Scanner* S,struct Token* t){
  if(S->tail != NULL){
    S->tail->next = t;
    t->prev = S->tail;
    S->tail = t;
  }
  else{
    S->tokens = t;
    S->tail = t;
  }

}

void add_token(struct Scanner* S, int type, char* string_literal, int string_length, double num_literal){
  int len = (S->current) - (S->start);
  // printf("len: %d\n",len);
  if(len < 0){
    printf("invalid length\n");
    return;
  }
  char* text = malloc(len*sizeof(char));
  int idx = 0;
  for(int i=S->start; i<S->current; i++){
      text[idx] = (S->source)[i];
      idx++;
  }
  struct Token* nt = new_token(type,text,S->line,string_literal,string_length,num_literal);
  tokens_append(S,nt);
}



char advance(struct Scanner* S){
  (S->current)++;
  return (S->source)[(S->current)-1];
}

bool match(struct Scanner* S, char expected){
    if(is_at_end(S)){
      return false;
    }
    if((S->source)[S->current] != expected){
      return false;
    }
    (S->current)++;
    return true;
}

char peek(struct Scanner* S){
  if(is_at_end(S)){
    return '\0';
  }
  return (S->source)[S->current];
}

char peek_next(struct Scanner* S){
  if((S->current)+1 >= S->num_chars_in_input){
    return 0;
  }
  return (S->source)[S->current+1];
}

void scan_number(struct Scanner* S){

  while(is_digit(peek(S))){
    advance(S);
  }

  if((peek(S) == 46) & (is_digit(peek_next(S)))){
    advance(S);
    while(is_digit(peek(S))){
      advance(S);
    }
  }
  //from start to current contains the chars for the value
  int num_digits = (S->current) - (S->start);
  // printf("%d digits\n",num_digits);
  int i=S->start;
  for(i=S->start; i<S->current; i++){
    if((S->source)[i] == 46){
      //hit a decimal point
      break;
    }
  }

  //upper from start to i
  //lower from i+1 to current
  double output = 0;
  int power = 0;
  int d;
  for(d=i-1; d>(S->start)-1; d--){
    int digit = ((S->source)[d])-48;
    // printf("encountered %d\n",digit);
    output += (digit * pow(10,power));
    power++;
  }
  // printf("upper\n");

// printf("d: %d     start %d       current %d     i %d\n",d,S->start,S->current,i);



  power = -1;
  for(int d=i+1; d<(S->current); d++){
    int digit = ((S->source)[d])-48;
    output += (digit * pow(10,power));
    power--;
  }

  // printf("lower\n");
  // printf("%lf\n",output);

  add_token(S,NUMBER,NULL,0,output);

}

void scan_string(struct Scanner* S){
  while(peek(S) != 34 & (is_at_end(S) == false)){
    if(peek(S) == 10){
      (S->line)++;
    }
    advance(S);
  }

  if(is_at_end(S)){
    error_message(S->line,"unterminated string");
    return;
  }

  advance(S);

  int len = ((S->current)-1) - ((S->start)+1);
  char* text = malloc(len*sizeof(char));
  int idx = 0;
  for(int i=(S->start)+1; i<(S->current)-1; i++){
    text[idx] = (S->source)[i];
    idx++;
  }

  // for(int i=0; i<len; i++){
  //   printf("%c",text[i]);
  // }
  // printf("\n");

  add_token(S,STRING,text,len,0);

}

void scan_identifier(struct Scanner* S){
  while(is_alphanumeric(peek(S))){
    advance(S);
  }
  int len = (S->current) - (S->start);
  char* text = malloc((len+1)*sizeof(char));
  int idx = 0;
  for(int i=S->start; i<S->current; i++){
      text[idx] = (S->source)[i];
      idx++;
  }
  text[len] = '\0';//to make it a string!

  printf("text 0:  %d\n",text[0]);

  int type;
  if((text != NULL) & (text[0] != '\0')){
    if(strcmp(text,"and") == 0){
      type = KEYWORD_AND;
    }
    else if(strcmp(text,"class") == 0){
      type = KEYWORD_CLASS;
    }
    else if(strcmp(text,"else") == 0){
      type = KEYWORD_ELSE;
    }
    else if(strcmp(text,"false") == 0){
      type = KEYWORD_FALSE;
    }
    else if(strcmp(text,"for") == 0){
      type = KEYWORD_FOR;
    }
    else if(strcmp(text,"fun") == 0){
      type = KEYWORD_FUN;
    }
    else if(strcmp(text,"if") == 0){
      type = KEYWORD_IF;
    }
    else if(strcmp(text,"nil") == 0){
      type = KEYWORD_NIL;
    }
    else if(strcmp(text,"or") == 0){
      type = KEYWORD_OR;
    }
    else if(strcmp(text,"print") == 0){
      type = KEYWORD_PRINT;
    }
    else if(strcmp(text,"return") == 0){
      type = KEYWORD_RETURN;
    }
    else if(strcmp(text,"super") == 0){
      type = KEYWORD_SUPER;
    }
    else if(strcmp(text,"this") == 0){
      type = KEYWORD_THIS;
    }
    else if(strcmp(text,"true") == 0){
      type = KEYWORD_TRUE;
    }
    else if(strcmp(text,"var") == 0){
      type = KEYWORD_VAR;
    }
    else if(strcmp(text,"while") == 0){
      type = KEYWORD_WHILE;
    }
    else{
      type = VARIABLE;
    }


    add_token(S,type,text,len,0);

  }

}

void scan_token(struct Scanner* S){
  char c = advance(S);
  switch(c){
    case 40:
      add_token(S,LPAREN,NULL,0,0);
      break;
    case 41:
      add_token(S,RPAREN,NULL,0,0);
      break;
    case 125:
      add_token(S,RCURLY,NULL,0,0);
      break;
    case 123:
      add_token(S,LCURLY,NULL,0,0);
      break;
    case 91:
      add_token(S,LBRACKET,NULL,0,0);
      break;
    case 93:
      add_token(S,RBRACKET,NULL,0,0);
    case 44:
      add_token(S,COMMA,NULL,0,0);
      break;
    case 46:
      add_token(S,DOT,NULL,0,0);
      break;
    case 45:
      add_token(S,MINUS,NULL,0,0);
      break;
    case 43:
      add_token(S,PLUS,NULL,0,0);
      break;
    case 59:
      add_token(S,SEMICOLON,NULL,0,0);
      break;
    case 42:
      add_token(S,STAR,NULL,0,0);
      break;
    case 33:
      if(match(S,61)){
          add_token(S,NOTEQUAL,NULL,0,0);
      }
      else{
        add_token(S,EXLAMATION,NULL,0,0);
      }
      break;
    case 61:
      if(match(S,61)){
        add_token(S,EQUALEQUAL,NULL,0,0);
      }
      else{
        add_token(S,EQUAL,NULL,0,0);
      }
      break;
    case 60:
      if(match(S,61)){
        add_token(S,LESSEQUAL,NULL,0,0);
      }
      else{
        add_token(S,LESSTHAN,NULL,0,0);
      }
      break;
    case 62:
      if(match(S,61)){
          add_token(S,GREATEREQUAL,NULL,0,0);
      }
      else{
        add_token(S,GREATERTHAN,NULL,0,0);
      }
      break;
    case 32:
      break;
    case 9:
      break;
    case 13:
      break;
    case 10:
      (S->line)++;
      break;
    default:
      if(c==34){
        scan_string(S);
      }
      else if(is_digit(c)){
        scan_number(S);
      }
      else if(is_alphabetic(c)){
        scan_identifier(S);
      }
      else{
        error_message(S->line,"unexpected character");
      }
      break;


  }


}

struct Token* scan_tokens(struct Scanner* S){
  printf("scanning tokens\n");
  while(!is_at_end(S)){
    S->start = S->current;
    scan_token(S);
  }
  tokens_append(S,new_token(EOFTOKEN,0,0,NULL,0,0));
}
