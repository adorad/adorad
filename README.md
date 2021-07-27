<div align="center">
<!-- <p>
    <img width="80" src="https://raw.githubusercontent.com/AdoradLang/Adorad-logo/master/dist/adorad-logo.svg?sanitize=true">
</p> -->
<h1>The Adorad Language</h1>

[Adorad](https://github.com/AdoradLang/Adorad) |
[Documentation](https://github.com/AdoradLang/adorad/blob/master/doc/docs.md) |
[Contributing](https://github.com/AdoradLang/adorad/blob/master/CONTRIBUTING.md) |
[Compiler design](https://github.com/AdoradLang/adorad/blob/master/COMPILER.md)

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

## Key Features of Adorad

- Simplicity: the language can be learned in less than an hour
- Fast compilation
- Easy to develop: Adorad compiles itself in less than a second
- Performance: as fast as C (Adorad's main backend compiles to human-readable C)
- Safety: no undefined behavior, immutability by default
- C to Adorad translation
- Easy cross compilation
- REPL

<!--
## Interactive Shell

```shell
$ adorad
    Adorad Language 0.0.1 (Apr 8 2021 02:39:23)
    GCC version: 9.3.0 on linux
    All engines are a go!

adorad> print("Hello World")
Hello World
adorad> exit
    Goodbye! Set the world on fire!
```

Using a script `hello.hzl`:

```ruby
print("Hello World")
```

Running on the terminal is as easy as:

```shell
$ adorad hello.hzl
Hello World
```

## Compiler

```shell
$ adorad compile hello.hzl -o hello
Starting the compiler engine...
Compiling Adorad code into build/hello.c
Compiling the C code into machine code...
Cleaning up the temporary files...

Finished compiling.

Binary is ready here: build/hello
$ build/hello
hello world
```

Run `adorad --help` to see more options.

## Uninstall Adorad

You can uninstall the `adorad` binary and its C headers with:

```shell
make uninstall
```

## Stability guarantee and future changes

Despite being at an early development stage, the Adorad language is relatively stable and has backwards compatibility 
guarantee, meaning that the code you write today is guaranteed to work a month, a year, or five years from now.

There still may be minor syntax changes before the 1.0 release, but they will be handled automatically.

The Adorad core APIs (the standard modules) will still have minor changes until they are stabilized in end-2021. Of course the 
APIs will grow after that, but without breaking existing code.

Our guarantee: Adorad is always going to be lightweight, portable and *extremely* fast. 
-->

## Installing Adorad from Source

### Linux, macOS, Windows, *BSD, Solaris, WSL, Android, Raspbian

```shell
git clone https://github.com/AdoradLang/Adorad
cd Adorad
cmake -S . -B build
```

That's it! Now you have a Adorad executable at `[path to Adorad repo]/build/Adorad`. 
<!-- `[path to Adorad repo]` can be anywhere. -->
<!--
(On Windows `make` means running `make.bat`, so make sure you use `cmd.exe`)

Now you can try `./adorad run examples/hello_world.hzl` (`adorad.exe` on Windows).

Adorad is constantly being updated. To update Adorad, simply run:

```shell
adorad up
```


### C compiler

It's recommended to use Clang, GCC, or Visual Studio. If you are in development, you most likely already have one of those 
installed.

However, if none is found when running `make` on Linux or Windows, TCC is downloaded as the default C backend.
It's very lightweight (several MB) so this shouldn't take too long.

</details> -->

<!--
### Testing and running the examples

Make sure Adorad can compile itself:

```shell
adorad self
```

```shell
$ adorad
    Adorad Language 0.0.1 (Apr 8 2021 02:39:23)
    GCC version: 9.3.0 on linux
    All engines are a go!

adorad> print('Hello world')
Hello world
```

```bash
cd examples
adorad hello_world.hzl && ./hello_world   # or simply
adorad  hello_world.hzl                   # this builds the program and runs it right away
```

## Adorad sync
Adorad's `sync` module and channel implementation uses libatomic.

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
Please file an issue in GitHub [here](https://github.com/AdoradLang/adorad/issues)
<!--
Please see the [Troubleshooting](https://github.com/AdoradLang/adorad/wiki/Troubleshooting) section on our [wiki page](https://github.com/AdoradLang/adorad/wiki)
-->

## Supporting This Project

If you are able to and would like to sponsor this project, I'm available here: 

It is never required, but always appreciated :)

<p align="center">
<!--    <a href="https://www.buymeacoffee.com/jasmcaus" target = "_blank"><img alt="Buy Jason a Coffee" width="48px" src="https://raw.githubusercontent.com/adi1090x/files/master/other/1.png"></a> -->
    <a href="https://www.ko-fi.com/jasmcaus" target="_blank"><img alt="Buy Jason a Coffee" width="48px" src="https://raw.githubusercontent.com/adi1090x/files/master/other/2.png"></a>
    <a href="https://www.paypal.me/jasmcaus" target="_blank"><img alt="Buy Jason a Coffee" width="48px" src="https://raw.githubusercontent.com/adi1090x/files/master/other/3.png"></a>
    <a href="https://www.patreon.com/jasmcaus" target="_blank"><img alt="Buy Jason a Coffee" width=48px src="https://raw.githubusercontent.com/adi1090x/files/master/other/4.png"></a>
</p>



[TwitterBadge]: https://twitter.com/jasmcaus
[TwitterUrl]: https://img.shields.io/twitter/follow/jasmcaus.svg?style=flatl&label=Follow&logo=twitter&logoColor=white&color=1da1f2
