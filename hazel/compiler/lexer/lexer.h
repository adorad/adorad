#ifndef _HAZEL_LEXER
#define _HAZEL_LEXER 

#include <hazel/core/hcore.h> 
#include <hazel/compiler/tokens/tokens.h>

/*
    Hazel's Lexer is built in such a way that no (or negligible) memory allocations are necessary during usage. 

    In order to be able to not allocate any memory during tokenization, STRINGs and NUMBERs are just sanity checked but _not_ converted - it is the responsibility of the Parser to perform the right conversion.

    Reference: 
        1. ASCII Table: http://www.theasciicode.com.ar
*/


typedef struct LexerStruct {
    char* buffer;         // buffer
    UInt32 position;      // current buffer position (in characters)
    UInt32 buffer_length; // current buffer length (in Bytes)
    UInt32 offset;        // current buffer offset (in Bytes)

    char curr_char;   // current char 
    TokensEnum token; // current token
    UInt32 char_idx;  // the index of the token
    UInt32 line_no;   // the line number in the source where the token occured
    UInt32 col_no;    // the column number
    UInt32 file_id;   // the file ID
} Lexer; 

// Useful Macros for the Lexer (might decide whether to put this in <lexer.h> at some later point in time)
#define NEXT            lexer->buffer[lexer->offset++]; ++lexer->position; INCREMENT_COLUMN
#define PEEK_CURR       (int)lexer->buffer[lexer->offset]
#define PEEK_NEXT       (lexer->offset < lexer->length ? (int)lexer->buffer[lexer->offset+1] : 0 
#define PEEK_NEXT2      (lexer->offset+1 < lexer->length ? (int)lexer->buffer[lexer->offset+2] : 0 

#define INCREMENT_LINE     ++lexer->line_no; RESET_COLUMN
#define INCREMENT_COLUMN   ++lexer->col_no; 
#define DECREMENT_LINE     --lexer->line_no; RESET_COLUMN
#define DECREMENT_COLUMN   --lexer->col_no; 
#define INCREMENT_OFFSET   ++lexer->offset; INCREMENT_COLUMN
#define DECREMENT_OFFSET   --lexer->offset; DECREMENT_COLUMN
#define INCREMENT_POSITION ++lexer->offset;
#define DECREMENT_POSITION --lexer->offset;

#define RESET_LINE      lexer->line_no = 1
#define RESET_COLUMN    lexer->col_no  = 1
#define LEXER_IS_EOF    lexer->offset >= lexer->length

// Useful Macros for Tokens (might decide whether to put this in <tokens.h> at some later point in time)
#define TOKEN_RESET         lexer->token = NO_TOKEN; \
                            lexer->token.position = lexer->position \
                            lexer->token.value = lexer->buffer + lexer->offset; \
                            lexer->token.line_no = lexer->line_no; \ 
                            lexer->token.col_no = lexer->col_no;

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

Token* lexer_collect_string(Lexer* lexer); 
Token* lexer_collect_char(Lexer* lexer); 
Token* lexer_collect_digit(Lexer* lexer); 
Token* lexer_collect_token_id(Lexer* lexer); 

char* lexer_collect_charstr(Lexer* lexer);

inline bool isBuiltinOperator (int c);
inline bool isIdentifier(char c);
inline bool isNewLine(Lexer* lexer, char c);
inline bool isSlashComment(char c1, char c2);
inline bool isHashComment(char c);
inline bool isSemicolon(char c);
inline bool isString(char c);
inline bool isMacro(char c);


#endif // _HAZEL_LEXER