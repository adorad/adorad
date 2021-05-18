# The Hazel Compiler 

I wrote the Hazel compiler to be as simple and readable as possible.  One of Hazel's goals is to be open to developers with 
different levels of experience in compiler development. Compilers don't need to be black boxes full of magic that only few 
people understand.

The Hazel compiler is modular, and can be used by other applications. It is located in `cmd/hazel/` and `hzlib/hazel/`.

The most important and useful command to remember when working on the Hazel compiler is `hazel self`. It rebuilds the Hazel 
compiler.

Be careful if you introduce a breaking change and rebuild Hazel: you will no longer be able to use Hazel to build itself. So 
it's a good idea to make a backup copy of a working compiler executable.

But don't worry, you can always simply run `make` (or `make.bat`), it will download the C version of the compiler and rebuild 
it from scratch.


### Compiler Architecture 
The architecture of the compiler is very simple and has three distinct steps:

1. Parse/generate AST (`hazel.parser`)
2. Check types (`hazel.checker`)
3. Generate C/JavaScript/machine code (`hazel.gen`)


The main files are:

1. `cmd/hazel/hazel.hzl` => The entry point.

- Hazel figures out the build mode.
- Constructs the compiler object (`struct Hazel`).
- Creates a list of .hzl files that need to be parsed.
- Creates a parser object for each file and runs `parse()` on them.
- The correct backend is called (C, JS, x64), and a binary is compiled.

2. `hazel/lexer` The lexer's job is to parse a list of characters and convert them to tokens.

3. `hazel/token` This is simply a list of all tokens, their string values, and a couple of helper functions.

4. `hazel/parser` The parser converts a list of tokens into an AST (Abstract Binary Tree). 
In Hazel, objects can be used before declaration, so unknown types are marked as unresolved. They are resolved later in the 
type checker.

5. `hazel/table` Hazel creates one table object that is shared by all parsers. It contains all types, consts, and functions, 
as well as several helpers to search for objects by name, register new objects, modify types' fields, etc.

6. `hazel/checker` Type checker and resolver. It processes the AST and makes sure the types are correct. Unresolved types are 
resolved, type information is added to the AST.

7. `hazel/gen/c` The C backend. It simply walks the AST and generates C code that can be compiled with Clang, GCC, Visual 
Studio, and TCC.

8. `json.hzl` defines the json code generation. 
> Note: This file will be removed once Hazel supports comptime code generation, and it will be possible to do this using the 
language's tools.

9. `hazel/gen/x64` is the directory with all the machine code generation logic. It defines a set of functions that translate 
assembly instructions to machine code and build the binary from scratch byte by byte. It manually builds all headers, 
segments, sections, symtable, relocations, etc. Right now it only has basic support of the x64 platform/ELF format.

The rest of the directories are hzlib modules: `builtin/` (Strings, Tensors, maps), `time/`, `os/`, etc. Their documentation is pretty clear.