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
sources = $(wildcard HazelInternalTests/Compiler/Lexer/*.c HazelInternalTests/Compiler/Tokens/*.c HazelInternalTests/Compiler/Types/*.c HazelInternalTests/*.c )
objects = $(sources:HazelInternalTests/.c=.o)

# To disable warnings, use "-w"
flags = -g -w -std=c11 -Wall -Wextra -Werror=return-type -Wno-unknown-pragmas -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-result -Wno-unused-local-typedefs -Wno-strict-overflow -Wno-strict-aliasing -Wno-error=deprecated-declarations -Wno-error=comment
CC = gcc


all :
	$(MAKE) internaltest
	$(CC) $(objects) $(flags) -o $(exec) -I .
	echo Compiled Hazel!
	echo ----------------------------------------
	echo ----------------------------------------
	$(exec)
.PHONY: all 

objects:
	echo $(sources)
.PHONY: objects 

emitcmd :
	echo $(CC) $(objects) $(flags) -o $(exec) -I .
.PHONY: emitcmd 

emitoutput :
	$(CC) -E Hazel/main.c $(flags) -o $(emitout) -I .
	echo Saved to $(emitout) ...
.PHONY: emitoutput 

emitsources :
	echo Sources: $(objects)
.PHONY: emitsources 

echo:
	echo All Engines are a go!
.PHONY: echo 

run:
	$(exec)
.PHONY: run

clean:
	rm $(exec).exe
.PHONY: clean


# ---------- CMAKE STUFF --------
TARGET = Hazel
STATICTARGET = HazelStatic
SHAREDTARGET = HazelShared
SOURCE_DIR = .
BUILD_DIR = build
RUN_DIR = build/bin
GENERATOR = "MinGW Makefiles" 

# PROCESS:
## 1. CMake Exec (generate the build files)
## 2. Run CMake Makefile
## 3. Run CXX compiled executable
 
cmake:
	cmake -S $(SOURCE_DIR) -B $(BUILD_DIR) -G $(GENERATOR)
	echo --------------------------------------------
	echo --------------------------------------------
	cd $(BUILD_DIR) && $(MAKE) && \
	echo --------------------------------------------
	echo ----------------- TESTING! -----------------
	echo --------------------------------------------
	cd $(BUILD_DIR)/test && ctest -j 10 -C Release --output-on-failure
.PHONY: cmake 

cmakemsvc:
	cmake -S $(SOURCE_DIR) -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	echo --------------------------------------------
	echo --------------------------------------------
	cd $(BUILD_DIR) && cmake --build . --config Release && \
	echo --------------------------------------------
	echo ----------------- TESTING! -----------------
	echo --------------------------------------------
	cd $(BUILD_DIR)/test && ctest -j 10 -C Release --output-on-failure
.PHONY: cmakemsvc 

# Generate the CMake MinGW Makefiles
cmakeexec: 
	cmake -S $(SOURCE_DIR) -B $(BUILD_DIR) -G $(GENERATOR)
.PHONY: cmakeexec

cmakeexecmsvc: 
	cmake -S $(SOURCE_DIR) -B $(BUILD_DIR)
.PHONY: cmakeexecmsvc

cmakemake:
	cd $(BUILD_DIR) && $(MAKE)
.PHONY: cmakemake

cmakerun:
	cd $(RUN_DIR) && $(TARGET)
.PHONY: cmakerun

cmakeclean: 
	rmdir /Q /S $(BUILD_DIR) && mkdir $(BUILD_DIR)
.PHONY: cmakeclean

# ------------ HazelInternalTests -----------
internaltest:
	python tools/tests/before_tests_ci.py
.PHONY: internaltest

# ------------ Minor Testing only -----------
test:
	$(CC) test.c $(flags) -o test -I . -I Hazel
	echo Compiled Test!
	echo -------------------
	echo -------------------
	test 
.PHONY: test 

emittestcmd:
	echo $(CC) test.c $(flags) -o test -I .
.PHONY: emittestcmd

emittestoutput:
	$(CC) -E test.c $(flags) -o $(emittestout) -I .
	echo Saved to $(emittestout) ...
.PHONY: emittestoutput 

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
# 		   $(SRCDIR)/hazel/compiler/tokens/__token.c                \

# 	python $(SRCDIR)/tools/scripts/generate_tokens.py token_py     \
# 		   $(SRCDIR)/hazel/compiler/grammar/Tokens                  \
# 		   $(SRCDIR)/hazel/compiler/tokens/__token.py               \
# .PHONY: regenerate-tokens
