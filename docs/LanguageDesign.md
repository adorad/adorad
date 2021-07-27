# Inspiration
1. https://github.com/Encryptr/O-plus-plus
2. https://github.com/artagnon/rhine
3. https://github.com/vczh/tinymoe

# The Adorad Language Design 

We use this resource: https://gist.github.com/hawkw/9213690

## Importing
Adorad uses the ``import`` keyword to import Adorad modules/packages and the ``include <file.hzl>`` syntax to use an existing
``.hzl`` file. 

It's that simple: You ``import`` packages and ``include`` files. Nothing more, nothing else. 

For example: 
```cpp
// This imports a dummy package called "Latte" 
import Latte

// This uses a single Adorad file "latte.hz"
include <latte.hz>
```

You can also create aliases for packages using the ``as`` keyword:
```cpp
import Latte as lt 

include <latte.hzl> as coffee
```

This will now refer to the ``Latte`` packages as ``lt`` and the include file ``latte.hzl`` as ``coffee``. 

Note that to include multiple Adorad files, you'll need to add the ``include <file.hzl>`` syntax for as many ``.hzl`` 
files you intend to add to your project

## Comments 
Adorad uses the ``//`` syntax for one-line comments and the Python-style ```""" """``` for multi-line comments. 

```cpp
// This is a one-line comment 

"""
This comment is a 
multi-line comment. 
Embrace Adorad's powerful 
JIT Engine. 
"""
```


## Defining a Function 
To define a function simply stick the ``def`` keyword infront. Since Adorad is *statically-typed*, each function argument **must** have an associated type to it (this may be a <ins>built-in type</ins> or a <ins>custom type</ins>). Also like everything else in Adorad, you must use the ``{ }`` brackets to define your code block (Adorad's JIT Engine will complain if this is omitted). If you're used to programming in Scala, this won't be much of a jump.

```python
def do_something(String message) {
    print(message)
}

def do_something_else(Uint8 num1, Uint8 num2) {
    return num1 + num2 
}
```

## Macros 
Adorad uses LISP-style macros (see [here](https://stopa.io/post/229) for more).
```js
macro nullthrows(sourceCodeSnippet) {
    return String(
        const result = ${sourceCodeSnippet}; 
        if (result is null or result is undefined) {
            throw new Error("Uh oh, this returned null:" + ${String(sourceCodeSnippet)});
        } else return result;
    )
}
```

## Defining a Class 

## Memory Managment


