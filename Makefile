# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s
#Suppress display of executed commands.
$(VERBOSE).SILENT:
MAKEFLAGS += --silent

SOURCE_DIR = .
BUILD_DIR = build
# GENERATOR = -G "MinGW Makefiles"

cmake:
	python3 ./tools/tests/before_tests_ci.py
	cmake -S $(SOURCE_DIR) -B $(BUILD_DIR) $(GENERATOR) -DCMAKE_BUILD_TYPE=Debug -DADORAD_BUILDTESTS=ON
	cmake --build $(BUILD_DIR) --config Debug
	echo ------------------ Running Target ------------------
	cd build/bin/ ; ./AdoradInternalTestsWithMain
.PHONY: cmake 

clean:
	$(MAKE) cmakeclean
.PHONY: clean

cmakeclean:
	rm -rf build
	rm -rf AdoradInternalTests
.PHONY: cmakeclean

test:
	gcc test.c -o test.exe -g -I .
	./test.exe
.PHONY: test

testoutput:
	gcc -E test.c -o test.txt -g -I .
.PHONY: testoutput

testclean:
	rm test.exe 
.PHONY: testclean

testvalgrind:
	valgrind ./test.exe --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose
.PHONY: testvalgrind

emitoutput:
	gcc -E adorad/core/buffer.h -I . -o adoradoutput.txt
.PHONY: emitoutput