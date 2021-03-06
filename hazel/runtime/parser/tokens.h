#ifndef HAZEL_TOKEN
#define HAZEL_TOKEN 

// token.h defines constants representing the lexical tokens of the Hazel programming language and basic operations on 
// tokens (printing, predicates).

// Set of lexical tokens in the Hazel Programming Language
typedef struct TokenNames {
    enum {
        // Special (internal usage only)
        TOK_ID, 
        // Special Tokens
        TOK_EOF,        // EOF
        ERROR,      
        COMMENT,    // //
        WHITESPACE, // '\n  \t'
        IDENTIFIER, // foo, bar 
        AT_SIGN,    // @
        COMMA,      // ,
        SEMICOLON,  // ;

        // Keywords
        KEYWORD, 
        BEGIN, 
        BREAK, 
        CASE,
        CATCH, 
        CONST,
        CONTINUE, 
        DO, 
        DEF,
        DEFAULT, 
        ELSE,
        ELSEIF, 
        END, 
        EXPORT, 
        FINALLY, 
        FOR,
        // GLOBAL // ==> not required tbh coz we follow a no-globals approach
        IF,
        IMPORT,
        INCLUDE,
        MACRO,
        MATCH, // similar to 'switch' in C++, Java and others
        MODULE,
        MUTABLE,
        // NEW // ==> if we go for a memory-safety approach like Rust
        RANGE,
        RETURN, 
        STRUCT, 
        TRY, 
        TYPE, 
        USE,
        WHERE, 
        WHILE,

        // cstparser
        INVISIBLE_BRACKETS,
        NOTHING,
        WS, 
        SEMICOLON_WS,
        NEWLINE_WS,
        EMPTY_WS, 

        // Literals
        LITERAL,        // general
        INTEGER,        // 2
        BIN_INT,        // 0b1
        HEX_INT,        // 0x0
        FLOAT,          // 5.3, 2.6e+3
        STRING,         // "foo" or 'bar'
        IMAG,           // 123.45i
        TRIPLE_STRING,  // """ foobar \n """
        CHAR,           // 'c'
        TRUE, 
        FALSE,

        // Operators 
        ADD,            // +
        SUBTRACT,            // - 
        MUTLIPLICATION, // *
        QUOTIENT,       // /
        REM,            // %

        AND,       // &
        OR,        // |
        NOT,       // !
        XOR,       // ^
        AND_NOT,   // &^
        LAND,      // && 
        LOR,       // || 
        INCREMENT, // ++
        DECREMENT, // --

        // Comparison
        GREATER,                  // > 
        LESS,                     // <
        GREATER_THAN,             // >=
        GREATER_THAN_OR_EQUAL_TO, // ≥
        LESS_THAN,                // <=
        LESS_THAN_OR_EQUAL_TO,    // ≤
        DOUBLEEQUALS,             // == 
        NOT_EQUALS,               // !=
        NOT_EQUAL_TO,             // ≠
        IN,                       // in
        ISA,                      // isa

        // Delimiters
        LBRACK,  // [
        RBRACK,  // ]
        LBRACE,  // {
        RBRACE,  // }
        LPAREN,  // (
        RPAREN,  // )

        // Assignments
        EQUALS,           // = 
        PLUS_EQUALS,      // +=
        MINUS_EQUALS,     // -=
        START_EQUALS,     // *=
        OR_EQUALS,        // |=
        DIVISION_EQUALS,  // ÷=
        FWD_SLASH_EQUALS, // /=
        REM_EQUALS,       // %=
        LBITSHIFT_EQUALS, // <<=
        RBITSHIFT_EQUALS, // >>=
        AND_EQUALS,       // &=
        APPROX,           // ~ 

        // Colons
        COLON,    // :
        DOT,      // .
        DDOT,     // ..
        ELLIPSIS, // ...

        // Bitshifts
        LBITSHIFT, // << 
        RBITSHIFT, // >> 

        // Conditional
        CONDITIONAL, // ? 

        // Arrows
        PAIR_ARROWS,                  // =>
        ANON_FUNC,                    // -> 
        // RIGHT_ARROW,         // -> // DEFINED AS ANON_FUNC
        LEFT_ARROW,                   // <-
        LEFTWARDS_ARROW,              // ←
        RIGHTWARDS_ARROW,             // →
        HALFWIDTH_LEFTWARDS_ARROW,    // ￩
        HALFWIDTH_RIGHTWARDS_ARROW,   // ￫
        LONG_LEFTWARDS_ARROW,         // ⟵
        LONG_RIGHTWARDS_ARROW,        // ⟶ 

        // Declaration (we might/might not use this)
        DECLARATION, // :: 
        
        // Only to track the size of this enum 
        SIZE_OF_ENUM, 
    } type; // enum

    char* value; 
} Token; 


// Use a custom type here: 
// Like a Dict{TokenNames, String} mapping
// Useful only when a bootstrapped compiler is available
// Implementing a Dictionary in C is easy, but it has the potential to deride focus from Hazel which is not worth it.
// Something similar to Go's implementation: 
// https://github.com/golang/go/blob/964639cc338db650ccadeafb7424bc8ebb2c0f6c/src/go/token/token.go
// Dict{TokenNames, String} {
//     // Special Tokens
//     EOF        : "EOF"
//     COMMENT    : "COMMENT"
//     IDENTIFIER : "DENTIFIER"
//     AT_SIGN    : "@"
//     COMMA      : ","
//     SEMICOLON  : ";"

//     // Keywords
//     BEGIN     : "begin"
//     BREAK     : "break"
//     CASE      : "case"
//     CATCH     : "catch"
//     CONST     : "const"
//     CONTINUE  : "continue"
//     DO        : "do"
//     DEF       : "def"
//     DEFAULT   : "default"
//     ELSE      : "else"
//     ELSEIF    : "elseif"
//     EXPORT    : "export"
//     FINALLY   : "finally"
//     FOR       : "for"

//     // GLOBAL    : "global"
//     IF        : "if"
//     IMPORT    : "import"
//     INCLUDE   : "include"
//     MACRO     : "macro"
//     MATCH     : "match"
//     MODULE    : "module"
//     MUTABLE   : "mutable"
//     // NEW       : "new"
//     RANGE     : "range"
//     RETURN    : "return"
//     STRUCT    : "struct"
//     TRY       : "try"
//     TYPE      : "type"
//     USE       : "use"
//     WHERE     : "where"
//     WHILE     : "while"

//     // Literals
//     INTEGER : "INTEGER"
//     FLOAT   : "FLOAT"
//     STRING  : "STRING"
//     IMAG    : "IMAG"
//     CHAR    : "CHAR"
//     TRUE    : "TRUE"
//     FALSE   : "FALSE"

//     // Operators 
//     ADD            : "+"
//     SUB            : "- "
//     MUTLIPLICATION : "*"
//     QUOTIENT       : "/"
//     REM            : "%"

//     AND       : "&"
//     OR        : "|"
//     NOT       : "!"
//     XOR       : "^"
//     AND_NOT   : "&^"
//     LAND      : "&&"
//     LOR       : "||"
//     INCREMENT : "++"
//     DECREMENT : "--"

//     // Comparison
//     GREATER                  : ">" 
//     LESS                     : "<"
//     GREATER_THAN             : ">="
//     GREATER_THAN_OR_EQUAL_TO : "≥"
//     LESS_THAN                : "<="
//     LESS_THAN_OR_EQUAL_TO    : '≤'
//     DOUBLEEQUALS             : "==" 
//     NOT_EQUALS               : '!='
//     NOT_EQUAL_TO             : "≠"
//     IN                       : "in"
//     ISA                      : "isa"

//     // Delimiters
//     LBRACK  : "["
//     RBRACK  : "]"
//     LBRACE  : "{"
//     RBRACE  : "}"
//     LPAREN  : "("
//     RPAREN  : ")"

//     // Assignments
//     EQUALS           : "=" 
//     PLUS_EQUALS      : "+="
//     MINUS_EQUALS     : "-="
//     START_EQUALS     : "*="
//     OR_EQUALS        : "|="
//     DIVISION_EQUALS  : "÷="
//     FWD_SLASH_EQUALS : "/="
//     REM_EQUALS       : "%="
//     LBITSHIFT_EQUALS : "<<="
//     RBITSHIFT_EQUALS : ">>="
//     AND_EQUALS       : "&="
//     APPROX           : "~"

//     // Colons
//     COLON    : ":"
//     DOT      : "."
//     DDOT     : ".."
//     ELLIPSIS : "..."

//     // Bitshifts
//     LBITSHIFT : "<<"
//     RBITSHIFT : ">>"

//     // Conditional
//     CONDITIONAL : "?"

//     // Arrows
//     PAIR_ARROWS                : "=>"
//     ANON_FUNC                  : "->"
//     LEFT_ARROW                 : "<-"
//     LEFTWARDS_ARROW            : "←"
//     RIGHTWARDS_ARROW           : "→"
//     HALFWIDTH_LEFTWARDS_ARROW  : "￩"
//     HALFWIDTH_RIGHTWARDS_ARROW : "￫"
//     LONG_LEFTWARDS_ARROW       : "⟵"
//     LONG_RIGHTWARDS_ARROW      : "⟶ "

//     // Declaration (we might/might not use this)
//     DECLARATION : "::"
    
// }; // custom Dict 'tokens'


// Returns the string corresponding to the Token tok. 
// For operators, delimiters and keywords, the string is the actual token character sequence (e.g. for the token ADD,
// the string is "+"). For all other tokens, the string corresponds to the token constant name (e.g. for the token
// IN, the string is "IN")
// const char* get_tok_string(TokenNames tok) {
//     char c = '';
//     enum TokenNames enumsize; 
//     enumsize = SIZE_OF_ENUM;

//     if(tok>=0 && tok > TokenNames(enumsize)) {
//         c = tokens[tok];
//     }
//     if c == '' {
//         c = "token(" + tokens.strconv(int(tok)) + ")";
//     }
//     return c;
// }


Token* token_init(int type, char* value);


#endif // HAZEL_TOKEN 
