<div align="center">
<!-- <p>
    <img width="80" src="https://raw.githubusercontent.com/hazel-lang/Hazel-logo/master/dist/hazel-logo.svg?sanitize=true">
</p> -->
<h1>The Hazel Language</h1>

[Hazel](https://github.com/hazel-lang/Hazel) |
[Documentation](https://github.com/hazel-lang/Hazel/blob/master/doc/docs.md) |
[Contributing](https://github.com/hazel-lang/Hazel/blob/master/CONTRIBUTING.md) |
[Compiler design](https://github.com/hazel-lang/Hazel/blob/master/COMPILER.md)

</div>
<div align="center">

<!--
[![Build Status][WorkflowBadge]][WorkflowUrl]
-->
<!-- [![Sponsor][SponsorBadge]][SponsorUrl]
[![Patreon][PatreonBadge]][PatreonUrl]
[![Discord][DiscordBadge]][DiscordUrl] -->
[![Twitter][TwitterUrl]][TwitterBadge]

</div>

<!-- This is the repository for Hazel development code. The language design is based very similarly to C++, Lua and Julia. -->

## Key Features of Hazel

- Simplicity: the language can be learned in less than an hour
- Fast compilation: ≈80k loc/s with a Clang backend,
    ≈1 million loc/s with x64 and tcc backends *(Intel i5-7500, SSD, no optimization)*
- Easy to develop: Hazel compiles itself in less than a second
- Performance: as fast as C (Hazel's main backend compiles to human-readable C)
- Safety: no null, no globals, no undefined behavior, immutability by default
- C to Hazel translation
- Hot code reloading
- Innovative memory management
- Built-in Vision library
- Easy cross compilation
- REPL
- [Built-in ORM](https://github.com/hazel-lang/Hazel/blob/master/doc/docs.md#orm)
- C backend

<!--
## Interactive Shell

```shell
$ hazel
    Hazel Language 0.0.1 (Apr 8 2021 02:39:23)
    GCC version: 9.3.0 on linux
    All engines are a go!

hazel> print("Hello World")
Hello World
hazel> exit
    Goodbye! Set the world on fire!
```

Using a script `hello.hzl`:

```ruby
print("Hello World")
```

Running on the terminal is as easy as:

```shell
$ hazel hello.hzl
Hello World
```

## Compiler

```shell
$ hazel compile hello.hzl -o hello
Starting the compiler engine...
Compiling Hazel code into build/hello.c
Compiling the C code into machine code...
Cleaning up the temporary files...

Finished compiling.

Binary is ready here: build/hello
$ build/hello
hello world
```

Run `hazel --help` to see more options.

## Uninstall Hazel

You can uninstall the `hazel` binary and its C headers with:

```shell
make uninstall
```

## Stability guarantee and future changes

Despite being at an early development stage, the Hazel language is relatively stable and has backwards compatibility 
guarantee, meaning that the code you write today is guaranteed to work a month, a year, or five years from now.

There still may be minor syntax changes before the 1.0 release, but they will be handled automatically.

The Hazel core APIs (the standard modules) will still have minor changes until they are stabilized in end-2021. Of course the 
APIs will grow after that, but without breaking existing code.

Our guarantee: Hazel is always going to be lightweight, portable and *extremely* fast. 
-->

## Installing Hazel from Source

### Linux, macOS, Windows, *BSD, Solaris, WSL, Android, Raspbian

```shell
git clone https://github.com/hazel-lang/Hazel
cd Hazel
cmake -S . -B build
```

That's it! Now you have a Hazel executable at `[path to Hazel repo]/build/Hazel`. 
`[path to Hazel repo]` can be anywhere.
<!--
(On Windows `make` means running `make.bat`, so make sure you use `cmd.exe`)

Now you can try `./hazel run examples/hello_world.hzl` (`hazel.exe` on Windows).

Hazel is constantly being updated. To update Hazel, simply run:

```shell
hazel up
```


### C compiler

It's recommended to use Clang, GCC, or Visual Studio. If you are in development, you most likely already have one of those 
installed.

However, if none is found when running `make` on Linux or Windows, TCC is downloaded as the default C backend.
It's very lightweight (several MB) so this shouldn't take too long.


### Symlinking

NB: it is *highly recommended*, that you put Hazel on your PATH. That saves you the effort to type in the full path to your 
Hazel executable every time. Hazel provides a convenience `hazel symlink` command to do that more easily.

On Unix systems, it creates a `/usr/local/bin/Hazel` symlink to your executable. To do that, run:

```shell
sudo ./hazel symlink
```

On Windows, start a new shell with administrative privileges, for example by <kbd>Windows Key</kbd>, then type `cmd.exe`, 
right click on its menu entry, and choose `Run as administrator`. In the new administrative shell, cd to the path, where you 
have compiled hazel.exe, then type:

```bat
.\hazel.exe symlink
```

That will make Hazel available everywhere, by adding it to your PATH. You might need to restart your shell/editor after that, 
so that it can pick the new PATH variable.

NB: there is no need to run `hazel symlink` more than once - Hazel will continue to be available, even after `hazel up`, 
restarts and so on. 

You only need to run it again, if you decide to move the Hazel repo folder somewhere else.
-->
<!-- ### Docker

<details><summary>Expand Docker instructions</summary>

```bash
git clone https://github.com/hazel-lang/Hazel
cd v
docker build -t vlang .
docker run --rm -it vlang:latest
```

### Docker with Alpine/musl

```bash
git clone https://github.com/hazel-lang/Hazel
cd v
docker build -t vlang --file=Dockerfile.alpine .
docker run --rm -it vlang:latest
```

</details> -->

<!--
### Testing and running the examples

Make sure Hazel can compile itself:

```shell
hazel self
```

```shell
$ hazel
    Hazel Language 0.0.1 (Apr 8 2021 02:39:23)
    GCC version: 9.3.0 on linux
    All engines are a go!

hazel> print('Hello world')
Hello world
```

```bash
cd examples
hazel hello_world.hzl && ./hello_world   # or simply
hazel  hello_world.hzl                   # this builds the program and runs it right away
```

## Hazel sync
Hazel's `sync` module and channel implementation uses libatomic.

It is most likely already installed on your system, but if not, you can install it, by doing the following:

```bash
MacOS: already installed

Debian/Ubuntu:
sudo apt install libatomic1

Fedora/CentOS/RH:
sudo dnf install libatomic-static
```
-->
## Troubleshooting
Please file an issue in GitHub [here](https://github.com/HazelLang/Hazel/issues
<!--
Please see the [Troubleshooting](https://github.com/HazelLang/Hazel/wiki/Troubleshooting) section on our [wiki page](https://github.com/hazel-lang/Hazel/wiki)
-->

[TwitterBadge]: https://twitter.com/jasmcaus
[TwitterUrl]: https://img.shields.io/twitter/follow/jasmcaus.svg?style=flatl&label=Follow&logo=twitter&logoColor=white&color=1da1f2
