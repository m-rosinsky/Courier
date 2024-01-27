# Courier
Courier is an original toy compiled language written for the purposes of practicing compiler theory.

Check out the `dev_log.md` file, where I document my thought process throughout the development of this language.

I am developing this language as I take the Stanford University [Compiler Theory](https://learning.edx.org/course/course-v1:StanfordOnline+SOE.YCSCS1+3T2020/home), as a means of self-study as the course progresses.

The course is ultimately broken down into 6 parts:

1. Lexical Analysis
2. Parsing
3. Semantic Analysis and Type Checking
4. Code Generation & Operational Semantics
5. Optimization
6. Register Allocation & Garbage Collection

As I complete each part of the course, I intend to implement what I learn into this language.

My repo for the course, including notes, assignments, etc, is in my repo linked [here](https://github.com/m-rosinsky/stanford-compiler)

## Language Details

Since this course is on compiler theory, Courier is a compiled language.

I intend for it to syntactically be a hybrid between C++ and Python, hopefully taking a bit of the best aspects from both.

## Syntax

### Semicolons

Courier uses semicolons to denote the end of a statement, similar to C++.

### Scoping

Courier will use similar scoping rules to C++, as well as the same method of creating scopes, in the form of the `{` and `}` characters.

### Main Function

Courier will not implement a `main` function. Instruction execution will simply work from top-to-bottom.

## Type Checking

Courier will have strict type checking. Types must be declared during the creation of variables, and all casts must be explicit

For example, if a function's parameter calls for an `int`, and a `float` is passed, this should be caught at compile time.

## Sample Code

TODO
