# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s
#Suppress display of executed commands.
$(VERBOSE).SILENT:

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

cmakeclean:
	rm -rf build/ && mkdir build
.PHONY: cmakeclean

test:
	gcc test.c -o test.exe -I .
	./test.exe
.PHONY: test

testclean:
	rm test.exe 
.PHONY: testclean

emitoutput:
	gcc -E adorad/core/buffer.h -I . -o adoradoutput.txt
.PHONY: emitoutput