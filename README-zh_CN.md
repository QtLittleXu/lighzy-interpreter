# Lighzy 解释器

## 翻译

[English](README.md)  
[中文](README-zh_CN.md)  

## 介绍

lighzy-interpreter 是专门用来解释 Lighzy 语言的跨平台解释器，其通过对源代码一行一行地解释以实现运行。

## 构建、运行和测试

### 环境

本项目使用 CMake 构建系统，需要安装以下库，或使用像 vcpkg 这样的包管理工具安装：

- argparse (required, for arguments parsing)
- gtest (optional, for test)

### 构建

在命令行运行以下命令构建：

```shell
cmake -B build -S . -D BUILD_TYPE=Release -D BUILD_TEST=ON
cmake --build build --config Release
```

最终在 `build/bin` 目录中生成解释器的可执行文件 `li`。

### 运行

跳转到 `build/bin` 目录，执行 `./li --help` 获取帮助：

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

执行 `./li --repl` 进入行对行解释模式：

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

执行 `./li input.li` 解释输入文件：

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

## 测试

跳转到 `build` 目录，执行 `ctest` 开始测试：

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

## 开发进度

正在开发，尽请期待，已完成：

### 表达式

- 前缀
- 中缀
- if 判断
- 函数
- 赋值

### 语句

- 变量定义
- 返回值
- While 循环

### 数据类型

- 整数
- 浮点数
- 布尔
- 字符串
- 数组

### 标准库

- `print(obj)`: 打印 `obj` 对象到屏幕上
- `println(obj)`: 打印 `obj` 对象到屏幕上并换行
- `len(obj)`:
	- `obj: string`: 返回 `obj` 字符串的长度
	- `obj: array`: 返回 `obj` 数组的长度

## 贡献

欢迎为这个项目提交 PR 或 issue，我会非常高兴看到它！

## 许可协议

该项目在 MIT 协议下分发，另见 [LICENSE](LICENSE)。
