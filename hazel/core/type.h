// A minimal rewrite of C's ctype.h

static inline bool ignore(char c) {
    return (c==' ' || c=='\t' || c=='\r');
}

static inline bool isWhitespace(char c) {
    return (c==' ' || (int)c==10 || (int)c==13);
}

static inline bool isLetter(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'));
}

static inline bool isUpper(char c) {
    return (c>='A' && c<='Z');
}

static inline bool isLower(char c) {
    return (c>='a' && c<='z');
}

static inline bool isAlpha(char c) {
    return (isUpper(c) || isLower(c));
}

static inline bool isAlnum(char c) {
    return (isAlpha(c) || isDigit(c));
}

static inline bool isDigit(char c) {
    return ((c >= '0' && c <= '9'));
}

static inline bool isNewLine(char c) {
    return c == '\n';
}

static inline bool isEnd(char c) {
    return c == '\0';
}