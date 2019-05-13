import sys
import os
from array import array
from enum import Enum



class Zeta:
    def __init__(self):
        self.has_error = False
        #self.token_type = Enum("LPAREN","RPAREN","LBRACE","RBRACE","COMMA","DOT","MINUS","PLUS","SEMICOLON","SLASH","STAR","EXLAMATION","EXLAMATION_EQUAL","EQUAL","EQUAL_EQUAL","GREATER","GREATER_EQUAL","LESS","LESS_EQUAL","IDENTIFIER","STRING","NUMBER","AND","CLASS","ELSE","FALSE","FUN","FOR","IF","NIL","OR","PRINT","RETURN","SUPER","THIS","TRUE","VAR","WHILE")

    def error_message(self,line,message):
        print("ERROR: line",line,":",message)
        self.has_error = True
        return


Z = Zeta()

def main():




    Z.error_message(4,"test error message")

    if(len(sys.argv) > 2):
        print("Usage: zeta [script]")
        exit()
    elif(len(sys.argv) == 2):
        filename = sys.argv[1]
        if(filename[-2:] != ".z"):
            print("please provide a .z file")
            exit()
        run_file(filename)
    else:
        print("please provide ONE input file")
        exit()

    return


def run_file(filename):
    file_size_bytes = os.path.getsize(filename)
    f = open(filename,"rb")
    b = array('b')

    for i in range(0,file_size_bytes):
        #print(f.read(1))
        c = f.read(1)
        b.append(int.from_bytes(c,"big"))

    f.close

    s = Scanner(b)
    token_list = s.scan_tokens()

    # for t in token_list:
    #     print(t)
    #uncommment later

    s.print_byte_arr()


    run(b)

    if(Z.has_error):
        exit()

    return



def run(byte_arr):
    pass
    return




class Scanner:
    def __init__(self,byte_arr):
        self.start = 0
        self.current = 0
        self.line = 1

        self.byte_arr = byte_arr#called 'source' in book
        self.tokens = []


    def print_byte_arr(self):
        for i in range(0,len(self.byte_arr)):
            print(self.byte_arr[i])

    def is_at_end(self):
        return (self.current >= len(self.byte_arr))

    def scan_tokens(self):
        print("scanning tokens")
        while(self.is_at_end() == False):
            self.start = self.current
            self.scan_token()


        (self.tokens).append(Token("EOF","",None,None))#I think?
        #in book it is (EOF,"","NULL","line")
        return(self.tokens)
        #must return some list of token objects

    def scan_token(self):
        c = self.advance()
        if(c==40):
            self.add_token('(',None)
        elif(c==41):
            self.add_token('(',None)
        elif(c==123):
            self.add_token('{',None)
        elif(c==125):
            self.add_token('}',None)
        elif(c==44):
            self.add_token(',',None)
        elif(c==46):
            self.add_token('.',None)
        elif(c==45):
            self.add_token('-',None)
        elif(c==43):
            self.add_token('+',None)
        elif(c==59):
            self.add_token(';',None)
        elif(c==42):
            self.add_token('*',None)
        elif(c==33):
            if(self.match(61)):
                self.add_token('!=',None)
            else:
                self.add_token('!',None)
        elif(c==61):
            if(self.match(61)):
                self.add_token('==',None)
            else:
                self.add_token('=',None)
        elif(c==60):
            if(self.match(61)):
                self.add_token('<=',None)
            else:
                self.add_token('<',None)
        elif(c==62):
            if(self.match(61)):
                self.add_token('>=',None)
            else:
                self.add_token('>',None)
        elif(c==47):
            if(self.match(47)):
                while(self.peek() != '\n' and (self.is_at_end() == False)):
                    self.advance()
            else:
                self.add_token('/',None)
        elif(c==32):#space
            pass
        elif(c==9):#tab
            pass
        elif(c==13):#carriage return
            pass
        elif(c==10):#newline
            self.line += 1
        elif(c==34):#double quotes
            self.scan_string()
        else:
            Z.error_message(self.line,"unexpected character")

    def advance(self):
        self.current = self.current+1
        return (self.byte_arr)[self.current-1]

    def match(self,expected):
        if(self.is_at_end()):
            return False
        if((self.byte_arr)[self.current] != expected):
            return False

        self.current += 1
        return True

    def peek(self):
        if(self.is_at_end()):
            return '\0'
        return (self.byte_arr)[self.current]

    def scan_string(self):
        while(self.peek() != 34 and (self.is_at_end() == False)):
            if(self.peek() == 10):
                self.line += 1
            self.advance()

        if(self.is_at_end()):
            Z.error_message(self.line,"unterminated string")
            return

        self.advance()

        value = self.byte_arr[self.start+1:self.current-1:1]
        self.add_token('string',value)
    # def add_token(type):
    #     add_token(type,None)

    def add_token(self,type,literal):
        print("adding token")
        text = (self.byte_arr)[self.start:self.current:1]
        (self.tokens).append(Token(type,text,literal,self.line))




class Token:
    def __init__(self,type,lexeme,literal,line):
        self.type = type
        self.lexeme = lexeme
        self.literal = literal
        self.line = line



if __name__ == "__main__":
	main()
