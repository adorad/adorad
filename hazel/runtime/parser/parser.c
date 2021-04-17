#include <stdio.h> 
#include <string.h> 

#include <hazel/runtime/parser/parser.h> 
#include <hazel/runtime/ast/ast.h> 

Parser* parser_init(Lexer* lexer) {
    Parser* parser = calloc(1, sizeof(struct ParserDef));
    parser->lexer = lexer; 
    parser->curr_tok = lexer_get_next_token(lexer);
    parser->prev_tok = parser->curr_tok;

    return parser; 
}

void parser_chomp(Parser* parser, int tok_type) {
    if(parser->curr_tok->type == tok_type) {
        parser->prev_tok = parser->curr_tok;
        parser->curr_tok = lexer_get_next_token(parser->lexer);
    } else {
        printf("Unexpected token `%s` of type `%d`", parser->curr_tok->value, parser->curr_tok->type);
        exit(1);
    }
}

// Main entry point of the parser. 
// It will return the whole AST tree of the entire source code when parsed. 
Ast* parser_parse(Parser* parser) {
    return parser_parser_stmts(parser);
}

Ast* parser_parse_id(Parser* parser) {
    if(strcmp(parser->curr_tok->value, "var") == 0) {
        return parser_parse_variable_definition(parser);
    } else {
        // If we don't recognize the TOK_ID, we assume that the user is attempting to access a variable
        return parser_parse_variable(parser);
    }
}

// Parse a single statement
Ast* parser_parse_stmt(Parser* parser) {
    switch (parser->curr_tok->type) {
        case TOK_ID: {
            return parser_parse_id(parser);
        }
    }
    return ast_init(AST_NOOP);
}

// Parse multiple statements
Ast* parser_parser_stmts(Parser* parser) {
    // Create a compound-style AST node 
    Ast* compound = ast_init(AST_COMPOUND);
    compound->compound_value = calloc(1, sizeof(struct AstDef));

    // Parse first statement
    Ast* ast_statement = parser_parse_stmt(parser);
    compound->compound_value[0] = ast_statement; 
    compound->compound_size += 1;

    // SEMICOLON is the statement delimiter
    // TODO(jasmcaus) Remove this need.
    while(parser->curr_tok->type == SEMICOLON) {
        parser_chomp(parser, SEMICOLON);

        Ast* ast_statement = parser_parse_stmt(parser);
        compound->compound_size += 1;
        compound->compound_value = realloc(compound->compound_value, compound->compound_size*sizeof(struct AstDef));
        // Put the new statement at the last index of the list 
        compound->compound_value[compound->compound_size-1] = ast_statement; 
    }
    return compound;
}
// Parse mathematical expressions
// Eg: 1 + 2 * 3
Ast* parser_parse_expression(Parser* parser) {
    switch(parser->curr_tok->type) {
        case STRING: return parser_parse_string(parser);
    }
}
// Parse things that are multiplied
// Eg: 1 * 2 * (3+4)
Ast* parser_parse_factor(Parser* parser) {

}
// Parse things that are added
// Eg: 1 + 2 + (3*4)
Ast* parser_parse_term(Parser* parser) {

}

Ast* parser_parse_function_call(Parser* parser) {

}

Ast* parser_parse_variable(Parser* parser) {
    char* tok_value = parser->curr_tok->value; 
    parser_chomp(parser, TOK_ID); 

    if(parser->curr_tok->type == LPAREN)
        return parser_parse_function_call(parser); 
    
    Ast* ast_var = ast_init(AST_VARIABLE); 
    ast_var->var_name = tok_value; 

    return ast_var; 
}

Ast* parser_parse_variable_definition(Parser* parser) {
    parser_chomp(parser, TOK_ID); // var
    char* var_definition_var_name = parser->curr_tok->value;
    parser_chomp(parser, TOK_ID); // variable name 
    parser_chomp(parser, EQUALS); // ="
    Ast* var_definition_value = parser_parse_expression(parser); 

    Ast* var_definition = ast_init(AST_VARIABLE_DEFINITION);
    var_definition->var_definition_var_name = var_definition_var_name; 
    var_definition->var_definition_value = var_definition_value; 
}

Ast* parser_parse_string(Parser* parser) {
    Ast* ast_str = ast_init(STRING); 
    ast_str->string_value = parser->curr_tok->value; 
    
    parser_chomp(parser, STRING); 
    return ast_str; 
}