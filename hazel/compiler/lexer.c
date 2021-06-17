/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive * Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#include <hazel/compiler/lexer.h>
#include <hazel/core/debug.h>
#include <hazel/core/string.h>
#include <stdarg.h>

// String representation of a TokenKind
// To access the string representation of a TokenKind object, simply use `tokenHash[tokenkind]`
// NB: This is here as opposed to in `tokens.[hc]` because it isn't used there and simply leads to more headaches
// (multiple redefinition errors yada yada)
static const char* tokenHash[] = {
    #define TOKENKIND(kind, str)    str
        ALLTOKENS
    #undef TOKENKIND
};

// These macros are used in the switch() statements below during the Lexing of Hazel source files.
#define WHITESPACE_NO_NEWLINE \
    ' ': case '\r': case '\t': case '\v': case '\f'

#define DIGIT_NON_ZERO  \
    '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'

#define DIGIT \
    '0': case DIGIT_NON_ZERO

#define HEX_DIGIT \
    'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': \
    case DIGIT

#define ALPHA_EXCEPT_B_O_X \
         'a': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': \
    case 'm': case 'n': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'y': \
    case 'z': case 'A': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': \
    case 'L': case 'M': case 'N':case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W':  \
    case 'Y': case 'Z'  \

#define ALPHA \
         'b': case 'o': case 'x': case 'B': case 'O': case 'X': case ALPHA_EXCEPT_B_O_X


Lexer* lexer_init(const char* buffer, const char* fname) {
    Lexer* lexer = (Lexer*)calloc(1, sizeof(Lexer));
    lexer->buffer = buffer; 
    lexer->buffer_capacity = strlen(buffer);
    lexer->offset = 0;

    // Tokens
    lexer->tokenList = vec_new(sizeof(Token), TOKENLIST_ALLOC_CAPACITY);

    if(!fname)
        fname = "";

    // Location
    lexer->lineno = 1;
    lexer->colno = 1;
    lexer->fname = fname;
    return lexer;
}

static void lexer_tokenlist_push(Lexer* lexer, Token* token) {
    lexer->tokenList->push(lexer->tokenList, token);
}

static void lexer_free(Lexer* lexer) {
    free(lexer);
}

// Report an error and exit
void lexer_error(Lexer* lexer, const char* format, ...) {
    va_list vl;
    va_start(vl, format);
    fprintf(stderr, "%sSyntaxError: ", "\033[1;31m");
    vfprintf(stderr, format, vl);
    fprintf(stderr, " at %s:%d:%d%s\n", lexer->fname, lexer->lineno,lexer->colno, "\033[0m");
    va_end(vl);
    exit(1);
}

// Returns the curent character in the Lexical Buffer and advances to the next element
// It does this by incrementing the buffer offset.
static inline char lexer_advance(Lexer* lexer) {
    if(lexer->offset >= lexer->buffer_capacity)
        return nullchar; 
    
    LEXER_INCREMENT_COLNO;
    return (char)lexer->buffer[lexer->offset++];
}

// Advance `n` characters in the Lexical Buffer
static inline char lexer_advancen(Lexer* lexer, UInt32 n) {
    // The '>=' is here because offset and buffer_capacity are off by 1 (0-index vs 1-index respectively)
    if(lexer->offset + n >= lexer->buffer_capacity)
        return nullchar;
    
    lexer->colno += n;
    lexer->offset += n;
    return (char)lexer->buffer[lexer->offset];
}

// Returns the previous `n` elements in the Lexical buffer.
// This is non-destructive -- the buffer offset is not updated.
static inline char lexer_prev(Lexer* lexer, UInt32 n) {
    if(lexer->offset == 0)
        return nullchar;

    return (char)lexer->buffer[lexer->offset - n];

}

// Returns the current element in the Lexical Buffer.
static inline char lexer_peek(Lexer* lexer) {
    return (char)lexer->buffer[lexer->offset];
}

// "Look ahead" `n` characters in the Lexical buffer.
// It _does not_ increment the buffer offset.
static inline char lexer_peekn(Lexer* lexer, UInt32 n) {
    if(lexer->offset + n >= lexer->buffer_capacity)
        return nullchar;
    
    return (char)lexer->buffer[lexer->offset + n];
}

static void lexer_maketoken(Lexer* lexer, TokenKind kind, char* value) {  
    Token* token = (Token*)calloc(1, sizeof(Token));
    CSTL_CHECK_NOT_NULL(token, "Could not allocate memory. Memory full.");

    if(value == null && (kind == STRING || kind == IDENTIFIER || kind == INTEGER || kind == HEX_INT || 
                         kind == BIN_INT || kind == OCT_INT))
        CSTL_WARN("Expected a token value. Got `null`\n");

    if(value == null)
        value = token_toString(kind);

    token->kind = kind;
    token->offset = lexer->offset;
    token->colno = lexer->colno;
    token->lineno = lexer->lineno;
    token->fname = lexer->fname;
    token->value = value;

    lexer_tokenlist_push(lexer, token);
}

// Scan a comment (single line)
static inline void lexer_lex_sl_comment(Lexer* lexer) {
    char ch = lexer_advance(lexer);
    while(ch && ch != '\n')
        ch = lexer_advance(lexer);
}

// Scan a comment (multi-line)
static inline void lexer_lex_ml_comment(Lexer* lexer) {
    char ch = lexer_advance(lexer);
    bool asterisk_found = false; 
    while(ch && !(ch == '/' && asterisk_found)) {
        asterisk_found = false; 
        while(ch && ch!= '*')
            ch = lexer_advance(lexer);
        
        if(ch == '*')
            asterisk_found = true;

        ch = lexer_advance(lexer);
    }
    ch = lexer_advance(lexer);
}

// Scan a character
static inline void lexer_lex_char(Lexer* lexer) {
    char ch = lexer_advance(lexer);
    if(ch) {
        LEXER_INCREMENT_OFFSET;
        if(ch == '\n') {
            LEXER_INCREMENT_LINENO;
            LEXER_RESET_COLNO;
        }
    }
}

// Scan an escape char
static inline void lexer_lex_esc_char(Lexer* lexer) {
}

// Scan a string
static inline void lexer_lex_string(Lexer* lexer) {
    // We already know that the curr char is _not_ a quote (`"`) --> an empty string token (`""`) is
    // handled by `lexer_lex()`
    CSTL_CHECK_NE(LEXER_CURR_CHAR, '"');

    char ch = lexer_advance(lexer);
    char* str_value = (char*)calloc(MAX_TOKEN_SIZE, sizeof(char));
    UInt32 prev_offset = lexer->offset - 1;

    while(ch != '"') {
        if(ch == '\\') {
            // lexer_lex_esc_char(lexer);
            ch = lexer_advance(lexer);
        } else {
            ch = lexer_advance(lexer);
        }
    }
    CSTL_CHECK_EQ(ch, '"');
    UInt32 offset_diff = lexer->offset - prev_offset;
    // offset_diff should never be 0 because we already handle the `""` case in `lexer_lex()`
    CSTL_CHECK_NE(offset_diff, 0);
    // `offset_diff - 1` so as to ignore the closing quote `"` from being copied into `str_value`
    substr(str_value, lexer->buffer, prev_offset, offset_diff - 1);
    CSTL_CHECK_NOT_NULL(str_value, "str_value must not be null");
    lexer_maketoken(lexer, STRING, str_value);
}

// Returns whether `value` is a keyword or an identifier
static inline TokenKind lexer_is_keyword_or_identifier(char* value) {
    // Search `tokenHash` for a match for `value`. 
    // If we can't find one, we assume an identifier
    bool has_found_keyword = false;
    for(int tokenkind = TOK___KEYWORDS_BEGIN + 1; tokenkind < TOK___KEYWORDS_END; tokenkind++) {
        if(strcmp(tokenHash[tokenkind], value) == 0) {
            // Found a match
            return tokenkind;
        }
    }

    // If we're still here, we haven't found a keyword match
    return IDENTIFIER;
}

// Scan an identifier
static inline void lexer_lex_identifier(Lexer* lexer) {
    // When this function is called, we alread know that the first character statisfies the `case ALPHA`.
    // So, the remaining characters are ALPHA, DIGIT, or `_`
    char* ident_value = (char*)calloc(MAX_TOKEN_SIZE, sizeof(char));
    char ch = lexer_advance(lexer);
    UInt32 prev_offset = lexer->offset - 1;

    while(isLetter(ch) || isDigit(ch))
        ch = lexer_advance(lexer);

    UInt32 offset_diff = lexer->offset - prev_offset;
    CSTL_CHECK_NE(offset_diff, 0);
    substr(ident_value, lexer->buffer, prev_offset - 1, offset_diff);
    CSTL_CHECK_NOT_NULL(ident_value, "ident_value must not be null");

    // Determine if a keyword or just a regular identifier
    TokenKind tokenkind = lexer_is_keyword_or_identifier(ident_value);
    lexer_maketoken(lexer, tokenkind, ident_value);
}

static inline void lexer_lex_digit(Lexer* lexer) {
    // 0x... --> Hexadecimal ("0x"|"0X")[0-9A-Fa-f_]+
    // 0o... --> Octal       ("0o"|"0O")[0-7_]+
    // 0b... --> Binary      ("0b"|"0B")[01_]+
    // This cannot be `lexer_advance(lexer)` because we enter here from `lexer_lex()` where we already
    // know that the first char is a digit value. 
    // This value needs to be captured as well in `token->value`
    char ch = lexer_prev(lexer, 1);
    UInt32 prev_offset = lexer->offset - 1;
    TokenKind tokenkind = TOK_ILLEGAL;
    char* digit_value = (char*)calloc(MAX_TOKEN_SIZE, sizeof(char));

    CSTL_CHECK_TRUE(isDigit(ch));
    while(isDigit(ch)) {
        // Hex, Octal, or Binary?
        if(ch == '0') {
            ch = lexer_advance(lexer);
            switch(ch) {
                // Hex
                case 'x': case 'X':;
                    // Skip [xX]
                    ch = lexer_advance(lexer);
                    int hexcount = 0;
                    while(isHexDigit(ch)) {
                        ++hexcount; 
                        ch = lexer_advance(lexer);
                    }
                    if(hexcount == 0)
                        lexer_error(lexer, "Expected hexadecimal digits [0-9A-Fa-f] after `0x`");
                    
                    tokenkind = HEX_INT;
                    break;
                // Binary
                case 'b': case 'B':
                    // Skip [bB]
                    ch = lexer_advance(lexer);
                    int bincount = 0;
                    while(isBinaryDigit(ch)) {
                        ++bincount; 
                        ch = lexer_advance(lexer);
                    }
                    if(bincount == 0)
                        lexer_error(lexer, "Expected binary digit [0-1] after `0b`");
                    
                    tokenkind = BIN_INT;
                    break;
                // Octal
                // Depart from the (error-prone) C-style octals with an inital zero e.g 0123
                // Instead, we support the `0o` or `0O` prefix, like 0o123
                case 'o': case 'O':
                    // Skip [oO]
                    ch = lexer_advance(lexer);
                    int octcount = 0;
                    while(isOctalDigit(ch)) {
                        ++octcount; 
                        ch = lexer_advance(lexer);
                    }
                    if(octcount == 0)
                        lexer_error(lexer, "Expected octal digits [0-7] after `0o`");
                    
                    tokenkind = OCT_INT;
                    break;
                case ALPHA_EXCEPT_B_O_X:
                    // Error
                    lexer_error(lexer, "Invalid character `%c`. Hazel currently supports [xXbBoO] after `0`", ch);
                    break;
                default:
                    // An integer?
                    // lexer_error(lexer, "Cannot have an integer beginning with `0`");     
                    break;               
            } // switch(ch)
        }
        // Fractions, or Integer?
        else {
            // Normal Floats
            if(ch == '.' || ch == '_')
                ch = lexer_advance(lexer);
            
            // Exponents (Float)
            else if(ch == 'e' || ch == 'E') {
                // Skip over [eE]
                ch = lexer_advance(lexer);
                if(ch == '+' || ch == '-') { 
                    ch = lexer_advance(lexer);
                } else {
                    lexer_error(lexer, "Expected [+-] after exponent `e`. Got `%c`", ch);
                }
                
                int exp_digits = 0;
                while(isDigit(ch)) {
                    ch = lexer_advance(lexer);
                    ++exp_digits;
                }
                if(exp_digits == 0)
                    lexer_error(lexer, "Invalid character after exponent `e`. Expected a digit, got `%c`", ch);
            }
            // Imaginary
            else if(ch == 'j' || ch == 'J') {
            }
        }
        ch = lexer_advance(lexer);
    }

    CSTL_CHECK(tokenkind != TOK_ILLEGAL);
    substr(digit_value, lexer->buffer, prev_offset, lexer->offset - prev_offset - 1);
    CSTL_CHECK_NOT_NULL(digit_value, "digit_value must not be null");
    lexer_maketoken(lexer, tokenkind, digit_value);
}

// Lex the Source files
static void lexer_lex(Lexer* lexer) {
    // Some UTF8 text may start with a 3-byte 'BOM' marker sequence. If it exists, skip over them because they 
    // are useless bytes. Generally, it is not recommended to add BOM markers to UTF8 texts, but it's not 
    // uncommon (especially on Windows).
    if(lexer->buffer[0] == (char)0xef && lexer->buffer[1] == (char)0xbb && lexer->buffer[2] == (char)0xbf) {
        lexer_advancen(lexer, 3);
    }

    char next = nullchar;
    char curr = nullchar;
    TokenKind tokenkind = TOK_ILLEGAL;

    // NB: The fact that `lexer->offset > lexer->buffer_capacity` is handled by the first case block (nullchar).
    // This is faster (performance-wise) than repeatedly checking for the above condition.
    while(true) { 
        // `lexer_advance()` returns the current character and moves forward, and `lexer_peek()` returns the current
        // character (after the advance).
        // For example, if we start from buff[0], 
        //      curr = buff[0]
        //      next = buff[1]
        curr = lexer_advance(lexer);
        next = lexer_peek(lexer);
        tokenkind = TOK_ILLEGAL;

        switch(curr) {
            case nullchar: goto lex_eof;
            // The `-1` is there to prevent an ILLEGAL token kind from being appended to `lexer->tokenList`
            // NB: Whitespace as a token is useless for our case (will this change later?)
            case WHITESPACE_NO_NEWLINE: tokenkind = -1; break;
            case '\n':
                LEXER_INCREMENT_LINENO;
                LEXER_RESET_COLNO;
                tokenkind = -1;
                break;
            // Identifier
            case ALPHA: case '_': tokenkind = -1; lexer_lex_identifier(lexer); break;
            // Do _NOT_ include '0' because that clashes with `nullchar`
            case DIGIT: tokenkind = -1; lexer_lex_digit(lexer); break;
            case '"': 
                switch(next) {
                    // Empty String literal 
                    case '"': LEXER_INCREMENT_OFFSET; lexer_maketoken(lexer, STRING,  "\"\""); break;
                    default: tokenkind = -1; lexer_lex_string(lexer); break;
                }
                break;
            case ';': LEXER_INCREMENT_OFFSET; tokenkind = SEMICOLON; break;
            case ',': LEXER_INCREMENT_OFFSET; tokenkind = COMMA; break;
            case '\\': LEXER_INCREMENT_OFFSET; tokenkind = BACKSLASH; break;
            case '[': LEXER_INCREMENT_OFFSET; tokenkind = LSQUAREBRACK; break;
            case ']': LEXER_INCREMENT_OFFSET; tokenkind = RSQUAREBRACK; break;
            case '{': LEXER_INCREMENT_OFFSET; tokenkind = LBRACE; break;
            case '}': LEXER_INCREMENT_OFFSET; tokenkind = RBRACE; break;
            case '(': LEXER_INCREMENT_OFFSET; tokenkind = LPAREN; break;
            case ')': LEXER_INCREMENT_OFFSET; tokenkind = RPAREN; break;
            case '=':
                switch(next) {
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind = EQUALS_EQUALS; break;
                    case '>': LEXER_INCREMENT_OFFSET; tokenkind = EQUALS_ARROW; break;
                    default: tokenkind = EQUALS; break;
                }
                break;
            case '+':
                switch(next) {
                    // This might be removed at some point. 
                    // '++' serves no purpose since Hazel doesn't (and won't) support pointer arithmetic.
                    case '+': LEXER_INCREMENT_OFFSET; tokenkind = PLUS_PLUS; break;
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind  = PLUS_EQUALS; break;
                    default: tokenkind = PLUS; break;
                }
                break;
            case '-':
                switch(next) {
                    // This might be removed at some point. 
                    // '--' serves no purpose since Hazel doesn't (and won't) support pointer arithmetic.
                    case '-': LEXER_INCREMENT_OFFSET; tokenkind = MINUS_MINUS; break;
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind = MINUS_EQUALS; break;
                    case '>': LEXER_INCREMENT_OFFSET; tokenkind = RARROW; break;
                    default: tokenkind = MINUS; break;
                } 
                break;
            case '*':
                switch(next) {
                    case '*': LEXER_INCREMENT_OFFSET; tokenkind = MULT_MULT; break;
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind = MULT_EQUALS; break;
                    default: tokenkind = MULT; break;
                }
                break;
            case '/':
                switch(next) {
                    // Add tokenkind here? 
                    // (TODO) jasmcaus
                    case '/': tokenkind = -1; lexer_lex_sl_comment(lexer); break;
                    case '*': tokenkind = -1; lexer_lex_ml_comment(lexer); break;
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind = SLASH_EQUALS; break;
                    default: tokenkind = SLASH; break;
                }
                break;
            case '#': 
                // Ignore shebang on the first line
                if(lexer->lineno == 1 && next == '!' && lexer_peekn(lexer, 1) == '/') {
                    tokenkind = -1;
                    // Skip till end of line
                    while(LEXER_CURR_CHAR && LEXER_CURR_CHAR != '\n')
                        curr = lexer_advance(lexer);
                }
                else {
                    LEXER_INCREMENT_OFFSET; 
                    tokenkind = HASH_SIGN;
                }
                break;
            case '!':
                switch(next) {
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind = EXCLAMATION_EQUALS; break;
                    default: tokenkind = MINUS_MINUS; break;
                }
                break;
            case '%':
                switch(next) {
                    case '%': LEXER_INCREMENT_OFFSET; tokenkind = MOD_MOD; break;
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind = MOD_EQUALS; break;
                    default: tokenkind = MOD; break;
                }
                break;
            case '&':
                switch(next) {
                    case '&': LEXER_INCREMENT_OFFSET; tokenkind = AND_AND; break;
                    case '^': LEXER_INCREMENT_OFFSET; tokenkind = AND_NOT; break;
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind = AND_EQUALS; break;
                    default: tokenkind = AND; break;
                }
                break;
            case '|':
                switch(next) {
                    case '|': LEXER_INCREMENT_OFFSET; tokenkind = OR_OR; break;
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind = OR_EQUALS; break;
                    default: tokenkind = OR; break;
                }
                break;
            case '^':
                switch(next) {
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind = XOR_EQUALS; break;
                    default: tokenkind = XOR; break;
                }
                break;
            case '<':
                switch(next) {
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind = LESS_THAN_OR_EQUAL_TO; break;
                    case '-': LEXER_INCREMENT_OFFSET; tokenkind = LARROW; break;
                    case '<': 
                        LEXER_INCREMENT_OFFSET;
                        char c = lexer_peek(lexer);
                        if(c == '=') {
                            LEXER_INCREMENT_OFFSET; tokenkind = LBITSHIFT_EQUALS;
                        } else {
                            tokenkind = LBITSHIFT;
                        }
                        break;
                    default: tokenkind = LESS_THAN; break;
                }
                break;
            case '>':
                switch(next) {
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind = GREATER_THAN_OR_EQUAL_TO; break;
                    case '>': 
                        LEXER_INCREMENT_OFFSET;
                        char c = lexer_peek(lexer);
                        if(c == '=') {
                            LEXER_INCREMENT_OFFSET; tokenkind = RBITSHIFT_EQUALS;
                        } else {
                            tokenkind = RBITSHIFT;
                        }
                        break;
                    default: tokenkind = GREATER_THAN; break;
                }
                break;
            case '~':
                switch(next) {
                    case '=': LEXER_INCREMENT_OFFSET; tokenkind = TILDA_EQUALS; break;
                    default: tokenkind = TILDA; break;
                }
                break;
            case '.':
                switch(next) {
                    case '.': 
                        LEXER_INCREMENT_OFFSET;
                        char c = lexer_peek(lexer);
                        if(c == '.') {
                            LEXER_INCREMENT_OFFSET; tokenkind = ELLIPSIS;
                        } else {
                            tokenkind = DDOT;
                        }
                        break;
                    // Fractions are possible here:
                    // Eg: `.0192` or `.9983838`
                    case DIGIT: tokenkind = -1; lexer_lex_digit(lexer); break;
                    default: tokenkind = DOT; break;
                }
                break;
            case ':':
                switch(next) {
                    case ':': LEXER_INCREMENT_OFFSET; tokenkind = COLON_COLON; break;
                    default: tokenkind = COLON; break;
                }
                break;
            case '?': tokenkind = QUESTION; break;
            case '@': tokenkind = MACRO; break;
            default:
                lexer_error(lexer, "Invalid character `%c`", curr);
                break;
        } // switch(ch)

        if(tokenkind == -1) continue;
        lexer_maketoken(lexer, tokenkind, null);
    } // while

lex_eof:;

    lexer_maketoken(lexer, TOK_EOF, null);
}