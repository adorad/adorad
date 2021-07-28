/*
          _____         _____           _____  
    /\   |  __ \ / __ \|  __ \    /\   |  __ \
   /  \  | |  | | |  | | |__)|   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
  / /\ \ | |  | | |  | |  _  /  / /\ \ | |  | | Languages: C, C++, and Assembly
 / ____ \| |__| | |__| | | \ \ / ____ \| |__| | https://github.com/adorad/adorad/
/_/_ __\_\_____/_\____/|_|__\_/_/_ __\_\_____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

// Includes all the files needed for Adorad compilation

// This ALWAYS needs to be right at the top 
#include <adorad/core/hcore.h> 

#include <adorad/compiler/types.h>
#include <adorad/compiler/tokens.h>  
#include <adorad/compiler/lexer.h>
#include <adorad/compiler/ast.h>
#include <adorad/compiler/parser.h>