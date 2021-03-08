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

.PHONY : all run compiler clean

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
sources = $(wildcard hazel/runtime/lexer/*.c hazel/runtime/parser/*.c hazel/*.c )
objects = $(sources:Hazel/.c=.o)
flags = -g

all :
	gcc $(objects) $(flags) -o $(exec) -I .
	echo Compiled Hazel 
	echo ----------------------------------------
	echo ----------------------------------------
	$(exec)
.PHONY: all 

compile:
	gcc $(objects) $(flags) -o $(exec) -I .
.PHONY: compile

run:
	$(exec)
.PHONY: run

compiler:
	gcc -c hazel/compiler/hazel.c
.PHONY: compiler

clean:
	rm $(exec).exe
.PHONY: clean

# For test.c (internal usage only)
compiletest:
	gcc test.c $(flags) -o test
.PHONY: compiletest 

runtest:
	test
.PHONY: runtest 

cleantest:
	rm test.exe
.PHONY: cleantest 

regenerate-tokens:
	# Regenerate hazel/runtime/parser/token.h from tools/scripts/generate_tokens.py
	python $(SRCDIR)/tools/scripts/generate_tokens.py token_header \
		   $(SRCDIR)/hazel/runtime/grammar/Tokens                  \
		   $(SRCDIR)/hazel/runtime/parser/__token.h                \

	python $(SRCDIR)/tools/scripts/generate_tokens.py token_c      \
		   $(SRCDIR)/hazel/runtime/grammar/Tokens                  \
		   $(SRCDIR)/hazel/runtime/parser/__token.c                \

	python $(SRCDIR)/tools/scripts/generate_tokens.py token_py     \
		   $(SRCDIR)/hazel/runtime/grammar/Tokens                  \
		   $(SRCDIR)/hazel/runtime/parser/__token.py               \
.PHONY: regenerate-tokens

