# Dynamic Function Generator

Runtime function generator written in C for Linux x86-64.

## About

This project implements a dynamic function generator capable of
creating machine code at runtime.

The generated function acts as a wrapper around an existing C
function, dynamically configuring how its arguments are obtained.

## Features

Supports functions with up to three parameters.

Each parameter can be:

- PARAM — received by the generated function
- FIX — fixed when the function is generated
- IND — loaded indirectly from a memory address

Supported parameter types:

- Integer
- Pointer

## Example

Given:

int soma3(int a, int b, int c);

We can configure:

a → PARAM
b → FIX(100)
c → PARAM

The generated function behaves approximately as:

f(x, y) → soma3(x, 100, y)

For:

f(1, 2)

The result is:

103

## How it works

The generator emits x86-64 machine instructions directly into
a memory buffer.

It manipulates the argument registers according to the
System V AMD64 calling convention:

RDI → first argument
RSI → second argument
RDX → third argument

After configuring the arguments, the generated code calls
the original function and returns its result.

## Build

make

## Run tests

make test

## Project structure

src/        Source code
include/    Header files
tests/      Tests
docs/       Test documentation

## Tests

Detailed test results are available in:

docs/test-report.md

## Concepts explored

- C
- x86-64
- Machine code
- CPU registers
- Function pointers
- Memory addressing
- Calling conventions
- Runtime code generation

## Academic context

Originally developed as a Basic Software university assignment.
