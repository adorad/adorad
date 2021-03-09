#ifndef HAZEL_PARSER
#define HAZEL_PARSER 

#include <hazel/runtime/lexer/lexer.h>
#include <hazel/runtime/ast/ast.h> 

typedef struct ParserDef {
    Lexer* lexer; 
    Token* curr_tok; 
} Parser;

Parser* parser_init(Lexer* lexer); 

// Expect method. When we call this func, we tell the Parser that we expect a certain token. 
// If we get an unexpected token, we're just going to die. 
// If we do get the expected token, we advance to the next token 
void parser_eat(Parser* parser, int tok_type); 

// Main entry point of the parser. 
// It will return the whole AST tree of the entire source code when parsed. 
Ast* parser_parse(Parser* parser); 


#endif // HAZEL_PARSER 