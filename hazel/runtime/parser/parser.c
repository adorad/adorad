#include <stdio.h> 

#include <hazel/runtime/parser/parser.h> 

Parser* parser_init(Lexer* lexer) {
    Parser* parser = calloc(1, sizeof(struct ParserDef));
    parser->lexer = lexer; 
    parser->curr_tok = lexer_get_next_token(lexer);

    return parser; 
}

void parser_eat(Parser* parser, int tok_type) {
    if(parser->curr_tok->type == tok_type) {
        parser->curr_tok = lexer_get_next_token(parser->lexer);
    } else {
        printf("Unexpected token `%s` of type `%d`", parser->curr_tok->value, parser->curr_tok->type);
        exit(1);
    }
}

// Main entry point of the parser. 
// It will return the whole AST tree of the entire source code when parsed. 
Ast* parser_parse(Parser* parser) {

}

// Parse a single statement
Ast* parser_parse_stmt(Parser* parser);
// Parse multiple statements
Ast* parser_parser_stmts(Parser* parser) {

}
// Parse mathematical expressions
// Eg: 1 + 2 * 3
Ast* parser_parse_expression(Parser* parse) {

}
// Parse things that are multiplied
// Eg: 1 * 2 * (3+4)
Ast* parser_parse_factor(Parser* parser) {

}
// Parse things that are added
// Eg: 1 + 2 + (3*4)
Ast* parser_parse_term(Parser* parser) {

}

Ast* parser_parse_function_call(Parser* parser);
Ast* parser_parse_variable(Parser* parser) {

}
Ast* parser_parse_string(Parser* parser);