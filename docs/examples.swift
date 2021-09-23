import std::string
import std::hash as hash
from std::hash import some_really_important_function

// Simple Functions
func greet(name: string, msg: string) -> string {
    return f"Hello there, {name}! A message came through for you: {msg}"
}

// Generic Functions
func holler<T>(name: T, msg: T) -> T 
where
    T: {string}
{
    return f"Hello there, {name}! A message: {msg}"
}

// Comptime Fucntions
[comptime] func add(a: i8, b: u32) -> u32 {
    return @cast(u32, a) + b
}

// Export functions
export func subtract(a: i8, b: i8) -> u32 {
    return @cast(u32, (a-b))
}

// Global variables
global {
    favourite_fruit: string = "Mango"
    lucky_num: i32 = 13
}

Fruits = enum {
    Apple,
    Mango,
    Jackfruit,
}

func main() {
    parser = parser_new()
    parser := parser_new()
    if parser == nil
        print("nil value")
    loop {
        match parser.curr_tok {
            when '\n' ==> ...
            when '\v' ==> ...
            default ==> ...
        }
    } // loop
}

// Aliases
alias Fruits as DeliciousFruits


