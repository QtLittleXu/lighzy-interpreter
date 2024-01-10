# Lighzy 解释器

## 介绍

lighzy-interpreter 是专门用来解释 Lighzy 语言的跨平台解释器，其通过对源代码一行一行地解释以实现运行。

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
