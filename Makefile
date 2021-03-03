exec = hello.out 
sources = $(wildcard Hazel/*.c Hazel/lexer/*.c)
objects = $(sources:Hazel/.c=.o)
flags = -g 

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

run:
	hello.out

compiler:
	gcc -c hazel/compiler/hazel.c

%.o: %.c Hazel/lexer/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm Hazel/*.o


