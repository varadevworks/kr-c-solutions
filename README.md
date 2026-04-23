# kr-c-solutions

My solutions to exercises from *The C Programming Language* by Kernighan and Ritchie.

## 📁 Structure

Solutions are grouped by chapter under `exercises/`.

Example layout:

```text
exercises/
  chapter-01/
    ex-18.c
    ex-19.c
  chapter-02/
  chapter-03/
```

Each file is named by exercise number, such as `ex-18.c` or `ex-19.c`.

## 📚 Solutions

| Chapter   | Folder                                         |
| --------- | ---------------------------------------------- |
| Chapter 1 | [exercises/chapter-01/](exercises/chapter-01/) |

## 🛠️ Build

Build all programs:

```sh
make
```

Build one program:

```sh
make ex-19
```

## ▶️ Run

Run a program:

```sh
make run FILE=ex-19.c
```

Run a program with input from a file:

```sh
make run FILE=ex-19.c INPUT=input.txt
```

## 📝 Notes

These solutions were written and tested on macOS.
