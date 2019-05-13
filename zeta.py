import sys
import os
from array import array
from enum import Enum
from abc import ABC, abstractmethod
import scanner#scanner.py


class Zeta:
    def __init__(self):
        self.has_error = False

    def error_message(self,line,message):
        print("ERROR: line",line,":",message)
        self.has_error = True
        return


Z = Zeta()



def main():

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
    # b = array('b')
    b = []

    for i in range(0,file_size_bytes):
        #print(f.read(1))
        c = f.read(1)
        b.append(int.from_bytes(c,"big"))



    f.close

    s = Scanner(b)
    token_list = s.scan_tokens()

    # for t in self.token_list:
    #     print(t.type)

    print_all_tokens(token_list)



    run(b)

    if(Z.has_error):
        exit()

    return

def print_all_tokens(token_list):
    for t in token_list:
        print(t.type,end=' ')
        if(t.type == 'number' or t.type=='string' or t.type=='identifier'):
            print(t.literal,end='')
        print('')




def run(byte_arr):
    pass
    return


class Expression(ABC):
    def __init__(self):
        pass

class Binary(Expression, left, operator, right):
    def __init__(self):
        self.left = left
        self.operator = operator
        self.right = right

class Unary(Expression,operator,right):
    def __init__(self):
        self.operator = operator
        self.right = tight

class Grouping(Expression,expression):
    def __init__(self):
        self.expression = expression

class Literal(Expression,value):
    def __init__(self):
        self.value = value

class Operator(Expression,value):
    def __init__(self):
        self.value = value

class tree_printer:
    def string_print(Expression expr):
        return expr.accept(self)



if __name__ == "__main__":
	main()
