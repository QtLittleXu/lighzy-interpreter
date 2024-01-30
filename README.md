# Lighzy 解释器

## 介绍

lighzy-interpreter 是专门用来解释 Lighzy 语言的跨平台解释器，其通过对源代码一行一行地解释以实现运行。

## 构建和运行

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