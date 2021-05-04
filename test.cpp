#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#include <test.h>

#define LEXER_NEXT 		lexer->buffer[lexer->offset++]; lexer->col_no++
#define LEXER_PEEK      (int)lexer->buffer[lexer->offset]

Token* tokenize(Lexer* lexer) {
	// List of tokens
	char* tokens[500];
	int token = 0; 

	while(lexer->offset < lexer->buffer_capacity){
		char prev = LEXER_NEXT;
		char curr = LEXER_PEEK;
		switch(prev) {
			case '+':
				token = PLUS;
				break;
		}
		if(token == 0) {
			if(isalpha(prev)) {
				printf("%c%c", prev, curr);
				while(isalpha(lexer->buffer[lexer->offset++])) {
					printf("%c", lexer->buffer[lexer->offset]);
				}
				token = IDENTIFIER;
			} 
		}
	}
	return token; 
}

int main() {
	char* buffer = readFile("test/LexerDemo.hzl");
	Lexer* lexer = lexer_init(buffer); 

	printf("BUFFER: \n%s\n", lexer->buffer);
	printf("----------------------\n");

	Token* token = tokenize(lexer);
	printf("%d", token); 
}