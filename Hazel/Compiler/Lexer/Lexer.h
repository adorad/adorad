#ifndef _HAZEL_LEXER
#define _HAZEL_LEXER 

#include <Hazel/Core/HCore.h> 
#include <Hazel/Compiler/Tokens/Tokens.h>

/*
    Hazel's Lexer is built in such a way that no (or negligible) memory allocations are necessary during usage. 

    In order to be able to not allocate any memory during tokenization, STRINGs and NUMBERs are just sanity checked but _not_
    converted - it is the responsibility of the Parser to perform the right conversion.

    In case of a scan error, ILLEGAL is returned and the error details can be extracted from the token itself.

    Reference: 
        1. ASCII Table: http://www.theasciicode.com.ar
*/


typedef struct LexerStruct {
    char* buffer;         // buffer
    UInt32 position;      // current buffer position (in characters)
    UInt32 buffer_length; // current buffer length (in Bytes)
    UInt32 offset;        // current buffer offset (in Bytes)
                          // offset of the beginning of the line (no. of chars b/w the beginning of the Lexical Buffer
                          // and the beginning of the line)

    char curr_char;   // current char 
    Token token;      // current token
    UInt32 char_idx;  // the index of the token
    UInt32 line_no;   // the line number in the source where the token occured
    UInt32 col_no;    // the column number
    UInt32 fname;     // the file name
} Lexer; 


// Useful Macros for the Lexer 
#define NEXT            lexer->buffer[lexer->offset++]; \
                        ++lexer->position; INCREMENT_COLUMN
#define PEEK_CURR       (int)lexer->buffer[lexer->offset]
#define PEEK_NEXT       (lexer->offset < lexer->buffer_length ? (int)lexer->buffer[lexer->offset+1] : 0 
#define PEEK_NEXT2      (lexer->offset+1 < lexer->buffer_length ? (int)lexer->buffer[lexer->offset+2] : 0 

#define RESET_LINE      lexer->line_no = 1
#define RESET_COLUMN    lexer->col_no  = 1
#define LEXER_IS_EOF    lexer->offset >= lexer->buffer_length

#define INCREMENT_LINE     ++lexer->line_no; RESET_COLUMN
#define INCREMENT_COLUMN   ++lexer->col_no; 
#define DECREMENT_LINE     --lexer->line_no; RESET_COLUMN
#define DECREMENT_COLUMN   --lexer->col_no; 
#define INCREMENT_OFFSET   ++lexer->offset; INCREMENT_COLUMN
#define DECREMENT_OFFSET   --lexer->offset; DECREMENT_COLUMN
#define INCREMENT_POSITION ++lexer->offset;
#define DECREMENT_POSITION --lexer->offset;

#define INCREMENT_OFFSET_AND_POSITION       INCREMENT_OFFSET; INCREMENT_POSITION

// Useful Macros for Tokens
#define TOKEN_RESET         lexer->token = NO_TOKEN; \
                            lexer->token.position = lexer->position; \
                            lexer->token.value = lexer->buffer + lexer->offset; \
                            lexer->token.line_no = lexer->line_no; \
                            lexer->token.col_no = lexer->col_no;

#define TOKEN_FINALIZE(__t)    lexer->token.type = __t; \
                               lexer->token.fname = lexer->fname
#define INCREMENT_TOKENBYTES   ++lexer->token.bytes
#define DECREMENT_TOKENBYTES   --lexer->token.bytes
#define INCREMENT_TOKENLENGTH  ++lexer->token.length
#define DECREMENT_TOKENLENGTH  -- lexer->token.length


Lexer* lexer_init(char* buffer); 
void lexer_free(Lexer* lexer); 

Token* lexer_get_next_token(Lexer* lexer); 
Token* lexer_advance_with_token(Lexer* lexer, int type); 

void lexer_advance(Lexer* lexer); 
void lexer_expect_char(Lexer* lexer, char c);
void lexer_skip_whitespace(Lexer* lexer); 
void lexer_skip_inline_comment(Lexer* lexer);
void lexer_skip_block_comment(Lexer* lexer);

Token* lexer_lex_string(Lexer* lexer); 
Token* lexer_lex_operator(Lexer* lexer);
Token* lexer_lex_char(Lexer* lexer); 
Token* lexer_lex_digit(Lexer* lexer); 
Token* lexer_lex_token_id(Lexer* lexer); 

char* lexer_lex_charstr(Lexer* lexer);

static inline bool isBuiltinOperator (int c);
static inline bool isIdentifier(char c);
static inline bool isNewLine(Lexer* lexer, char c);
static inline bool isSlashComment(char c1, char c2);
static inline bool isHashComment(char c);
static inline bool isSemicolon(char c);
static inline bool isString(char c);
static inline bool isMacro(char c);


#endif // _HAZEL_LEXER