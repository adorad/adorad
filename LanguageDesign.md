# The Hazel Language Design 

We use this resource: https://gist.github.com/hawkw/9213690

## Importing
Hazel uses the ``import`` keyword to import Hazel modules/packages and the ``include <file.hzl>`` syntax to use an existing
``.hzl`` file. 

It's that simple: You ``import`` packages and ``include`` files. Nothing more, nothing else. 

For example: 
```cpp
// This imports a dummy package called "Latte" 
import Latte

// This uses a single Hazel file "latte.hz"
include <latte.hz>
```

You can also create aliases for packages using the ``as`` keyword:
```cpp
import Latte as lt 

include <latte.hzl> as coffee
```

This will now refer to the ``Latte`` packages as ``lt`` and the include file ``latte.hzl`` as ``coffee``. 

Note that to include multiple Hazel files, you'll need to add the ``include <file.hzl>`` syntax for as many ``.hzl`` 
files you intend to add to your project

## Comments 
Hazel uses the ``//`` syntax for one-line comments and the Python-style ```""" """``` for multi-line comments. 

```cpp
// This is a one-line comment 

"""
This comment is a 
multi-line comment. 
Embrace Hazel's powerful 
JIT Engine. 
"""
```


## Defining a Function 

## Defining a Class 

## Memory Managment


