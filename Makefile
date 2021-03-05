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


# ======================== MISC VARIABLES ========================
exec = hello.out 
sources = $(wildcard Hazel/*.c Hazel/lexer/*.c)
objects = $(sources:Hazel/.c=.o)
flags = -g 

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

run:
	hello.out

compiler:
	gcc -c hazel/compiler/hazel.c

%.o: %.c Hazel/lexer/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm Hazel/*.o

.PHONY 
regenerate-tokens:
	# Regenerate hazel/runtime/parser/token.h from tools/scripts/generate_tokens.py 
	python $(SRCDIR)/tools/scripts/generate_tokens.py token_header \
		   $(SRCDIR)/hazel/runtime/grammar/Tokens       \ 
		   $(SRCDIR)/hazel/runtime/parser/__token.h       \

	python $(SRCDIR)/tools/scripts/generate_tokens.py token_c \
		   $(SRCDIR)/hazel/runtime/grammar/Tokens       \ 
		   $(SRCDIR)/hazel/runtime/parser/__token.c       \

	python $(SRCDIR)/tools/scripts/generate_tokens.py token_py \
		   $(SRCDIR)/hazel/runtime/grammar/Tokens       \ 
		   $(SRCDIR)/hazel/runtime/parser/__token.py      \
		   

