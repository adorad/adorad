/** 
    Jason Dsouza's (jasmcaus) C Helper Library 
    Available in the public domain
    Use at your own risk - no warranty implied

    This is a single header file that replaces (and makes modern) a bunch of useful stuff from 
    the C/C++ Standard Library

LICENSE
    This software is dual-licensed to the public domain and under the following license: you are 
    granted a perpetual, irrevocable license to copy, modify, 	publish, and distribute this file 
    as you see fit.

WARNING
    - This library is **slightly** experimental and features may not work as expected.
    - This also means that many functions are not documented.
CREDITS
    Written by Jason Dsouza <@jasmcaus>
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <adorad/core/adcore.h>

// -------------------------------------------------------------------------
// buffer.c
// -------------------------------------------------------------------------

// Create a new `cstlBuffer`
cstlBuffer* buff_new(char* buff_data) {
    cstlBuffer* buffer = cast(cstlBuffer*)calloc(1, sizeof(cstlBuffer));
    CORETEN_ENFORCE_NN(buffer, "Could not allocate memory. Memory full.");

    buffer->is_utf8 = false;
    buff_set(buffer, buff_data);

    return buffer;
}

// Return the n'th character in the buffer data
char buff_at(cstlBuffer* buffer, UInt64 n) {
    CORETEN_ENFORCE_NN(buffer, "Expected not null");
    CORETEN_ENFORCE_NN(buffer->data, "Expected not null");
    
    if(n >= buffer->len)
        return nullchar;

    return (char)buffer->data[n];
}

// Returns a pointer to the beginning of the buffer data
char* buff_begin(cstlBuffer* buffer) {
    CORETEN_ENFORCE_NN(buffer, "Expected not null");
    CORETEN_ENFORCE_NN(buffer->data, "Expected not null");

    return cast(char*)buffer->data;
}

// Returns a pointer to the end of the buffer data
char* buff_end(cstlBuffer* buffer) {
    CORETEN_ENFORCE_NN(buffer, "Expected not null");
    CORETEN_ENFORCE_NN(buffer->data, "Expected not null");

    return cast(char*)(buffer->data + ((buffer->len - 1) * sizeof(char)));
}

// Is the buffer data empty?
bool buff_is_empty(cstlBuffer* buffer) {
    CORETEN_ENFORCE_NN(buffer, "Expected not null");
    CORETEN_ENFORCE_NN(buffer->data, "Expected not null");

    return buffer->len == 0;
}

// Returns the length of the buffer
// Note: The implementation for this was taken from:
// https://github.com/lattera/glibc/blob/master/string/strlen.c
// All due credit for this goes to the rightful author.
UInt32 __internal_strlength(const char* str, bool is_utf8) {
    if(!is_utf8) {
        const char* char_ptr;
        const unsigned long int* longword_ptr;
        unsigned long int longword, himagic, lomagic;

        // Handle the first few characters by reading one character at a time.
        // Do this until CHAR_PTR is aligned on a longword boundary.
        for(char_ptr = str; (cast(unsigned long int)char_ptr & (sizeof(longword) - 1)) != 0; ++char_ptr) {
            if (*char_ptr == nullchar)
                return char_ptr - str;
        }

        // All these elucidatory comments refer to 4-byte longwords, but the theory applies equally 
        // well to 8-byte longwords
        longword_ptr = cast(unsigned long int*)char_ptr;

        // Bits 31, 24, 16, and 8 of this number are zero. Call these bits the "holes".
        // Note that there is a hole just to the left of each byte, with an extra at the end:
        //     bits:  01111110 11111110 11111110 11111111
        //     bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD
        // The 1-bits make sure that carries propagate to the next 0-bit.
        // The 0-bits provide holes forcarries to fall into.
        himagic = 0x80808080L;
        lomagic = 0x01010101L;

        if(sizeof(longword) > 4) {
            // 64-bit version of the magic.
            // Do the shift in two steps to avoid a warning if long has 32 bits.
            himagic = ((himagic << 16) << 16) | himagic;
            lomagic = ((lomagic << 16) << 16) | lomagic;
        }
        if(sizeof(longword) > 8)
            abort();

        // Instead of the traditional loop which tests each character, we will test a longword at a time.
        // The tricky part is testing if *any of the four* bytes in the longword in question are zero.
        for(;;) {
            longword = *longword_ptr++;

            if (((longword - lomagic) & ~longword & himagic) != 0) {
                // Which of the bytes was the zero?  If none of them were, it was a misfire; continue the search.
                const char* cp = cast(const char* )(longword_ptr - 1);

                if (cp[0] == 0)
                    return cp - str;
                if (cp[1] == 0)
                    return cp - str + 1;
                if (cp[2] == 0)
                    return cp - str + 2;
                if (cp[3] == 0)
                    return cp - str + 3;

                if (sizeof(longword) > 4) {
                    if (cp[4] == 0)
                        return cp - str + 4;
                    if (cp[5] == 0)
                        return cp - str + 5;
                    if (cp[6] == 0)
                        return cp - str + 6;
                    if (cp[7] == 0)
                        return cp - str + 7;
                }
            }
        }
    } 
    // UTF-8
    else {
        UInt32 count = 0;
        for(; *str; count++) {
            UInt8 c = *str;
            UInt32 increment = 0;
                 if(c < 0x80)               increment = 1;
            else if((c & 0xe0) == 0xc0)     increment = 2;
            else if((c & 0xf0) == 0xe0)     increment = 3;
            else if((c & 0xf8) == 0xf0)     increment = 4;
            else return -1;
            
            str += increment;
        }
        return count;
    }
}

// Append `buff2` to the buffer data
// Returns `buffer`
void buff_append(cstlBuffer* buffer, cstlBuffer* buff2) {
    CORETEN_ENFORCE_NN(buffer, "Expected not null");
    CORETEN_ENFORCE_NN(buffer->data, "Expected not null");
    CORETEN_ENFORCE_NN(buff2, "Expected not null");
    CORETEN_ENFORCE_NN(buff2->data, "Expected not null");

    UInt64 new_len = buffer->len + buff2->len + 1;
    char* newstr = cast(char*)calloc(1, new_len);
    strcpy(newstr, buffer->data);
    strcat(newstr, buff2->data);
    buff_set(buffer, newstr);
}

// Append a character to the buffer data
void buff_append_char(cstlBuffer* buffer, char ch) {
    CORETEN_ENFORCE_NN(buffer, "Expected not null");
    CORETEN_ENFORCE_NN(buffer->data, "Expected not null");

    UInt64 len = buffer->len;
    char* newstr = cast(char*)calloc(1, len + 1);
    strcpy(newstr, buffer->data);
    newstr[len] = ch;
    newstr[len + 1] = nullchar;
    buff_set(buffer, newstr);
    buffer->len += 2;
}

// Assign `new_buff` to the buffer data
void buff_set(cstlBuffer* buffer, char* new_buff) {
    CORETEN_ENFORCE_NN(buffer, "Expected not null");

    UInt64 len;
    if(!new_buff) {
        len = 0;
        new_buff = "";
    } else {
        len = (UInt64)__internal_strlength(new_buff, buffer->is_utf8);
    }

    buffer->data = new_buff;
    buffer->len = len;
}

// Returns the buffer length
UInt64 buff_len(cstlBuffer* buffer) {
    return buffer->len;
}

// Free the buffer from its associated memory
void buff_reset(cstlBuffer* buffer) {
    if(buffer == null)
        return;

    buffer->data = null;
    buffer->len = 0;
}

// Reverse a buffer (non-destructive)
cstlBuffer* buff_rev(cstlBuffer* buffer) {
    cstlBuffer* rev = buff_new(null);
    UInt64 length = buffer->len;
    if(!length)
        return rev;
    
    char* temp = cast(char*)calloc(1, length + 1);
    for(UInt64 i=0; i<length; i++)
        *(temp + i) = *(buffer->data + length - i - 1);
    
    buff_set(rev, temp);
    return rev;
}

// Compare two buffers (case-sensitive)
// Returns true if `buff1` is lexicographically equal to `buff2`
bool buff_cmp(cstlBuffer* buff1, cstlBuffer* buff2) {
    if(buff1->len != buff2->len)
        return false;
    
    const unsigned char* s1 = cast(const unsigned char*) buff1->data;
    const unsigned char* s2 = cast(const unsigned char*) buff2->data;
    unsigned char ch1, ch2;

    do {
        ch1 = cast(unsigned char) *s1++;
        ch2 = cast(unsigned char) *s2++;
        if(ch1 == nullchar)
            return (ch1 - ch2) == 0 ? true : false;
    } while(ch1 == ch2);

    return (ch1 - ch2) == 0 ? true : false;
}

// Compare two buffers (ignoring case)
// Returns true if `buff1` is lexicographically equal to `buff2`
bool buff_cmp_nocase(cstlBuffer* buff1, cstlBuffer* buff2) {
    if(buff1->len != buff2->len)
        return false;
    
    const unsigned char* s1 = cast(const unsigned char*) buff1->data;
    const unsigned char* s2 = cast(const unsigned char*) buff2->data;
    int result;

    if(s1 == s2)
        return true;
    
    while((result = char_to_lower(*s1) - char_to_lower(*s2++)) == 0) {
        if(*s1++ == nullchar)
            break;
    }
    return result == 0 ? true : false;
}

// Get a slice of a buffer
cstlBuffer* buff_slice(cstlBuffer* buffer, int begin, int bytes) {
    CORETEN_ENFORCE_NN(buffer, "`buffer` cannot be null");
    CORETEN_ENFORCE(begin >= 0);
    CORETEN_ENFORCE(bytes >  0);

    cstlBuffer* slice = buff_new(null);
    CORETEN_ENFORCE_NN(slice, "`slice` cannot be null");
    char* temp = cast(char*)calloc(1, bytes);
    strncpy(temp, &(buffer->data[begin]), bytes);
    buff_set(slice, temp);
    CORETEN_ENFORCE_NN(slice, "`slice source` cannot be null");
    return slice;
}

// Clone a buffer
cstlBuffer* buff_clone(cstlBuffer* buffer) {
    CORETEN_ENFORCE_NN(buffer, "Cannot clone a null buffer :(");
    cstlBuffer* clone = buff_new(null);
    char* dest = cast(char*)calloc(1, buff_len(buffer));
    char* source = buffer->data;

    if(source) {
        char* str = dest;
        while(*source)
            *str++ = *source++;
    }
    buff_set(clone, dest);
    return clone;
}

// Clone a buffer (upto `n` chars)
cstlBuffer* buff_clone_n(cstlBuffer* buffer, int n) {
    CORETEN_ENFORCE_NN(buffer, "Cannot clone a null buffer :(");
    CORETEN_ENFORCE(n > 0);
    CORETEN_ENFORCE(n > buffer->len);
    cstlBuffer* clone = buff_new(null);
    char* dest = cast(char*)calloc(1, buff_len(buffer));
    char* source = buffer->data;

    if(source) {
        char* str = dest;
        while(n > 0 && *source) {
            *str++ = *source++;
            n--;
        }
    }
    buff_set(clone, dest);
    return clone;
}


// Free the buffer from its associated memory
void buff_free(cstlBuffer* buffer) {
    if(buffer)
        free(buffer);
}

// Convert a buffer to lowercase
cstlBuffer* buff_tolower(cstlBuffer* buffer) {
    cstlBuffer* lower = buff_new(null);
    if(!buffer->data) 
        return lower;

    char* temp = cast(char*)calloc(1, buffer->len);    
    strcpy(temp, buffer->data);
    int i = 0;
    while(*temp) {
        *temp = char_to_lower(*temp);
        temp++;
        i++;
    }
    // Backtrack `i` times
    temp -= i;
    buff_set(lower, temp);
    return lower;
}

// Convert a buffer to uppercase
cstlBuffer* buff_toupper(cstlBuffer* buffer) {
    cstlBuffer* upper = buff_new(null);
    if(!buffer->data) 
        return upper;

    char* temp = cast(char*)calloc(1, buffer->len);    
    strcpy(temp, buffer->data);
    int i = 0;
    while(*temp) {
        *temp = char_to_upper(*temp);
        temp++;
        i++;
    }
    // Backtrack `i` times
    temp -= i;
    buff_set(upper, temp);
    return upper;
}

// -------------------------------------------------------------------------
// char.c
// -------------------------------------------------------------------------

bool char_is_upper(char c) { 
    return c >= 'A' && c <= 'Z'; 
}

bool char_is_lower(char c) { 
    return c >= 'a' && c <= 'z'; 
}

bool char_is_digit(char c) { 
    return c >= '0' && c <= '9'; 
}

bool char_is_alpha(char c) { 
    return char_is_upper(c) || char_is_lower(c); 
}

bool char_is_alphanumeric(char c) { 
    return char_is_alpha(c) || char_is_digit(c); 
}

bool char_is_octal_digit(char c) { 
    return CORETEN_IS_BETWEEN(c, '0', '7'); 
}

bool char_is_binary_digit(char c) { 
    return c == '0' || c == '1'; 
}

Int32 digit_to_int(char c) { 
    return char_is_digit(c) ? c - '0' : c - 'W'; 
}

bool char_is_hex_digit(char c) {
    return char_is_digit(c)                   ||
           CORETEN_IS_BETWEEN(c, 'a', 'f') ||
           CORETEN_IS_BETWEEN(c, 'A', 'F'); 
}

bool char_is_letter(char c) {
    return  (c >= 'a' && c <= 'z') || 
            (c >= 'A' && c <= 'Z') || 
            (c == '_') ;
}

char char_to_lower(char c) {
    if(c >= 'A' && c <= 'Z') 
        return 'a' + (c - 'A');
    return c;
}

char char_to_upper(char c) {
    if(c >= 'a' && c <= 'z') 
        return 'A' + (c - 'a');
    return c;
}

bool char_is_whitespace(char c) {
    if(c == ' '  || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\v')
        return true; 
    return false;
}

Int32 hexdigit_to_int(char c) {
    if(char_is_digit(c))
        return digit_to_int(c);

    else if(CORETEN_IS_BETWEEN(c, 'a', 'f'))
        return c-'a' + 10; 

    else if(CORETEN_IS_BETWEEN(c, 'A', 'F'))
        return c-'A' + 10; 

    return -1; 
}

char* char_first_occurence(char* str, char ch) {
    for(; *str != ch; str++) {
        if(*str == nullchar)
            return null; // didn't find a `ch`
    }
    return str;
}

char* char_last_occurence(char* str, char ch) {
    char* result = null;
    do {
        if(*str == ch)
            result = str;
    } while(*str++);
    return result;
}

// -------------------------------------------------------------------------
// clock.c
// -------------------------------------------------------------------------

// Returns the current time (in clock_t)
double now() {
    return clock();
}

// Get duration between `start` and `end` in seconds.
double duration(clock_t start, clock_t end) {
    return (double)(end - start)/CLOCKS_PER_SEC;
}

// -------------------------------------------------------------------------
// debug.c
// -------------------------------------------------------------------------


int CORETEN_ATTRIBUTE_(format (printf, 2, 3))
cstlColouredPrintf(int colour, const char* fmt, ...) {
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

#if defined(CORETEN_OS_UNIX)
    {
        const char* str;
        switch(colour) {
            case CORETEN_COLOUR_ERROR:    str = "\033[1;31m"; break;
            case CORETEN_COLOUR_SUCCESS:  str = "\033[1;32m"; break;
            case CORETEN_COLOUR_WARN:     str = "\033[1;33m"; break;
            case CORETEN_COLOUR_CYAN:     str = "\033[1;36m"; break;
            case CORETEN_COLOUR_BOLD:     str = "\033[1m"; break;
            default:                      str = "\033[0m"; break;
        }
        printf("%s", str);
        n = printf("%s", buffer);
        printf("\033[0m"); // Reset the colour
        return n;
    }
#elif defined(CORETEN_OS_WINDOWS)
    {
        HANDLE h;
        CONSOLE_SCREEN_BUFFER_INFO info;
        WORD attr;

        h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(h, &info);

        switch(colour) {
            case CORETEN_COLOUR_ERROR:      attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case CORETEN_COLOUR_SUCCESS:    attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CORETEN_COLOUR_CYAN:       attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CORETEN_COLOUR_WARN:       attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case CORETEN_COLOUR_BOLD:       attr = FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED | 
                                                   FOREGROUND_INTENSITY; break;
            default:                        attr = 0; break;
        }
        if(attr != 0)
            SetConsoleTextAttribute(h, attr);
        n = printf("%s", buffer);
        SetConsoleTextAttribute(h, info.wAttributes);
        return n;
    }
#else
    n = printf("%s", buffer);
    return n;
#endif // CORETEN_UNIX_
}

void coreten_panic(PanicLevel pl, const char* format, ...) {
    va_list args;
    char buffer[256];

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';
    const char* str;

    switch(pl) {
        case PanicLevelUnreachable: str = "CoretenUnreachable: "; break;
        default: str = "Panic: "; break;
    }
    cstlColouredPrintf(CORETEN_COLOUR_ERROR, "%s", str);
    printf("%s\n", buffer);
    abort();
}

// -------------------------------------------------------------------------
// endian.c
// -------------------------------------------------------------------------

inline UInt16 endian_swap16(UInt16 i) {
    return (i >> 8) | (i << 8);
}

inline UInt32 endian_swap32(UInt32 i) {
    return (i >> 24) | (i << 24) | ((i & 0x00ff0000u) >> 8) | ((i & 0x0000ff00u) << 8);
}

inline UInt64 endian_swap64(UInt64 i) {
    return (i >> 56) | (i << 56) |
           ((i & 0x00ff000000000000ull) >> 40) | ((i & 0x000000000000ff00ull) << 40) |
           ((i & 0x0000ff0000000000ull) >> 24) | ((i & 0x0000000000ff0000ull) << 24) |
           ((i & 0x000000ff00000000ull) >> 8)  | ((i & 0x00000000ff000000ull) << 8);
}

// -------------------------------------------------------------------------
// hash.c
// -------------------------------------------------------------------------

#if defined(CORETEN_INCLUDE_HASH_H)

UInt32 hash_adler32(void const* data, Ll len) {
    UInt32 const MOD_ALDER = 65521;
    UInt32 a = 1, b = 0;
    Ll i, block_len;
    UInt8 const* bytes = cast(UInt8 const* )data;

    block_len = len % 5552;

    while(len) {
        for(i = 0; i+7 < block_len; i += 8) {
            a += bytes[0], b += a;
            a += bytes[1], b += a;
            a += bytes[2], b += a;
            a += bytes[3], b += a;
            a += bytes[4], b += a;
            a += bytes[5], b += a;
            a += bytes[6], b += a;
            a += bytes[7], b += a;

            bytes += 8;
        }
        for(; i < block_len; i++)
            a += *bytes++, b += a;

        a %= MOD_ALDER, b %= MOD_ALDER;
        len -= block_len;
        block_len = 5552;
    }

    return (b << 16) | a;
}

static UInt32 const CORETEN__CRC32_TABLE[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
    0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
    0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
    0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
    0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
    0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
    0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
    0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
    0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
    0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
    0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
    0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
    0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
    0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
    0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
    0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
    0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
    0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
    0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
    0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
    0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
    0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
};

static UInt64 const CORETEN__CRC64_TABLE[256] = {
    0x0000000000000000ull, 0x42f0e1eba9ea3693ull, 0x85e1c3d753d46d26ull, 0xc711223cfa3e5bb5ull,
    0x493366450e42ecdfull, 0x0bc387aea7a8da4cull, 0xccd2a5925d9681f9ull, 0x8e224479f47cb76aull,
    0x9266cc8a1c85d9beull, 0xd0962d61b56fef2dull, 0x17870f5d4f51b498ull, 0x5577eeb6e6bb820bull,
    0xdb55aacf12c73561ull, 0x99a54b24bb2d03f2ull, 0x5eb4691841135847ull, 0x1c4488f3e8f96ed4ull,
    0x663d78ff90e185efull, 0x24cd9914390bb37cull, 0xe3dcbb28c335e8c9ull, 0xa12c5ac36adfde5aull,
    0x2f0e1eba9ea36930ull, 0x6dfeff5137495fa3ull, 0xaaefdd6dcd770416ull, 0xe81f3c86649d3285ull,
    0xf45bb4758c645c51ull, 0xb6ab559e258e6ac2ull, 0x71ba77a2dfb03177ull, 0x334a9649765a07e4ull,
    0xbd68d2308226b08eull, 0xff9833db2bcc861dull, 0x388911e7d1f2dda8ull, 0x7a79f00c7818eb3bull,
    0xcc7af1ff21c30bdeull, 0x8e8a101488293d4dull, 0x499b3228721766f8ull, 0x0b6bd3c3dbfd506bull,
    0x854997ba2f81e701ull, 0xc7b97651866bd192ull, 0x00a8546d7c558a27ull, 0x4258b586d5bfbcb4ull,
    0x5e1c3d753d46d260ull, 0x1cecdc9e94ace4f3ull, 0xdbfdfea26e92bf46ull, 0x990d1f49c77889d5ull,
    0x172f5b3033043ebfull, 0x55dfbadb9aee082cull, 0x92ce98e760d05399ull, 0xd03e790cc93a650aull,
    0xaa478900b1228e31ull, 0xe8b768eb18c8b8a2ull, 0x2fa64ad7e2f6e317ull, 0x6d56ab3c4b1cd584ull,
    0xe374ef45bf6062eeull, 0xa1840eae168a547dull, 0x66952c92ecb40fc8ull, 0x2465cd79455e395bull,
    0x3821458aada7578full, 0x7ad1a461044d611cull, 0xbdc0865dfe733aa9ull, 0xff3067b657990c3aull,
    0x711223cfa3e5bb50ull, 0x33e2c2240a0f8dc3ull, 0xf4f3e018f031d676ull, 0xb60301f359dbe0e5ull,
    0xda050215ea6c212full, 0x98f5e3fe438617bcull, 0x5fe4c1c2b9b84c09ull, 0x1d14202910527a9aull,
    0x93366450e42ecdf0ull, 0xd1c685bb4dc4fb63ull, 0x16d7a787b7faa0d6ull, 0x5427466c1e109645ull,
    0x4863ce9ff6e9f891ull, 0x0a932f745f03ce02ull, 0xcd820d48a53d95b7ull, 0x8f72eca30cd7a324ull,
    0x0150a8daf8ab144eull, 0x43a04931514122ddull, 0x84b16b0dab7f7968ull, 0xc6418ae602954ffbull,
    0xbc387aea7a8da4c0ull, 0xfec89b01d3679253ull, 0x39d9b93d2959c9e6ull, 0x7b2958d680b3ff75ull,
    0xf50b1caf74cf481full, 0xb7fbfd44dd257e8cull, 0x70eadf78271b2539ull, 0x321a3e938ef113aaull,
    0x2e5eb66066087d7eull, 0x6cae578bcfe24bedull, 0xabbf75b735dc1058ull, 0xe94f945c9c3626cbull,
    0x676dd025684a91a1ull, 0x259d31cec1a0a732ull, 0xe28c13f23b9efc87ull, 0xa07cf2199274ca14ull,
    0x167ff3eacbaf2af1ull, 0x548f120162451c62ull, 0x939e303d987b47d7ull, 0xd16ed1d631917144ull,
    0x5f4c95afc5edc62eull, 0x1dbc74446c07f0bdull, 0xdaad56789639ab08ull, 0x985db7933fd39d9bull,
    0x84193f60d72af34full, 0xc6e9de8b7ec0c5dcull, 0x01f8fcb784fe9e69ull, 0x43081d5c2d14a8faull,
    0xcd2a5925d9681f90ull, 0x8fdab8ce70822903ull, 0x48cb9af28abc72b6ull, 0x0a3b7b1923564425ull,
    0x70428b155b4eaf1eull, 0x32b26afef2a4998dull, 0xf5a348c2089ac238ull, 0xb753a929a170f4abull,
    0x3971ed50550c43c1ull, 0x7b810cbbfce67552ull, 0xbc902e8706d82ee7ull, 0xfe60cf6caf321874ull,
    0xe224479f47cb76a0ull, 0xa0d4a674ee214033ull, 0x67c58448141f1b86ull, 0x253565a3bdf52d15ull,
    0xab1721da49899a7full, 0xe9e7c031e063acecull, 0x2ef6e20d1a5df759ull, 0x6c0603e6b3b7c1caull,
    0xf6fae5c07d3274cdull, 0xb40a042bd4d8425eull, 0x731b26172ee619ebull, 0x31ebc7fc870c2f78ull,
    0xbfc9838573709812ull, 0xfd39626eda9aae81ull, 0x3a28405220a4f534ull, 0x78d8a1b9894ec3a7ull,
    0x649c294a61b7ad73ull, 0x266cc8a1c85d9be0ull, 0xe17dea9d3263c055ull, 0xa38d0b769b89f6c6ull,
    0x2daf4f0f6ff541acull, 0x6f5faee4c61f773full, 0xa84e8cd83c212c8aull, 0xeabe6d3395cb1a19ull,
    0x90c79d3fedd3f122ull, 0xd2377cd44439c7b1ull, 0x15265ee8be079c04ull, 0x57d6bf0317edaa97ull,
    0xd9f4fb7ae3911dfdull, 0x9b041a914a7b2b6eull, 0x5c1538adb04570dbull, 0x1ee5d94619af4648ull,
    0x02a151b5f156289cull, 0x4051b05e58bc1e0full, 0x87409262a28245baull, 0xc5b073890b687329ull,
    0x4b9237f0ff14c443ull, 0x0962d61b56fef2d0ull, 0xce73f427acc0a965ull, 0x8c8315cc052a9ff6ull,
    0x3a80143f5cf17f13ull, 0x7870f5d4f51b4980ull, 0xbf61d7e80f251235ull, 0xfd913603a6cf24a6ull,
    0x73b3727a52b393ccull, 0x31439391fb59a55full, 0xf652b1ad0167feeaull, 0xb4a25046a88dc879ull,
    0xa8e6d8b54074a6adull, 0xea16395ee99e903eull, 0x2d071b6213a0cb8bull, 0x6ff7fa89ba4afd18ull,
    0xe1d5bef04e364a72ull, 0xa3255f1be7dc7ce1ull, 0x64347d271de22754ull, 0x26c49cccb40811c7ull,
    0x5cbd6cc0cc10fafcull, 0x1e4d8d2b65facc6full, 0xd95caf179fc497daull, 0x9bac4efc362ea149ull,
    0x158e0a85c2521623ull, 0x577eeb6e6bb820b0ull, 0x906fc95291867b05ull, 0xd29f28b9386c4d96ull,
    0xcedba04ad0952342ull, 0x8c2b41a1797f15d1ull, 0x4b3a639d83414e64ull, 0x09ca82762aab78f7ull,
    0x87e8c60fded7cf9dull, 0xc51827e4773df90eull, 0x020905d88d03a2bbull, 0x40f9e43324e99428ull,
    0x2cffe7d5975e55e2ull, 0x6e0f063e3eb46371ull, 0xa91e2402c48a38c4ull, 0xebeec5e96d600e57ull,
    0x65cc8190991cb93dull, 0x273c607b30f68faeull, 0xe02d4247cac8d41bull, 0xa2dda3ac6322e288ull,
    0xbe992b5f8bdb8c5cull, 0xfc69cab42231bacfull, 0x3b78e888d80fe17aull, 0x7988096371e5d7e9ull,
    0xf7aa4d1a85996083ull, 0xb55aacf12c735610ull, 0x724b8ecdd64d0da5ull, 0x30bb6f267fa73b36ull,
    0x4ac29f2a07bfd00dull, 0x08327ec1ae55e69eull, 0xcf235cfd546bbd2bull, 0x8dd3bd16fd818bb8ull,
    0x03f1f96f09fd3cd2ull, 0x41011884a0170a41ull, 0x86103ab85a2951f4ull, 0xc4e0db53f3c36767ull,
    0xd8a453a01b3a09b3ull, 0x9a54b24bb2d03f20ull, 0x5d45907748ee6495ull, 0x1fb5719ce1045206ull,
    0x919735e51578e56cull, 0xd367d40ebc92d3ffull, 0x1476f63246ac884aull, 0x568617d9ef46bed9ull,
    0xe085162ab69d5e3cull, 0xa275f7c11f7768afull, 0x6564d5fde549331aull, 0x279434164ca30589ull,
    0xa9b6706fb8dfb2e3ull, 0xeb46918411358470ull, 0x2c57b3b8eb0bdfc5ull, 0x6ea7525342e1e956ull,
    0x72e3daa0aa188782ull, 0x30133b4b03f2b111ull, 0xf7021977f9cceaa4ull, 0xb5f2f89c5026dc37ull,
    0x3bd0bce5a45a6b5dull, 0x79205d0e0db05dceull, 0xbe317f32f78e067bull, 0xfcc19ed95e6430e8ull,
    0x86b86ed5267cdbd3ull, 0xc4488f3e8f96ed40ull, 0x0359ad0275a8b6f5ull, 0x41a94ce9dc428066ull,
    0xcf8b0890283e370cull, 0x8d7be97b81d4019full, 0x4a6acb477bea5a2aull, 0x089a2aacd2006cb9ull,
    0x14dea25f3af9026dull, 0x562e43b4931334feull, 0x913f6188692d6f4bull, 0xd3cf8063c0c759d8ull,
    0x5dedc41a34bbeeb2ull, 0x1f1d25f19d51d821ull, 0xd80c07cd676f8394ull, 0x9afce626ce85b507ull,
};

UInt32 hash_crc32(void const* data, Ll len) {
    Ll remaining;
    UInt32 result = ~(cast(UInt32)0);
    UInt8 const* c = cast(UInt8 const* )data;
    for(remaining = len; remaining--; c++)
        result = (result >> 8) ^ (CORETEN__CRC32_TABLE[(result ^ *c) & 0xff]);
    return ~result;
}

UInt64 hash_crc64(void const* data, Ll len) {
    Ll remaining;
    UInt64 result = ~(cast(UInt64)0);
    UInt8 const* c = cast(UInt8 const* )data;
    for(remaining = len; remaining--; c++)
        result = (result >> 8) ^ (CORETEN__CRC64_TABLE[(result ^ *c) & 0xff]);
    return ~result;
}

UInt32 hash_fnv32(void const* data, Ll len) {
    Ll i;
    UInt32 h = 0x811c9dc5;
    UInt8 const* c = cast(UInt8 const* )data;

    for(i = 0; i < len; i++)
        h = (h * 0x01000193) ^ c[i];

    return h;
}

UInt64 hash_fnv64(void const* data, Ll len) {
    Ll i;
    UInt64 h = 0xcbf29ce484222325ull;
    UInt8 const* c = cast(UInt8 const* )data;

    for(i = 0; i < len; i++)
        h = (h * 0x100000001b3ll) ^ c[i];

    return h;
}

UInt32 hash_fnv32a(void const* data, Ll len) {
    Ll i;
    UInt32 h = 0x811c9dc5;
    UInt8 const* c = cast(UInt8 const* )data;

    for(i = 0; i < len; i++)
        h = (h ^ c[i]) * 0x01000193;

    return h;
}

UInt64 hash_fnv64a(void const* data, Ll len) {
    Ll i;
    UInt64 h = 0xcbf29ce484222325ull;
    UInt8 const* c = cast(UInt8 const* )data;

    for(i = 0; i < len; i++)
        h = (h ^ c[i]) * 0x100000001b3ll;

    return h;
}

UInt32 hash_murmur32(void const* data, Ll len) { 
    return hash_murmur32_seed(data, len, 0x9747b28c); 
}

UInt64 hash_murmur64(void const* data, Ll len) { 
    return hash_murmur64_seed(data, len, 0x9747b28c); 
}

UInt32 hash_murmur32_seed(void const* data, Ll len, UInt32 seed) {
    UInt32 const c1 = 0xcc9e2d51;
    UInt32 const c2 = 0x1b873593;
    UInt32 const r1 = 15;
    UInt32 const r2 = 13;
    UInt32 const m  = 5;
    UInt32 const n  = 0xe6546b64;

    Ll i, nblocks = len / 4;
    UInt32 hash = seed, k1 = 0;
    UInt32 const* blocks = cast(UInt32 const*)data;
    UInt8 const* tail = cast(UInt8 const* )(data) + nblocks*4;

    for(i = 0; i < nblocks; i++) {
        UInt32 k = blocks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        hash ^= k;
        hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
    }

    // Fallthrough intended
    CORETEN_GCC_SUPPRESS_WARNING_PUSH
    CORETEN_CLANG_SUPPRESS_WARNING_PUSH
    CORETEN_MSVC_SUPPRESS_WARNING_PUSH
    CORETEN_GCC_SUPPRESS_WARNING("-Wimplicit-fallthrough")
    CORETEN_CLANG_SUPPRESS_WARNING("-Wimplicit-fallthrough")
    CORETEN_MSVC_SUPPRESS_WARNING(26819)
    switch (len & 3) {
        // fall through
        case 3:
            k1 ^= tail[2] << 16;
        case 2:
            k1 ^= tail[1] << 8;
        case 1:
            k1 ^= tail[0];

            k1 *= c1;
            k1 = (k1 << r1) | (k1 >> (32 - r1));
            k1 *= c2;
            hash ^= k1;
    }
    CORETEN_GCC_SUPPRESS_WARNING_POP
    CORETEN_CLANG_SUPPRESS_WARNING_POP
    CORETEN_MSVC_SUPPRESS_WARNING_POP

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}

UInt64 hash_murmur64_seed(void const* data__, Ll len, UInt64 seed) {
#if defined(CORETEN_ARCH_64BIT)
    UInt64 const m = 0xc6a4a7935bd1e995ULL;
    Int32 const r = 47;

    UInt64 h = seed ^ (len * m);

    UInt64 const* data = cast(UInt64 const* )data__;
    UInt8  const* data2 = cast(UInt8 const* )data__;
    UInt64 const* end = data + (len / 8);

    while(data != end) {
        UInt64 k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    // Fallthrough intended
    CORETEN_GCC_SUPPRESS_WARNING_PUSH
    CORETEN_CLANG_SUPPRESS_WARNING_PUSH
    CORETEN_MSVC_SUPPRESS_WARNING_PUSH
    CORETEN_GCC_SUPPRESS_WARNING("-Wimplicit-fallthrough")
    CORETEN_CLANG_SUPPRESS_WARNING("-Wimplicit-fallthrough")
    CORETEN_MSVC_SUPPRESS_WARNING(26819)
    switch (len & 7) {
        // fall through
        case 7: h ^= cast(UInt64)(data2[6]) << 48;
        case 6: h ^= cast(UInt64)(data2[5]) << 40;
        case 5: h ^= cast(UInt64)(data2[4]) << 32;
        case 4: h ^= cast(UInt64)(data2[3]) << 24;
        case 3: h ^= cast(UInt64)(data2[2]) << 16;
        case 2: h ^= cast(UInt64)(data2[1]) << 8;
        case 1: h ^= cast(UInt64)(data2[0]);
            h *= m;
    };
    CORETEN_GCC_SUPPRESS_WARNING_POP
    CORETEN_CLANG_SUPPRESS_WARNING_POP
    CORETEN_MSVC_SUPPRESS_WARNING_POP

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
#else
    UInt64 h;
    UInt32 const m = 0x5bd1e995;
    Int32 const r = 24;

    UInt32 h1 = cast(UInt32)(seed) ^ cast(UInt32)(len);
    UInt32 h2 = cast(UInt32)(seed >> 32);

    UInt32 const* data = cast(UInt32 const* )data__;

    while(len >= 8) {
        UInt32 k1, k2;
        k1 = *data++;
        k1 *= m;
        k1 ^= k1 >> r;
        k1 *= m;
        h1 *= m;
        h1 ^= k1;
        len -= 4;

        k2 = *data++;
        k2 *= m;
        k2 ^= k2 >> r;
        k2 *= m;
        h2 *= m;
        h2 ^= k2;
        len -= 4;
    }

    if (len >= 4) {
        UInt32 k1 = *data++;
        k1 *= m;
        k1 ^= k1 >> r;
        k1 *= m;
        h1 *= m;
        h1 ^= k1;
        len -= 4;
    }

    switch (len) {
        case 3: h2 ^= (cast(UInt8 const* )data)[2] << 16;
        case 2: h2 ^= (cast(UInt8 const* )data)[1] <<  8;
        case 1: h2 ^= (cast(UInt8 const* )data)[0] <<  0;
            h2 *= m;
    };

    h1 ^= h2 >> 18;
    h1 *= m;
    h2 ^= h1 >> 22;
    h2 *= m;
    h1 ^= h2 >> 17;
    h1 *= m;
    h2 ^= h1 >> 19;
    h2 *= m;

    h = h1;
    h = (h << 32) | h2;

    return h;
#endif // CORETEN_ARCH_64BIT
}

#endif // CORETEN_INCLUDE_HASH_H

// -------------------------------------------------------------------------
// io.h
// -------------------------------------------------------------------------

char* readFile(const char* fname) {
    FILE* file = fopen(fname, "rb"); 
    
    if(!file) {
        cstlColouredPrintf(CORETEN_COLOUR_ERROR, "Could not open file: <%s>\n", fname);
        cstlColouredPrintf(CORETEN_COLOUR_ERROR, "%s\n", !file_exists(fname) ?  
                            "FileNotFoundError: File does not exist." : "");
        exit(1);
    }

    // Get the length of the input buffer
    fseek(file, 0, SEEK_END); 
    long buff_length = ftell(file); 
    fseek(file, 0, SEEK_SET);

    char* buffer = cast(char*)malloc(sizeof(char) * (buff_length + 1) );
    if(!buffer) {
        fprintf(stderr, "Could not allocate memory for buffer for file at %s\n", fname);
        exit(1);
    }

    fread(buffer, 1, buff_length, file); 
    buffer[buff_length] = nullchar;
    fclose(file); 

    return buffer;
}

bool file_exists(const char* path) {
#ifdef WIN32
    if (GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES) return true;
#else
    // if(access(path, 0) == 0) return true;
    struct stat st;
    return !stat(path, &st);
#endif // WIN32
    return false;
}

// -------------------------------------------------------------------------
// math.h
// -------------------------------------------------------------------------

Float32 coreten_toRadians(Float32 degrees) { 
    return degrees * CORETEN_MATH_TAU / 360.0f;
}

Float32 coreten_toDegrees(Float32 radians) { 
    return radians * 360.0f / CORETEN_MATH_TAU;
}

Float32 coreten_sin(Float32 x) {
    Float32 x0 = +1.91059300966915117e-31f;
    Float32 x1 = +1.00086760103908896f;
    Float32 x2 = -1.21276126894734565e-2f;
    Float32 x3 = -1.38078780785773762e-1f;
    Float32 x4 = -2.67353392911981221e-2f;
    Float32 x5 = +2.08026600266304389e-2f;
    Float32 x6 = -3.03996055049204407e-3f;
    Float32 x7 = +1.38235642404333740e-4f;
    return x0 + x*(x1 + x*(x2 + x*(x3 + x*(x4 + x*(x5 + x*(x6 + x*x7))))));
}

Float32 coreten_cos(Float32 x) {
    Float32 x0 = +1.00238601909309722f;
    Float32 x1 = -3.81919947353040024e-2f;
    Float32 x2 = -3.94382342128062756e-1f;
    Float32 x3 = -1.18134036025221444e-1f;
    Float32 x4 = +1.07123798512170878e-1f;
    Float32 x5 = -1.86637164165180873e-2f;
    Float32 x6 = +9.90140908664079833e-4f;
    Float32 x7 = -5.23022132118824778e-14f;
    return x0 + x*(x1 + x*(x2 + x*(x3 + x*(x4 + x*(x5 + x*(x6 + x*x7))))));
}

Float32 coreten_tan(Float32 radians) {
    Float32 rr = radians * radians;
    Float32 x = 9.5168091e-03f;
    x *= rr;
    x += 2.900525e-03f;
    x *= rr;
    x += 2.45650893e-02f;
    x *= rr;
    x += 5.33740603e-02f;
    x *= rr;
    x += 1.333923995e-01f;
    x *= rr;
    x += 3.333314036e-01f;
    x *= rr;
    x += 1.0f;
    x *= radians;
    return x;
}

Float32 coreten_arctan(Float32 x) {
    Float32 u  = x*x;
    Float32 u2 = u*u;
    Float32 u3 = u2*u;
    Float32 u4 = u3*u;
    Float32 f  = 1.0f+0.33288950512027f*u-0.08467922817644f*u2+0.03252232640125f*u3-0.00749305860992f*u4;
    return x/f;
}

Float32 coreten_arctan2(Float32 x, Float32 y) {
    if (CORETEN_ABS(y) > CORETEN_ABS(x)) {
        Float32 a = coreten_arctan(x/y);
        if (y > 0.0f)
            return a;
        else
            return x > 0.0f ? a + CORETEN_MATH_TAU_BY_2 : a - CORETEN_MATH_TAU_BY_2;
    } else {
        Float32 a = coreten_arctan(y/x);
        if (y > 0.0f)
            return x > 0.0f ? CORETEN_MATH_TAU_BY_4 - a : -CORETEN_MATH_TAU_BY_4 - a;
        else
            return x > 0.0f ? CORETEN_MATH_TAU_BY_4 + a : -CORETEN_MATH_TAU_BY_4 + a;
    }
}

Float32 coreten_exp(Float32 x) {
    union { Float32 f; int i; } u, v;
    u.i = (int)(6051102 * x + 1056478197);
    v.i = (int)(1056478197 - 6051102 * x);
    return u.f / v.f;
}

Float32 coreten_log(Float32 x) {
    union { Float32 f; int i; } u = {x};
    return (u.i - 1064866805) * 8.262958405176314e-8f; // 1 / 12102203.0;
}

Float32 coreten_pow(Float32 x, Float32 y) {
    int flipped = 0, e;
    Float32 f, r = 1.0f;
    if (y < 0) {
        flipped = 1;
        y = -y;
    }
    e = (int)y;
    f = coreten_exp(y - e);
    while (e) {
        if (e & 1) r *= x;
        x *= x;
        e >>= 1;
    }
    r *= f;
    return flipped ? 1.0f/r : r;
}

Float32 coreten_square(Float32 x){
    return coreten_exp(CORETEN_MATH_LOG_TWO * x);
}

Float32 coreten_log2(Float32 x){
    return coreten_log(x) / CORETEN_MATH_LOG_TWO;
}

// -------------------------------------------------------------------------
// os.h
// -------------------------------------------------------------------------

cstlBuffer* os_get_cwd() {
#if defined(CORETEN_OS_WINDOWS)
    // This (or its equivalent) is not defined in any include in Windows as far as I've come across
    #define PATH_MAX 4096
    char result[PATH_MAX];
    _getcwd(result, PATH_MAX);
    if(!*result) {
        fprintf(stderr, "Unable to `os_get_cwd()`. 'getcwd()' failed");
        exit(1);
    }
    cstlBuffer* buff = buff_new(result);
    return buff;
#elif defined(CORETEN_OS_POSIX)
    long n;
    char *buf;

    n = pathconf(".", _PC_PATH_MAX);
    CORETEN_ENFORCE(n != -1);
    buf = cast(char*)calloc(n, sizeof(*buf));
    CORETEN_ENFORCE_NN(buf, "calloc failed. Out of memory");
    char* result = getcwd(buf, n);
    if(!result) {
        fprintf(stderr, "Unable to `os_get_cwd()`. 'getcwd()' failed");
        exit(1);
    }
    cstlBuffer* buff = buff_new(buf);
    return buff;
#else
    #error "No `os_get_cwd()` implementation supported for your platform."
    return null;
#endif // CORETEN_OS_WINDOWS
}

cstlBuffer* __os_dirname_basename(cstlBuffer* path, bool is_basename) {
    UInt64 length = path->len;
    if(!length)
        return path;

    cstlBuffer* result = buff_new(null);
    char* end = buff_end(path);

    // dirname
    if(!is_basename) {
        cstlBuffer* rev = buff_rev(path);

        // The `/` or `\\` is not so important in getting the dirname, but it does interfere with `strchr`, so
        // we skip over it (if present)
        if(*rev->data == CORETEN_OS_SEP_CHAR)
            rev->data++;
        char* rev_dir = strchr(rev->data, CORETEN_OS_SEP_CHAR);
        buff_set(result, rev_dir);
        result = buff_rev(result);
        buff_free(rev);
    } 

    // basename
    else {
        // If the last character is a `sep`, `basename` is empty
        if(os_is_sep(*end))
            return buff_new(null);
        
        // If there is no `sep` in `path`, `path` is the basename
        if(!(strstr(path->data, "/") || strstr(path->data, "\\")))
            return path;
        
        cstlBuffer* rev = buff_rev(path);
        for(UInt64 i = 0; i<length; i++) {
            if(os_is_sep(*(rev->data + i))) {
                *(rev->data + i) = nullchar;
                break;
            }
        }
        buff_set(result, rev->data);
        result = buff_rev(result);
        buff_free(rev);
    }
    
    return result;
}

cstlBuffer* os_path_dirname(cstlBuffer* path) {
    return __os_dirname_basename(path, false);
}

cstlBuffer* os_path_basename(cstlBuffer* path) {
    return __os_dirname_basename(path, true);
}

cstlBuffer* os_path_extname(cstlBuffer* path) {
    cstlBuffer* basename = os_path_basename(path);
    if(!strcmp(basename->data, ""))
        return basename;
    
    char* ext = strchr(basename->data, '.');
    if(!ext) {
       free(basename);
       return buff_new(null);
    }

    buff_set(basename, ext);
    return basename;
}

cstlBuffer* os_path_join(cstlBuffer* path1, cstlBuffer* path2) {
    UInt64 length = path1->len;
    if(!length)
        return path1;
        
    char* end = buff_end(path1);
    if(!os_is_sep(*end))
        buff_append_char(path1, CORETEN_OS_SEP_CHAR);
    buff_append(path1, path2);
    return path1;
}

bool os_is_sep(char ch) {
#ifdef CORETEN_OS_WINDOWS
    return ch == '\\' || ch == '/';
#else
    return ch == '/';
#endif // CORETEN_OS_WINDOWS
}

bool os_path_is_abs(cstlBuffer* path) {
    bool result = false;
    CORETEN_ENFORCE_NN(path, "Cannot do anything useful on a null path :(");
#ifdef CORETEN_OS_WINDOWS
    // The 'C:\...`
    result = path->len > 2 &&
             char_is_alpha(path->data[0]) &&
             (path->data[1] == ':' && path->data[2] == CORETEN_OS_SEP_CHAR);
#else
    result = path->len > 2 &&
             path->data[0] == CORETEN_OS_SEP_CHAR;
#endif // CORETEN_OS_WINDOWS
    return cast(bool)result;
}

bool os_path_is_rel(cstlBuffer* path) {
    return cast(bool) !os_path_is_abs(path);
}

bool os_path_is_root(cstlBuffer* path) {
    bool result = false;
    CORETEN_ENFORCE_NN(path, "Cannot do anything useful on a null path :(");
#ifdef CORETEN_OS_WINDOWS
    result = os_path_is_abs(path) && path->len == 3;
#else
    result = os_path_is_abs(path) && path->len == 1;
#endif // CORETEN_OS_WINDOWS
    return result;
}

// -------------------------------------------------------------------------
// utf8.c
// -------------------------------------------------------------------------


/*
    The following macros define the last codepoint part of a UTF-8 category
*/
// Basic Latin (U+0000 - U+007F)
#define UTF8_MAX_BASIC_LATIN	0x007F
// Latin-1 Supplement (U+0080 - U+00FF)
#define UTF8_MAX_LATIN_1		0x00FF
// Basic Multilingual Plane (BMP)
#define UTF8_MAX_BMP			0xFFFF
// Last legal character in Unicode
#define UTF8_MAX_UNICODE		0x10FFFF

/*
    Replacement character
*/
// The codepoint used to replace illegal codepoints
#define UTF8_REPLACEMENT_CHAR	0xFFFD
// Replacement character as a UTF8-encoded string
#define UTF8_REPLACEMENT_STRING	"\xEF\xBF\xBD"
// Length of the replacement character
#define UTF8_REPLACEMENT_STRLEN	3

/*
    Surrogates
*/
// The minimum codepoint for the high member of the surrogate pair
#define UTF8_SURROGATE_HIGH_START	0xD800
// The maximum codepoint for the high member of the surrogate pair
#define UTF8_SURROGATE_HIGH_END		0xDBFF
// The minimum codepoint for the low member of the surrogate pair
#define UTF8_SURROGATE_LOW_START	0xDC00
// The maximum codepoint for the low member of the surrogate pair
#define UTF8_SURROGATE_LOW_END		0xDFFF

/*
    Hangul Jamo
*/
// The first & last codepoint part of the HJ block
#define UTF8_HANGUL_JAMO_FIRST      0x1100
#define UTF8_HANGUL_JAMO_LAST       0x11FF
// The first & last codepoint part of the HJ L section used for normalization
#define UTF8_HANGUL_L_FIRST         0x1100
#define UTF8_HANGUL_L_LAST          0x1112
// The first & last codepoint part of the HJ V section used for normalization
#define UTF8_HANGUL_V_FIRST         0x1161
#define UTF8_HANGUL_V_LAST          0x1175
// The first & last codepoint part of the HJ T section used for normalization
#define UTF8_HANGUL_T_FIRST         0x11A7
#define UTF8_HANGUL_T_LAST          0x11C2
// The first & last codepoint in the Hangul Syllables block
#define UTF8_HANGUL_S_FIRST         0xAC00
#define UTF8_HANGUL_S_LAST          0xD7A3

// The number of codepoints in the HJ ... sections (where `...` is either L,V,T,N,S) 
#define UTF8_HANGUL_LCOUNT         19
#define UTF8_HANGUL_VCOUNT         21
#define UTF8_HANGUL_TCOUNT         28
#define UTF8_HANGUL_NCOUNT         588
#define UTF8_HANGUL_SCOUNT         11172


#define UTF8_CP_LATIN_CAPITAL_LETTER_I                 0x0049
#define UTF8_CP_LATIN_CAPITAL_LETTER_J                 0x004A
#define UTF8_CP_LATIN_SMALL_LETTER_I                   0x0069
#define UTF8_CP_LATIN_SMALL_LETTER_J                   0x006A
#define UTF8_CP_LATIN_CAPITAL_LETTER_I_WITH_GRAVE      0x00CC
#define UTF8_CP_LATIN_CAPITAL_LETTER_I_WITH_ACUTE      0x00CD
#define UTF8_CP_LATIN_CAPITAL_LETTER_I_WITH_TILDE      0x0128
#define UTF8_CP_LATIN_CAPITAL_LETTER_I_WITH_OGONEK     0x012E
#define UTF8_CP_LATIN_SMALL_LETTER_I_WITH_OGONEK       0x012F
#define UTF8_CP_LATIN_CAPITAL_LETTER_I_WITH_DOT_ABOVE  0x0130
#define UTF8_CP_LATIN_SMALL_LETTER_DOTLESS_I           0x0131
#define UTF8_CP_COMBINING_GRAVE_ACCENT                 0x0300
#define UTF8_CP_COMBINING_ACUTE_ACCENT                 0x0301
#define UTF8_CP_COMBINING_TILDE_ACCENT                 0x0303
#define UTF8_CP_COMBINING_DOT_ABOVE                    0x0307
#define UTF8_CP_COMBINING_GREEK_YPOGEGRAMMENI          0x0345
#define UTF8_CP_COMBINING_GRAPHEME_JOINER              0x034F
#define UTF8_CP_GREEK_CAPITAL_LETTER_SIGMA             0x03A3

#ifndef UTF8_UINT16_MAX
    #define UTF8_UINT16_MAX     65535U
#endif // UTF8_UINT16_MAX

#include <adorad/core/utf8_data.h>
// #include <adorad/core/utf8_properties.h>

const Rune codepoint_decoded_length[256] = {
    // Basic Latin
    1, 1, 1, 1, 1, 1, 1, 1, // 0x00 - 0x07
    1, 1, 1, 1, 1, 1, 1, 1, // 0x08 - 0x0F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x10 - 0x17
    1, 1, 1, 1, 1, 1, 1, 1, // 0x18 - 0x1F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x20 - 0x27
    1, 1, 1, 1, 1, 1, 1, 1, // 0x28 - 0x2F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x30 - 0x37
    1, 1, 1, 1, 1, 1, 1, 1, // 0x38 - 0x3F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x40 - 0x47
    1, 1, 1, 1, 1, 1, 1, 1, // 0x48 - 0x4F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x50 - 0x57
    1, 1, 1, 1, 1, 1, 1, 1, // 0x58 - 0x5F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x60 - 0x67
    1, 1, 1, 1, 1, 1, 1, 1, // 0x68 - 0x6F
    1, 1, 1, 1, 1, 1, 1, 1, // 0x70 - 0x77
    1, 1, 1, 1, 1, 1, 1, 1, // 0x78 - 0x7F

    // Malformed continuation byte
    0, 0, 0, 0, 0, 0, 0, 0, // 0x80 - 0x87
    0, 0, 0, 0, 0, 0, 0, 0, // 0x88 - 0x8F
    0, 0, 0, 0, 0, 0, 0, 0, // 0x90 - 0x97
    0, 0, 0, 0, 0, 0, 0, 0, // 0x98 - 0x9F
    0, 0, 0, 0, 0, 0, 0, 0, // 0xA0 - 0xA7
    0, 0, 0, 0, 0, 0, 0, 0, // 0xA8 - 0xAF
    0, 0, 0, 0, 0, 0, 0, 0, // 0xB0 - 0xB7
    0, 0, 0, 0, 0, 0, 0, 0, // 0xB8 - 0xBF

    // Two bytes
    2, 2, 2, 2, 2, 2, 2, 2, // 0xC0 - 0xC7
    2, 2, 2, 2, 2, 2, 2, 2, // 0xC8 - 0xCF
    2, 2, 2, 2, 2, 2, 2, 2, // 0xD0 - 0xD7
    2, 2, 2, 2, 2, 2, 2, 2, // 0xD8 - 0xDF

    // Three bytes
    3, 3, 3, 3, 3, 3, 3, 3, // 0xE0 - 0xE7
    3, 3, 3, 3, 3, 3, 3, 3, // 0xE8 - 0xEF

    // Four bytes
    4, 4, 4, 4, 4, 4, 4, 4, // 0xF0 - 0xF7

    // Five bytes
    5, 5, 5, 5,             // 0xF8 - 0xFB

    // Six bytes
    6, 6,                   // 0xFC - 0xFD

    // Invalid
    7, 7                    // 0xFE - 0xFF
};

const Byte utf8class[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0 
};

static inline bool utf8_is_codepoint_valid(Rune uc) {
    // if(uc < 0 || uc >= 0x110000 || ((uc & 0xFFFF) >= 0xFFFE) || (uc >= 0xD800 && uc < 0xE000) || 
    //   (uc >= 0xFDD0 && uc < 0xFDF0))
    //     return false;
    // return true;
    return ((cast(Rune)uc) - 0xd800 > 0x07ff) && (cast(Rune)uc < 0x110000);
}

// Determine the number of bytes needed to store the UTF-8 character
// This can be one in <1, 2, 3, 4>
// Theoretically, this number can extend to 6, and even 7, bytes, but this is rare
static inline Ll utf8_encode_nbytes(Rune value) {
    Ll nbytes = 0;
    CORETEN_ENFORCE(value > 0, "Cannot encode a negative value :(");

    if(value <= 0x7f) nbytes = 1;     // 127
    if(value <= 0x7ff) nbytes = 2;    // 2047
    if(value <= 0xffff) nbytes = 3;   // 65535
    if(value <= 0x10ffff) nbytes = 4; // 1114111
    else nbytes = 0;
    CORETEN_ENFORCE(nbytes > 0, "Invalid code point");
    return nbytes;
}

// Determine the number of bytes used by the UTF-8 character
// This can be one in <1, 2, 3, 4>
// Theoretically, this number can extend to 6, and even 7, bytes, but this is rare
static inline Ll utf8_decode_nbytes(Rune byte) {
    CORETEN_ENFORCE(byte > 0, "Cannot decode  a negative value :(");
    Ll nbytes;

    // If the byte starts with 10xxxxx, it's the middle of a UTF-8 sequence, so don't count it at all.
    if((byte & 0xc0) == 0x80) nbytes = 0;
    
    // The first byte's high bits tell us how many bytes are in the UTF-8 sequence.
    else if((byte & 0xf8) == 0xf0) nbytes = 4;
    else if((byte & 0xf0) == 0xe0) nbytes = 3;
    else if((byte & 0xe0) == 0xc0) nbytes = 2;
    else nbytes = 0;
    CORETEN_ENFORCE(nbytes > 0, "Invalid code point");
    return nbytes;
}

static inline char* utf8_encode(Rune value) {
    Byte mask = 0x3f; // 63
    char* dst = cast(char*)calloc(4, sizeof(char));

    if(value <= (value << 7) - 1) {
        dst[0] = cast(char)value;
        return dst;
    } else if(value <= (value << 11) - 1) {
        dst[0] = (0xc0) | (cast(char)(value >> 6));
        dst[1] = (0x80) | (cast(char)(value) & mask);
        return dst;
    } 
    // Invalid/Surrogate range
    if(value > CORETEN_RUNE_MAX || CORETEN_IS_BETWEEN(value, 0xd800, 0xdff)) {
        value = CORETEN_RUNE_INVALID;
        dst[0] = (0xe0) | (cast(char)(value >> 12));
        dst[1] = (0x80) | (cast(char)(value >> 12) & mask);
        dst[2] = (0x80) | (cast(char)(value) & mask);
        return dst;
    } else if(value <= (value << 16) - 1) {
        dst[0] = (0xe0) | (cast(char)(value >> 12));
        dst[1] = (0x80) | (cast(char)(value >> 6) & mask);
        dst[2] = (0x80) | (cast(char)(value) & mask);
        return dst;
    }

    dst[0] = (0xf0) | (cast(char)(value >> 18));
    dst[1] = (0x80) | (cast(char)(value >> 12) & mask);
    dst[2] = (0x80) | (cast(char)(value >> 6) & mask);
    dst[3] = (0x80) | (cast(char)(value) & mask);

    return dst;
}

/*
    WIP
*/

static cstlUTF8Str* ubuff_new(Rune* data) {
    cstlUTF8Str* ubuff = cast(cstlUTF8Str*)calloc(1, sizeof(ubuff));
    CORETEN_ENFORCE_NN(ubuff, "Could not allocate memory. Memory full.");

    // ubuff_set(ubuff, data);
    return ubuff;
}

void __grow_ubuff(cstlUTF8Str* ubuff, int grow_by) {
    CORETEN_ENFORCE_NN(ubuff, "Expected not null");
    CORETEN_ENFORCE(grow_by > 0);

    ubuff->data = cast(Byte*)realloc(ubuff->data, grow_by);
    memset(ubuff->data + ubuff->nbytes, 0, grow_by);
    ubuff->nbytes += grow_by;
}

void ubuff_push_char(cstlUTF8Str* ubuff, Rune ch) {
    CORETEN_ENFORCE(ubuff->data[ubuff->len] == 0); // ensure this is writeable (we shouldn't overwrite over a char)

    // 0b0xxxxxxx
    if(ch <= 0x7f) {
        __push_ascii_char(ubuff, ch);
    }
    // 110xxxxx 10xxxxxx
    else if(ch >= 0xC080 && ch <= 0xDFBF) {
        // 2-byte character
        __grow_ubuff(ubuff, 2);
        UInt64 nbytes = ubuff->nbytes;
        ubuff->data[nbytes    ] = cast(Byte)(ch >> 8);
        ubuff->data[nbytes + 1] = cast(Byte)(ch);
        ubuff->data[nbytes + 2] = 0;
        ubuff->nbytes += 2;
        ubuff->len += 1;
    }
    // 1110xxxx 10xxxxxx 10xxxxxx
    else if(ch >= 0xE08080 && ch <= 0xEFBFBF) {
        // 3-byte character
        __grow_ubuff(ubuff, 3);
        UInt64 nbytes = ubuff->nbytes;
        ubuff->data[nbytes    ] = cast(Byte)(ch >> 16);
        ubuff->data[nbytes + 1] = cast(Byte)(ch >> 8);
        ubuff->data[nbytes + 2] = cast(Byte)(ch);
        ubuff->data[nbytes + 3] = 0;
        ubuff->nbytes += 3;
        ubuff->len += 1;
    }
    // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    else if(ch >= 0xF0808080 && ch <= 0xF7BFBFBF) {
        // 3-byte character
        __grow_ubuff(ubuff, 3);
        UInt64 nbytes = ubuff->nbytes;
        ubuff->data[nbytes    ] = cast(Byte)(ch >> 24);
        ubuff->data[nbytes + 1] = cast(Byte)(ch >> 16);
        ubuff->data[nbytes + 2] = cast(Byte)(ch >> 8);
        ubuff->data[nbytes + 3] = cast(Byte)(ch);
        ubuff->data[nbytes + 4] = 0;
        ubuff->nbytes += 4;
        ubuff->len += 1;
    }
    else {
        // TODO (jasmcaus): `panic()` here
        fprintf(stderr, "Invalid UTF-8 character: %x", ch);
        exit(1);
    }
}

void __push_byte(cstlUTF8Str* ubuff, Byte byte) {
    CORETEN_ENFORCE(byte <= 0x7F);
    __grow_ubuff(ubuff, 1);
    ubuff->data[ubuff->nbytes++] = byte;
    ubuff->data[ubuff->nbytes] = 0;
}

void __push_ascii_char(cstlUTF8Str* ubuff, Byte byte) {
    __push_byte(ubuff, byte);
    ubuff->len += 1;
}

// Returns the number of UTF8 characters in the buffer
static UInt64 ubuff_len(cstlUTF8Str* ubuff) {
    return ubuff->len;
}

// Returns the number of bytes used by the UTF8 buffer.
static UInt64 ubuff_nbytes(cstlUTF8Str* ubuff) {
    return ubuff->nbytes;
}

/*
#define WRITE_OUT(val)	if(out)	*out = (val)
// Return the byte at the `n`th byte offset
// If `out` is null, we don't intend on knowing how many bytes the UTF8 character uses.
static int __byte_offset_at(cstlUTF8Str* ubuff, Int64 n, int* out) {
    CORETEN_ENFORCE_NN(ubuff, "Expected not null");
    CORETEN_ENFORCE_NN(ubuff->data, "Expected not null");
    // Fail-fast approach. If this changes, comment this, and uncomment the following (commented) snippet
    CORETEN_ENFORCE(n < ubuff->nbytes);
    CORETEN_ENFORCE(n > 0);
    // if((n >= ubuff->nbytes) || (n < 0)) {
    // 	return nullchar;
    // }

    Byte byte = cast(Byte)(ubuff->data[n]);
    if(byte == 0) {
        return cast(Byte)0;
    } 
    // 1-byte UTF8
    else if(byte <= 0x07F) {
        WRITE_OUT(1);
        return byte;
    } 
    // 2-byte UF8
    else if(byte >= 0x80 && byte <= 0x07FF) {
        WRITE_OUT(2);
        byte = (byte << 8) + cast(Byte)(ubuff->data[n + 1]);
        return byte;
    } 
    // 3-byte UTF8
    else if(byte >= 0x0800 && byte <= 0xFFFF) {
        WRITE_OUT(3);
        byte = byte << 16;
        byte = (byte) | (cast(Byte)(ubuff->data[n + 1]) << 8);
        byte = (byte) | (cast(Byte)(ubuff->data[n + 2]) << 16);
        return byte;
    }
    // 4-byte UTF8
    else if(byte >= 0x00010000 && byte <= 0x0010FFFF) {
        WRITE_OUT(4);
        byte = byte << 24;
        byte = (byte) | (cast(Byte)(ubuff->data[n + 1]) << 16);
        byte = (byte) | (cast(Byte)(ubuff->data[n + 2]) << 8);
        byte = (byte) | (cast(Byte)(ubuff->data[n + 3]));
        return byte;
    } else {
        // TODO (jasmcaus): `panic()` here
        fprintf(stderr, "Cannot decode UTF8 byte `%hx` at byte offset %d", byte, n);
        exit(1);
    }
}

// Return the byte at the `n`th byte offset
static Byte ubuff_byte_offset_at(cstlUTF8Str* ubuff, Int64 n) {
    return __byte_offset_at(ubuff, n, null);
}


// Returns the byte at `n`th character offset
static Byte ubuff_at(cstlUTF8Str* ubuff, Int64 n) {
    CORETEN_ENFORCE_NN(ubuff, "Expected not null");
    CORETEN_ENFORCE_NN(ubuff->data, "Expected not null");
    CORETEN_ENFORCE(n < ubuff->len);
    CORETEN_ENFORCE(n > 0);
    int a[1];

    UInt64 byte_offset = 0;
    while(n != 0) {
        Byte b = __byte_offset_at(ubuff, byte_offset, a); // we only care about `a`
        byte_offset += cast(UInt64)a;
        --n;
    }
    return ubuff_byte_offset_at(ubuff, byte_offset);
}
*/


// -------------------------------------------------------------------------
// vector.c
// -------------------------------------------------------------------------

// Create a new `cstlVector`
// size = size of each element (in bytes)
// capacity = number of elements
cstlVector* _vec_new(UInt64 objsize, UInt64 capacity) {
    if(capacity == 0)
        capacity = VEC_INIT_ALLOC_CAP;
    
    cstlVector* vec = cast(cstlVector*)calloc(1, sizeof(cstlVector));
    CORETEN_ENFORCE_NN(vec, "Could not allocate memory. Memory full.");

    vec->internal.data = cast(void*)calloc(objsize, capacity);
    if(!vec->internal.data) {
        free(vec);
        CORETEN_ENFORCE_NN(vec->internal.data, "Could not allocate memory. Memory full.");
    }

    vec->internal.capacity = capacity;
    vec->internal.size = 0;
    vec->internal.objsize = objsize;

    return vec;
} 

// Free a cstlVector from it's associated memory
void vec_free(cstlVector* vec) {
    if(vec) {
        if(vec->internal.data)
            free(vec->internal.data);
        free(vec);
    }
}

// Return a pointer to element `i` in `vec`
void* vec_at(cstlVector* vec, UInt64 elem) {
    CORETEN_ENFORCE_NN(vec, "Expected not null");
    CORETEN_ENFORCE_NN(vec->internal.data, "Expected not null");

    if(elem > vec->internal.size)
        return null;

    return VECTOR_AT_MACRO(vec, elem);
}

// Return a pointer to first element in `vec`
void* vec_begin(cstlVector* vec) {
    CORETEN_ENFORCE_NN(vec, "Expected not null");
    CORETEN_ENFORCE_NN(vec->internal.data, "Expected not null");

    if(vec->internal.size == 0) 
        return null;
    
    return vec->internal.data;
}

// Return a pointer to last element in `vec`
void* vec_end(cstlVector* vec) {
    CORETEN_ENFORCE_NN(vec, "Expected not null");
    CORETEN_ENFORCE_NN(vec->internal.data, "Expected not null");

    if(vec->internal.data == 0)
        return null;
    
    return (void*)(cast(char*)vec->internal.data + (vec->internal.size - 1) * sizeof(cstlVector));
}

// Is `vec` empty?
bool vec_is_empty(cstlVector* vec) {
    CORETEN_ENFORCE_NN(vec, "Expected not null");
    CORETEN_ENFORCE_NN(vec->internal.data, "Expected not null");

    return vec->internal.size == 0;
}

// Returns the size of `vec` (i.e the number of bytes)
UInt64 vec_size(cstlVector* vec) {
    CORETEN_ENFORCE_NN(vec, "Expected not null");
    CORETEN_ENFORCE_NN(vec->internal.data, "Expected not null");

    return vec->internal.size;
}

// Returns the allocated capacity of `vec` (i.e the number of bytes)
UInt64 vec_cap(cstlVector* vec) {
    CORETEN_ENFORCE_NN(vec, "Expected not null");
    CORETEN_ENFORCE_NN(vec->internal.data, "Expected not null");

    return vec->internal.capacity;
}

// Clear all contents of `vec`
bool vec_clear(cstlVector* vec) {
    CORETEN_ENFORCE_NN(vec, "Expected not null");
    CORETEN_ENFORCE_NN(vec->internal.data, "Expected not null");

    vec->internal.size = 0;
    return true;
}

// Push an element into `vec` (at the end)
bool vec_push(cstlVector* vec, const void* data) {
    CORETEN_ENFORCE_NN(vec, "Expected not null");
    CORETEN_ENFORCE_NN(vec->internal.data, "Expected not null");

    if(vec->internal.size + 1 > vec->internal.capacity) {
        bool result = __vec_grow(vec, vec->internal.size + 1);
        if(!result)
            return false;
    }

    CORETEN_ENFORCE(vec->internal.objsize > 0);

    if(vec->internal.data != null)
        memcpy(VECTOR_AT_MACRO(vec, vec->internal.size), data, vec->internal.objsize);

    vec->internal.size++;
    return true;
}

// Pop an element from the end of `vec`
bool vec_pop(cstlVector* vec) {
    CORETEN_ENFORCE_NN(vec, "Expected not null");
    CORETEN_ENFORCE_NN(vec->internal.data, "Expected not null");

    if(vec->internal.size == 0) 
        return false;
    
    vec->internal.size--;
    return true;
}

// Grow the capacity of `vec` to at least `capacity`.
// If more space is needed, grow `vec` to `capacity`, but at least by a factor of 1.5.
bool __vec_grow(cstlVector* vec, UInt64 capacity) {
    void* newdata;
    UInt64 newcapacity;

    CORETEN_ENFORCE_NN(vec, "Expected not null");
    CORETEN_ENFORCE_NN(vec->internal.data, "Expected not null");

    if (capacity <= vec->internal.capacity)
        return true;

    CORETEN_ENFORCE(vec->internal.objsize > 0);
    CORETEN_ENFORCE(capacity < cast(UInt64)-1/vec->internal.objsize);

    // Grow small vectors by a factor of 2, and 1.5 for larger ones
    if (vec->internal.capacity < VEC_INIT_ALLOC_CAP / vec->internal.objsize) {
        newcapacity = vec->internal.capacity + vec->internal.capacity + 1;
    } else {
        newcapacity = vec->internal.capacity + vec->internal.capacity / 2 + 1;
    }

    if (capacity > newcapacity || newcapacity >= (size_t) -1 / vec->internal.objsize)
        newcapacity = capacity;

    newdata = realloc(vec->internal.data, newcapacity * vec->internal.objsize);
    CORETEN_ENFORCE_NN(newdata, "Expected not null");

    vec->internal.data = newdata;
    vec->internal.capacity = newcapacity;

    return true;
}