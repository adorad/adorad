/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Hazel - The Fast, Expressive & Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/HazelLang/Hazel/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

// Includes all the files needed for Hazel compilation

// This ALWAYS needs to be right at the top 
#include <Hazel/Core/HCore.h> 

#include <Hazel/Compiler/Types/Types.h> 
#include <Hazel/Compiler/Lexer/Lexer.h>
#include <Hazel/Compiler/Lexer/Location.h>
#include <Hazel/Compiler/Tokens/Tokens.h> 
// #include <Hazel/Compiler/Ast/Ast.h>
// #include <Hazel/Compiler/Parser/Parser.h> 