/*
_ _    _           ______   _______        
| |  | |    /\    /___  /   |  ____|| |    
| |__| |   /  \      / /    | |__   | |       Adorad - The Fast, Expressive & Elegant Programming Language
|  __  |  / /\ \    / /     |  __|  | |       Languages: C, C++, and Assembly
| |  | | / ____ \  / /___   | |____ | |____   https://github.com/AdoradLang/adorad/
|_|_ |_|/_/    \_\/_______\ |______|_\______|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

// Includes all the files needed for Adorad compilation

// This ALWAYS needs to be right at the top 
#include <adorad/core/hcore.h> 

#include <adorad/compiler/types.h>
#include <adorad/compiler/tokens.h>  
#include <adorad/compiler/lexer.h>
#include <adorad/compiler/ast.h>
#include <adorad/compiler/parser.h>