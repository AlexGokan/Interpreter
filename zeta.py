import sys
import os
from array import array

class zeta:
    def __init__(self):
        self.has_error = False

    def error_message(self,line,message):
        print("ERROR: line",line,":",message)
        self.has_error = True
        return


Z = zeta()

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

    s = scanner(b)
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




class scanner:
    def __init__(self,byte_arr):
        self.byte_arr = byte_arr

    def print_byte_arr(self):
        for i in range(0,len(self.byte_arr)):
            print(self.byte_arr[i])

    def scan_tokens(self):
        pass
        #must return some list of token objects

class token:
    def __init__(self):
        pass



if __name__ == "__main__":
	main()
