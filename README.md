# lighzy-interpreter

## Translations

[English](README.md)  
[中文](README-zh_CN.md)  

## Introduction

lighzy-interpreter is a cross-platform interpreter in C++ for lighzy language. [Lighzy](https://github.com/QtLittleXu/Lighzy) is dynamic, strongly typed and interpreted simple programming language by me. I created the project 2 months ago to improve my programming skills. Now I have finished the most of the features. I am considering to support for oriented-programming. It is really difficult to implement.

## Build & Run & Test

### Required Environments

The project is building by CMake. You need to install the following libraries, or use package managers such as vcpkg to install them simpler:

- argparse (required, for arguments parsing)
- gtest (optional, for test)

### Build

Run these commands to start building release version:

```shell
cmake -B build -S . -D BUILD_TYPE=Release -D BUILD_TEST=ON
cmake --build build --config Release
```

You can find the compiled executable file `li` in the folder `build/bin` at the end of building.

### Run & Use

Jump to the folder `build/bin` and run `./li --help` to get help:

```shell
> ./li --help
Usage: ./li [--help] [--version] [--repl] [--output VAR] source

lighzy-interpreter is a simple interpreter for Lighzy language

Positional arguments:
	source         input source [nargs=0..1]

Optional arguments:
	-h, --help     shows help message and exits
	-v, --version  prints version information and exits
	-r, --repl     run in Read-Evaluate-Print-Loop mode
	-o, --output   specify the output file, the default is standard output
```

Run `./li --repl` to start executing in REPL (Read-Evaluate-Print-Loop) mode:

```shell
> ./li --repl
Welcome to lighzy-interpreter!
>>> 1 + 1
2
>>> let a = 2
>>> a
2
>>> println("Hello world!")
Hello world!
>>> var arr = [1, 2, 3, 4]
>>> len(arr)
4
>>> exit
```

Run `./li input.li` to interpret the input file:

```shell
> cat test.li
let sort = fun(array)
{
    var i = 0
    while (i < len(array) - 1)
    {
        var j = i + 1
        while (j < len(array))
        {
            if (array[i] > array[j])
            {
                array[i] += array[j]
                array[j] = array[i] - array[j]
                array[i] -= array[j]
            }
            ++j
        }
        ++i
    }

    return array
}

let nums = [32, 63, -12, 16, -21, 85, 12, 9, 11]
let sorted = sort(nums)
println(sorted)
> ./li test.li
[-21, -12, 9, 11, 12, 16, 32, 63, 85]
```

### Test

Run `ctest` in the folder `build` to start testing:

```shell
> cd build
> ctest
Test project /home/qtlittlexu/Documents/CMake Projects/lighzy-interpreter/build/Debug
      Start  1: LexerTest.parseToken
 1/39 Test  #1: LexerTest.parseToken ................   Passed    0.00 sec
      Start  2: ParserTest.toString
 2/39 Test  #2: ParserTest.toString .................   Passed    0.00 sec
      Start  3: ParserTest.operatorPrecedence
 3/39 Test  #3: ParserTest.operatorPrecedence .......   Passed    0.00 sec
      Start  4: ParserTest.LetStat
 4/39 Test  #4: ParserTest.LetStat ..................   Passed    0.00 sec
      Start  5: ParserTest.VarTest
 5/39 Test  #5: ParserTest.VarTest ..................   Passed    0.00 sec
      Start  6: ParserTest.ReturnStat
 6/39 Test  #6: ParserTest.ReturnStat ...............   Passed    0.00 sec
      Start  7: ParserTest.IdentifierExpr
 7/39 Test  #7: ParserTest.IdentifierExpr ...........   Passed    0.00 sec
      Start  8: ParserTest.IntegerExpr
 8/39 Test  #8: ParserTest.IntegerExpr ..............   Passed    0.00 sec
      Start  9: ParserTest.FloatExpr
 9/39 Test  #9: ParserTest.FloatExpr ................   Passed    0.00 sec
      Start 10: ParserTest.PrefixExpr
10/39 Test #10: ParserTest.PrefixExpr ...............   Passed    0.00 sec
      Start 11: ParserTest.InfixExpr
11/39 Test #11: ParserTest.InfixExpr ................   Passed    0.00 sec
      Start 12: ParserTest.BoolExpr
12/39 Test #12: ParserTest.BoolExpr .................   Passed    0.00 sec
      Start 13: ParserTest.IfExpr
13/39 Test #13: ParserTest.IfExpr ...................   Passed    0.00 sec
      Start 14: ParserTest.FunctionExpr
14/39 Test #14: ParserTest.FunctionExpr .............   Passed    0.00 sec
      Start 15: ParserTest.CallExpr
15/39 Test #15: ParserTest.CallExpr .................   Passed    0.00 sec
      Start 16: ParserTest.StringExpr
16/39 Test #16: ParserTest.StringExpr ...............   Passed    0.00 sec
      Start 17: ParserTest.AssignExpr
17/39 Test #17: ParserTest.AssignExpr ...............   Passed    0.00 sec
      Start 18: ParserTest.errorHandling
18/39 Test #18: ParserTest.errorHandling ............   Passed    0.00 sec
      Start 19: ParserTest.ArrayExpr
19/39 Test #19: ParserTest.ArrayExpr ................   Passed    0.00 sec
      Start 20: ParserTest.IndexExpr
20/39 Test #20: ParserTest.IndexExpr ................   Passed    0.00 sec
      Start 21: ParserTest.WhileStat
21/39 Test #21: ParserTest.WhileStat ................   Passed    0.00 sec
      Start 22: ParserTest.InDecrementExpr
22/39 Test #22: ParserTest.InDecrementExpr ..........   Passed    0.00 sec
      Start 23: EvaluatorTest.evaluateInteger
23/39 Test #23: EvaluatorTest.evaluateInteger .......   Passed    0.00 sec
      Start 24: EvaluatorTest.evaluateFloat
24/39 Test #24: EvaluatorTest.evaluateFloat .........   Passed    0.00 sec
      Start 25: EvaluatorTest.evaluateBool
25/39 Test #25: EvaluatorTest.evaluateBool ..........   Passed    0.00 sec
      Start 26: EvaluatorTest.evaluateIf
26/39 Test #26: EvaluatorTest.evaluateIf ............   Passed    0.00 sec
      Start 27: EvaluatorTest.evaluateReturn
27/39 Test #27: EvaluatorTest.evaluateReturn ........   Passed    0.00 sec
      Start 28: EvaluatorTest.evaluateError
28/39 Test #28: EvaluatorTest.evaluateError .........   Passed    0.00 sec
      Start 29: EvaluatorTest.evaluateDeclaration
29/39 Test #29: EvaluatorTest.evaluateDeclaration ...   Passed    0.00 sec
      Start 30: EvaluatorTest.evaluateFunction
30/39 Test #30: EvaluatorTest.evaluateFunction ......   Passed    0.00 sec
      Start 31: EvaluatorTest.evaluateCall
31/39 Test #31: EvaluatorTest.evaluateCall ..........   Passed    0.00 sec
      Start 32: EvaluatorTest.evaluateString
32/39 Test #32: EvaluatorTest.evaluateString ........   Passed    0.00 sec
      Start 33: EvaluatorTest.evaluateAssign
33/39 Test #33: EvaluatorTest.evaluateAssign ........   Passed    0.00 sec
      Start 34: EvaluatorTest.evaluateBuiltInFun
34/39 Test #34: EvaluatorTest.evaluateBuiltInFun ....   Passed    0.00 sec
      Start 35: EvaluatorTest.evaluateArray
35/39 Test #35: EvaluatorTest.evaluateArray .........   Passed    0.00 sec
      Start 36: EvaluatorTest.evaluateIndex
36/39 Test #36: EvaluatorTest.evaluateIndex .........   Passed    0.00 sec
      Start 37: EvaluatorTest.evaluateWhile
37/39 Test #37: EvaluatorTest.evaluateWhile .........   Passed    0.00 sec
      Start 38: EvaluatorTest.evaluateInDecrement
38/39 Test #38: EvaluatorTest.evaluateInDecrement ...   Passed    0.00 sec
      Start 39: stdlibTest.utils
39/39 Test #39: stdlibTest.utils ....................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 39

Total Test time (real) =   0.06 sec
```

## Development Progress

In developing, have finished:

### Expressions

- Prefix Expression
- Infix Expression
- If Statement
- Function
- Assignment

### Statements

- Variable Declaration
- Return Value
- While Statement

### Types of Data

- Integer
- Float
- Boolean
- String
- Array

### Standard Libraries

- `print(obj)`: Print object `obj` on the screen
- `println(obj)`: Print object `obj` and linebreak on the screen
- `len(obj)`:
    - `obj: string`: Return the length of string `obj`
    - `obj: array`: Return the length of array `obj`

## Contributing

Welcome to create issues and pull requests for the project. I am sooo happy to see it!

## License

The project is licensed under MIT license. See also [LICENSE](LICENSE).
