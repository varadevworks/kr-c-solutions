# kr-c-solutions

My solutions to exercises from *The C Programming Language Second Edition* by Kernighan and Ritchie.

## 📁 Structure

Solutions are grouped by chapter under `exercises/`.

Example layout:

```text
exercises/
  chapter-01/
    ex-01-18.c
    ex-01-19.c
  chapter-02/
  chapter-03/
```

Each file is named by chapter and exercise number, such as `ex-01-18.c` or `ex-01-19.c`.

## 📚 Solutions

| Chapter   | Folder                                         |
| --------- | ---------------------------------------------- |
| Chapter 1 | [exercises/chapter-01/](exercises/chapter-01/) |
| Chapter 2 | [exercises/chapter-02/](exercises/chapter-02/) |
| Chapter 3 | [exercises/chapter-03/](exercises/chapter-03/) |
| Chapter 4 | [exercises/chapter-04/](exercises/chapter-04/) |
| Chapter 5 | [exercises/chapter-05/](exercises/chapter-05/) |
| Chapter 6 | [exercises/chapter-06/](exercises/chapter-06/) |
| Chapter 7 | [exercises/chapter-07/](exercises/chapter-07/) |
| Chapter 8 | [exercises/chapter-08/](exercises/chapter-08/) |

## ▶️ Run

### Using `make`

| Action              | Command                                         |
| ------------------- | ----------------------------------------------- |
| Build all           | `make`                                          |
| Build one           | `make ex-01-19`                                 |
| Build & run         | `make run FILE=ex-01-19.c`                      |
| Run with stdin file | `make run FILE=ex-01-19.c INPUT=test/input.txt` |
| List programs       | `make list`                                     |
| Clean build         | `make clean`                                    |

### Using VS Code (CodeLLDB)

Requires the **CodeLLDB** extension. Open any `.c` file under `exercises/` and use:

| Step        | Action                                                                               | Details                                                                                         |
| ----------- | ------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------------- |
| Build       | Command Palette (`⇧⌘P`) → `Tasks: Run Build Task`                                    | Runs default task **`Build active file`** (`clang` → `build/<name>`) from `.vscode/tasks.json`  |
| Debug / Run | Command Palette (`⇧⌘P`) → `Debug: Select and Start Debugging` → select configuration | Runs `preLaunchTask` (**`Build active file`**) then launches via `lldb` (`.vscode/launch.json`) |

Available `launch.json` configurations:

| Configuration                          | Input / Args                                         |
| -------------------------------------- | ---------------------------------------------------- |
| `Launch: no args (terminal stdin)`     | Type stdin in integrated terminal (`Ctrl+D` for EOF) |
| `Launch: prompt for args`              | Prompts for space-separated `progArgs`               |
| `Launch: prompt for stdin file`        | Prompts for full path `stdinFullPath`                |
| `Launch: prompt for stdin file + args` | Prompts for full path `stdinFullPath` and `progArgs` |

## 📝 Notes

These solutions were written and tested on macOS.