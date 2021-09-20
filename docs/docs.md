# Adorad Documentation

## Introduction

Adorad is a statically-typed, compiled programming language designed for building scalable & maintainable software.

Adorad is a very simple language: going through this documentation will take you about an hour or two at most,
and by the end of it, you will have a pretty solid hold of the entire language.

The language promotes writing simple and clear code with minimal abstraction. Despite being simple, Adorad gives you a ton of power - anything you can do in other languages, you can do in Adorad.

## 1. Lexical Conventions
### a. Source Encoding
Adorad source code is UTF-8 encoded. Invalid UTF-8 byte sequences result in compile-time errors.

### b. Whitespace
Whitespace is any non-empty string containing either of the following patterns:
* `U+0009` (horizontal tab, `'\t'`)
* `U+000A` (line feed, `'\n'`)
* `U+000B` (vertical tab)
* `U+000C` (form feed)
* `U+000D` (carriage return, `'\r'`)
* `U+0020` (space, ' ')
* `U+0085` (next line)
* `U+200E` (left-to-right mark)
* `U+200F` (right-to-left mark)
* `U+2028` (line separator)
* `U+2029` (paragraph separator)