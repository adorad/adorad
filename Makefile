# Top-level Makefile for the Hazel Language
#
# As distributed, this file is called Makefile.pre.in; it is processed into the real Makefile by running the script
# ./configure, which replaces things like @spam@ with values appropriate for your system.

# This means that if you edit Makefile, your changes get lost the next time you run the configure script.  Ideally, you can do:
#
#	./configure
#	make
#	make test
#	make install
#
# If you have a previous version of Hazel installed that you don't want to overwrite, you can use "make altinstall" instead
# of "make install".  Refer to the "Installing" section in the README file for additional details.
#
# See the section "Build instructions" in the README file for more instructions.

.PHONY : all echo compile run clean emitsources emitcmd emitoutput release test testclean 

# ======================== VARIABLES SET BY CONFIGURE ========================
VERSION    = @VERSION@
SRCDIR     = @SRCDIR@
ABS_SRCDIR = @ABS_SRCDIR@

GITVERSION = @GITVERSION@
GITTAG     = @GITTAG@
GITBRANCH  = @GITBRANCH@

# Executable suffix (.exe on Windows and Mac OS X)
EXE        =  @EXEEXT@
BUILDEXE   =  @BUILDEXEEXT@

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s
#Suppress display of executed commands.
$(VERBOSE).SILENT:

# ======================== MISC VARIABLES ========================
exec = hazel
emitout = hazeloutput.txt
emittestout = hazeltestoutput.txt
sources = $(wildcard Hazel/Compiler/IO/*.cpp Hazel/Compiler/Lexer/*.cpp Hazel/Compiler/Tokens/*.cpp Hazel/Compiler/File/*.cpp Hazel/Compiler/Ast/*.cpp Hazel/Compiler/Parser/*.cpp Hazel/*.cpp )
objects = $(sources:Hazel/.cpp=.o)

# To disable warnings, use "-w"
flags = -g -w -std=c++17
CC = g++

all :
	$(CC) $(objects) $(flags) -o $(exec) -I .
	echo Compiled Hazel!
	echo ----------------------------------------
	echo ----------------------------------------
	$(exec)
.PHONY: all 

emitcmd :
	echo $(CC) $(objects) $(flags) -o $(exec) -I .
.PHONY: emitcmd 

emitcmdtest:
	$(CC) test.cpp $(flags) -o test -I .
.PHONY: emitcmdtest

emitoutput :
	$(CC) -E Hazel/main.cpp $(flags) -o $(emitout) -I .
	echo Saved to $(emitout) ...
.PHONY: emitoutput 

emittestoutput :
	$(CC) -E test.cpp $(flags) -o $(emittestout) -I .
	echo Saved to $(emittestout) ...
.PHONY: emittestoutput 

emitsources :
	echo Sources: $(objects)
.PHONY: emitsources 

compile:
	$(CC) $(objects) $(flags) -I . -o $(exec)
.PHONY: compile

echo:
	echo All Engines are a go!
.PHONY: echo 

run:
	$(exec)
.PHONY: run

release:
	$(CC) $(objects) $(flags) -I . -o $(exec)

clean:
	rm $(exec).exe
.PHONY: clean


# For test.cpp (internal usage only)
test:
	$(CC) test.cpp $(flags) -o test -I .
	echo Compiled Test!
	echo -------------------
	echo -------------------
	test 
.PHONY: test 

testclean:
	rm test.exe
.PHONY: testclean 

# regenerate-tokens:
# 	# Regenerate hazel/compiler/tokens/token.h from tools/scripts/generate_tokens.py
# 	python $(SRCDIR)/tools/scripts/generate_tokens.py token_header \
# 		   $(SRCDIR)/hazel/compiler/grammar/Tokens                  \
# 		   $(SRCDIR)/hazel/compiler/tokens/__token.h                \

# 	python $(SRCDIR)/tools/scripts/generate_tokens.py token_c      \
# 		   $(SRCDIR)/hazel/compiler/grammar/Tokens                  \
# 		   $(SRCDIR)/hazel/compiler/tokens/__token.cpp                \

# 	python $(SRCDIR)/tools/scripts/generate_tokens.py token_py     \
# 		   $(SRCDIR)/hazel/compiler/grammar/Tokens                  \
# 		   $(SRCDIR)/hazel/compiler/tokens/__token.py               \
# .PHONY: regenerate-tokens

