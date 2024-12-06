# Custom Shell Project

**Course:** COP4610: Operating Systems   
**Team Members:**  

- Alek
- Sayed
- Bilal

## Project Overview
This project is a custom shell implementation that includes several key features such as I/O redirection, piping, background processing, and a set of built-in functions. The shell supports both foreground and background execution of commands and includes the ability to manage environment variables, handle tilde expansion, and search for executables in the `$PATH`.

## Division of Labor

### Part 1: Prompt
**Responsibilities:**  
- Implemented a shell prompt that displays the format `USER@MACHINE:PWD>`.  
- Expands the `$USER`, `$PWD`, and `$MACHINE` environment variables to show the username, machine name, and current working directory in the prompt.  

**Assigned to:** All Members

### Part 2: Environment Variables
**Responsibilities:**  
- Replaced tokens prefixed with `$` (such as `$USER`, `$PATH`) with their corresponding values by using `getenv()`.  
- Handled these expansions universally across commands.

**Assigned to:** All Members

### Part 3: Tilde Expansion
**Responsibilities:**  
- Implemented tilde (`~`) expansion to replace `~` or `~/` with the user's home directory (`$HOME`).  
- Ensured correct handling for paths like `~/dir`.

**Assigned to:** All Members

### Part 4: $PATH Search
**Responsibilities:**  
- Implemented searching for executable commands in the directories specified by the `$PATH` environment variable.  
- If the command does not include a `/`, the shell searches `$PATH` for the executable.

**Assigned to:** All Members

### Part 5: External Command Execution
**Responsibilities:**  
- Used `fork()` and `execv()` to execute external commands in both foreground and background modes.  
- Ensured that arguments were passed correctly to the commands.

**Assigned to:** All Members

### Part 6: I/O Redirection
**Responsibilities:**  
- Implemented support for input/output redirection using `<`, `>`, and `>>`.  
- Handled both input and output redirection for commands.

**Assigned to:** All Members

### Part 7: Piping
**Responsibilities:**  
- Implemented piping functionality to allow the output of one command to be passed as input to another.  
- Supported up to two pipes (`cmd1 | cmd2 | cmd3`).

**Assigned to:** Alek

### Part 8: Background Processing
**Responsibilities:**  
- Implemented support for running commands in the background using `&`.  
- Managed job lists to track background processes and notify the user when they finish.

**Assigned to:** Sayed

### Part 9: Internal Command Execution
**Responsibilities:**  
- Implemented built-in commands such as `cd`, `jobs`, and `exit`.  
- Ensured proper handling of internal commands without invoking `execv()`.

**Assigned to:** Bilal

  
## File Listing

```plaintext
shell/
│
├── src/
│   ├── main.c           // Main shell loop and program entry
│   ├── shell_util.c     // Utility functions for the shell
│   ├── piping.c         // Piping implementation
│   ├── io_redirection.c // I/O Redirection implementation
│   ├── builtin.c        // Built-in command implementation
│   ├── background.c     // Background process management
│   ├── tokenizer.c      // Tokenizer for parsing input
│
├── include/
│   ├── shell.h          // Header file for shell utilities
│   ├── piping.h         // Header file for piping
│   ├── io_redirection.h // Header file for I/O Redirection
│   ├── builtin.h        // Header file for built-in commands
│   ├── background.h     // Header file for background processing
│   ├── tokenizer.h      // Header file for tokenization
│
├── README.md            // This README file
└── Makefile             // Makefile for compiling the shell
```

## How to Compile & Execute

### Requirements
- **Compiler**: `gcc` for C
- Ensure that the necessary libraries (such as `unistd.h`, `sys/wait.h`, etc.) are available on your system.

### Compilation
To compile the project, use the following command:

```bash
make
```

 ```bash
 ./bin/shell
 ```



## Bug
- **Issue with Background Processes**: There is currently a bug where, after running a command in the background (e.g., ls &), the shell may not immediately display the prompt, and the user may need to press Enter to get the prompt back.
