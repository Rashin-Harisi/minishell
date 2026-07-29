*This project has been created as part of the 42 curriculum by <rabdolho>, <pmoser>.*

## Description

Minishell is a 42 school project about the creation of a basic shell program written in C. It handles command execution, built-in commands (such as `cd`, `env`, `echo`, `exit`, `unset`, `export` and `pwd`), pipelines, redirections and environment variables expansions.

## Instructions

### Usage

Clone the Repository and compile with:

- `make`        --> compiles minishell
- `make clean`  --> removes object files
- `make fclean` --> removes all files created by `make`
- `make re`     --> removes all files created by `make`, then recompiles

This produces a 'minishell' executable, which can be run with:

```bash
./minishell
```

### Prerequisites

The project requires the `readline` library to be installed:

- Debian/Ubuntu: `sudo apt install libreadline-dev`
- macOS: `brew install readline`

## Resources
* <a href="https://www.gnu.org/software/bash/manual/bash.html?utm_source=chatgpt.com#Shell-Syntax"> Bash Features</a>
* <a href="https://tldp.org/LDP/Bash-Beginners-Guide/html/index.html"> Bash Guide for Beginners</a>
AI Usage: AI was used for Debugging and reviewing edgecases. 