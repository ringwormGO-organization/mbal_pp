# How to start contributing
Just drop a Pull Request :)

## What can I do?
- fix bugs
- implement new features (maybe from [TODO](https://github.com/ringwormGO-organization/mbal_pp#todo) list)
- or simply check [For those who want know more](#for-those-who-want-know-more) section

# Coding style
1. Use LF instead of CRLF
2. Use spaces, size 4.

*Please run `make production` before pushing code*

## Indentation and line width
1. Indent both a case label and the case statement of a switch statement.

Right:

```c
switch (Condition)
{
    case 1:
        DoSomething();
        break;
}
```
Wrong:

```c
switch (Condition)
{
case 1:
     DoSomething();
     break;
}
```
When a function call does not fit onto a line, align arguments like this:

```c
FunctionCall(arg1,
             arg2,
             arg3);
```

When making new functions, use an return type.
Right:
```c
int func() {}
```
Wrong:
```c
func() {}
```

## Spacing
1. Do not use spaces around unary operators.

**Right:** `i++`

**Wrong:** `i ++`

-------------------------------------------------------------

2. Place spaces around binary and ternary operators.

**Right:** `a = b + c;`

**Wrong:** `a=b+c;`

3. Do not place spaces before comma and semicolon.

Right:

```c
for (int i = 0; i < 5; i++)
    DoSomething();

func1(a, b);
```

Wrong:

```c
for (int i = 0 ; i < 5 ; i++)
    DoSomething();
    
func1(a , b) ;
```

4. Place spaces between control statements and their parentheses.

Right:

```c
if (Condition)
    DoSomething();
```

Wrong:

```c
if(Condition)
    DoSomething();
```

5. Do not place spaces between a function and its parentheses (except for curly parentheses), or between a parenthesis and its content.

Right:

```c
func(a, b);
return { a, b };
````

Wrong:

```c
func (a, b);
func( a, b );

return {a, b};
```

### Line breaking
1. Each statement should get its own line.

Right:

```c
x++;
y++;

if (Condition)
{
    DoSomething();
}
```
Also right but don't use it often

```c
if (Condition) DoSomething(); 

if (Condition)
    DoSomething();
```
Wrong:

```c
x++; y++;
```

## Braces
1. Always put braces ({ and }) on their own lines.
2. One-line control clauses may use braces, but this is not a requirement. An exception are one-line control clauses including additional comments.

Right:

```c
if (Condition)
    DoSomething();

if (Condition)
{
    DoSomething();
}

if (Condition)
{
    // This is a comment
    DoSomething();
}


if (Condition)
    DoSomething();
else
    DoSomethingElse();

if (Condition)
{
    DoSomething();
}

else
{
    DoSomethingElse();
    YetAnother();
}
```

Wrong:

```c
if (Condition) {
    DoSomething();
}

if (Condition)
    // This is a comment
    DoSomething();

if (Condition)
    DoSomething();
else
{
    DoSomethingElse();
    YetAnother();
}
```

## Control structures
1. Don’t use inverse logic in control clauses.

**Right:** `if (i == 1)`
**Wrong:** `if (1 == i)`

2. Avoid too many levels of cascaded control structures. Prefer a “linear style” over a “tree style”. Use goto when it helps to make the code cleaner (e.g. for cleanup paths).

Right:

```c
if (!func1())
    return;

i = func2();
if (i == 0)
    return;

j = func3();
if (j == 1)
    return;
```
…

Wrong:

```c
if (func1())
{
    i = func2();
    if (func2())
    {
        j = func3();
        if (func3())
        {
            …
        }
    }
}
```

## Naming
### Naming functions
1. Function names are lowercase letters.

**Right:** `void func();`

**Wrong:** `void Func();`

### Naming structs, classes, unions and variables
2. Name a struct, class, union etc. with cappital letter and variable which lowercase letter

**Right:**
```c
struct Test
{
  int number;
}

Test test;
```

**Wrong:**
```c
struct test
{
  int Number;
}

test Test;
```

## Commenting
1. Avoid line-wasting comments, which could fit into a single line.

Right:

```c
// This is a one-line comment

/* This is a C-style comment */


//
// This is a comment over multiple lines.
// We don’t define any strict rules for it.
//
```

Wrong:

```c
//
// This comment wastes two lines
//
```

## Other points
1. Do not use `LARGE_INTEGER`/`ULARGE_INTEGER` unless needed for using APIs. Use `int64`/`uint64` instead
2. Use `#pragma once` instead of guard defines in headers

# Used resources:
* https://reactos.org/wiki/Coding_Style

<br></br>

# For those who want know more

## Adding new built-in function
1. Add new built-in function to `global_symbol_table` in `main.cpp`
```cpp
global_symbol_table->set(std::string("name"), std::make_shared<BuiltInFunction>("name"));
```
2. Make appropriate function in `Values.hpp`
```cpp
std::shared_ptr<RTResult> execute_name(std::shared_ptr<Context> exec_ctx);
```
3. Make an entry in `functions` map in `Values.cpp`
```cpp
{"name", [this](std::shared_ptr<Context> context) { return this->execute_name(context); }},
```
4. Add `if` or `else if` statment (depends on alphabetical order)
```cpp
else if (this->name == "name")
{
    this->arg_names.push_back("see arguments list down below");

    /**
     * value - string
     * index - number
     * list - list
     */
}
```

5. Make `execute_name` function
```cpp
std::shared_ptr<RTResult> BuiltInFunction::execute_name(std::shared_ptr<Context> exec_ctx)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    /* your code */

    return res->success(std::make_shared<Number>(0)); /* Number.null - CodePulse's code */
}
```

### Notes
- replace all instances of `name` to function's name
- all code here is required (except comments)
- you get variable using `symbol_table` variable in `exec_ctx` variable (be careful when using `get_value` function because it can return `nullptr`, base class `Value` or any derived class from it)
