
class Scanner:
    def __init__(self,byte_arr):
        self.start = 0
        self.current = 0
        self.line = 1

        self.byte_arr = byte_arr#called 'source' in book
        self.tokens = []

        self.keywords = [b'and',b'class',b'else',b'false',b'for',b'fun',b'if',b'nil',b'or',b'print',b'return',b'super',b'this',b'true',b'var',b'while']


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
            self.add_token(')',None)
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
        elif(c==47):#comment
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
        elif(self.is_digit(c)):
            self.scan_number()
        elif(self.is_alphabetic(c)):
            self.scan_indentifier()
        else:
            Z.error_message(self.line,"unexpected character")

    def scan_indentifier(self):
        while(self.is_alphanumeric(self.peek())):
            self.advance()

        text = self.byte_arr[self.start:self.current:1]
        text = bytes(text)
        print(text)
        if(text in self.keywords):
            print("keyword detected")
            type = text
        else:
            type = 'identifier'

        self.add_token(type,None)

    def is_alphabetic(self,c):
        if((c >= 97 and c <= 122) or (c >= 65 and c<=90) or (c == 95)):
            return True
        else:
            return False

    def is_alphanumeric(self,c):
        return (self.is_alphabetic(c) or self.is_digit(c))

    def scan_number(self):
        while(self.is_digit(self.peek())):
            self.advance()

        if(self.peek() == 46 and self.is_digit(self.peek_next())):
            self.advance()

            while(self.is_digit(self.peek())):
                self.advance()

        value = self.byte_arr[self.start:self.current:1]
        value = self.array_to_double(value)
        self.add_token('number',value)


    def array_to_double(self,v):
        num_digits = len(v)
        # print(v)
        for i in range(0,num_digits):
            if(v[i]==46):
                break
        upper = v[0:i:1]
        lower = v[i+1:num_digits:1]
        # print(upper)
        # print(lower)

        output = 0
        power = 0
        for d in reversed(upper):
            d = d-48
            output += (d*(10**power))
            power += 1

        power = -1
        for d in lower:
            d = d-48
            output += (d*(10**power))
            power -= 1

        # print(output)
        return output


    def is_digit(self,c):
        return ((c >= 48) and (c <= 57))

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

    def peek_next(self):
        if(self.current+1 >= len(self.byte_arr)):
            return 0
        return self.byte_arr[self.current+1]

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
        value_bytes = bytes(value)
        print(value_bytes)
        # for i in range(0,len(value)):
        #     integer_val = value[i]
        #     byte_val = byte(integer_val)
        #     value_bytes[i] = byte_val

        # b''.join(value_bytes).decode('utf-8')

        self.add_token('string',value_bytes)
    # def add_token(type):
    #     add_token(type,None)

    def add_token(self,type,literal):
        # print("adding token")
        text = (self.byte_arr)[self.start:self.current:1]
        (self.tokens).append(Token(type,text,literal,self.line))




class Token:
    def __init__(self,type,lexeme,literal,line):
        self.type = type
        self.lexeme = lexeme
        self.literal = literal
        self.line = line
