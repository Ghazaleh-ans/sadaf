# 🐚 Sadaf - A Beautiful Shell

<div align="center">
  <img src="https://img.shields.io/badge/Language-C-blue.svg" alt="Language">
  <img src="https://img.shields.io/badge/Standard-POSIX-green.svg" alt="Standard">
  <img src="https://img.shields.io/badge/42-Minishell-purple.svg" alt="42 Project">
  <img src="https://img.shields.io/badge/Version-1.0-orange.svg" alt="Version">
</div>

<div align="center">
  <h3>🌟 As beautiful as a shell 🌟</h3>
  <p><em>A fully-featured UNIX shell implementation built from scratch</em></p>
</div>

---

## 📖 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Installation](#-installation)
- [Usage](#-usage)
- [Architecture](#-architecture)
- [Built-in Commands](#-built-in-commands)
- [Advanced Features](#-advanced-features)
- [License](#-license)

---

## 🚀 Overview

**Sadaf** is a comprehensive shell implementation that recreates the core functionality of bash. Named after the Persian word meaning "shell" or "seashell," this project demonstrates deep understanding of:

- **Process Management** - Fork, exec, wait, and signal handling
- **File Descriptors** - Pipes, redirections, and I/O management  
- **Memory Management** - Dynamic allocation and leak prevention
- **Parsing** - Complex command line interpretation
- **UNIX Philosophy** - Simple, composable tools

This shell provides a fully interactive command-line experience with advanced features like variable expansion, heredocs, and sophisticated error handling.

---

## ✨ Features

### 🎯 Core Shell Functionality
- **Interactive Prompt** with colored output and directory display
- **Command History** with readline integration
- **PATH Resolution** for executable lookup
- **Signal Handling** (Ctrl+C, Ctrl+D, Ctrl+\\) matching bash behavior
- **Exit Status Management** with `$?` expansion

### 🔧 I/O & Redirection
- **Input Redirection** (`<`) - Redirect file content to command input
- **Output Redirection** (`>`) - Redirect command output to file
- **Append Mode** (`>>`) - Append command output to file
- **Here Documents** (`<<`) - Multi-line input with delimiter
- **Pipes** (`|`) - Connect command output to next command input

### 🌐 Variable & Environment Management
- **Environment Variables** (`$VAR`) with full expansion
- **Exit Status** (`$?`) - Access last command's exit code
- **Tilde Expansion** (`~`) - Home directory shortcuts
- **Quote Handling** - Single and double quote processing

### 🔄 Process Control
- **Background Processes** (`&`) - Non-blocking command execution
- **Command Sequences** (`;`) - Sequential command execution
- **Process Isolation** - Proper fork/wait patterns

---

## 📥 Installation

### Prerequisites
- **GCC** or compatible C compiler
- **Make** build system
- **Readline** library (`libreadline-dev` on Ubuntu/Debian)

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/yourusername/sadaf-shell.git
cd sadaf-shell

# Compile the project
make

# Run the shell
./minishell
```

### Makefile Targets
```bash
make        # Build the executable
make clean  # Remove object files
make fclean # Remove all generated files
make re     # Rebuild everything
```

---

## 🎮 Usage

### Starting the Shell
```bash
./minishell
```

You'll see the beautiful prompt:
```
Sadaf 🐚 -> current_directory $ 
```

### Basic Commands
```bash
# File operations
ls -la
cat file.txt
echo "Hello, World!"

# Directory navigation
cd /path/to/directory
pwd
cd ~

# Environment management
export MY_VAR=value
echo $MY_VAR
unset MY_VAR
env
```

### Advanced Usage
```bash
# Pipes and redirection
ls -la | grep txt > output.txt
cat < input.txt | sort | uniq >> result.txt

# Here documents
cat << EOF
This is a multi-line
input example
EOF

# Background processes
sleep 10 &
long_running_command &

# Command sequences
make clean ; make ; ./program
```

---

## 🏗️ Architecture

### Core Components

```
┌─────────────────────────────────────────────────────────────┐
│                        Sadaf Shell                          │
├─────────────────────────────────────────────────────────────┤
│  Input Layer    │ Readline | History | Signal Handling     │
├─────────────────────────────────────────────────────────────┤
│  Parsing Layer  │ Tokenizer | Parser | AST Builder         │
├─────────────────────────────────────────────────────────────┤
│  Execution      │ Process Mgmt | I/O | Builtins | PATH     │
├─────────────────────────────────────────────────────────────┤
│  System Layer   │ Environment | Memory | Error Handling    │
└─────────────────────────────────────────────────────────────┘
```

### Key Design Patterns

- **Command Pattern** - Unified command structure for different types
- **Visitor Pattern** - AST traversal for execution
- **Factory Pattern** - Command object creation
- **RAII Pattern** - Automatic resource cleanup

---

## 🛠️ Built-in Commands

| Command | Description | Usage |
|---------|-------------|-------|
| `echo` | Display text with optional formatting | `echo [-n] text...` |
| `cd` | Change current directory | `cd [path]` |
| `pwd` | Print working directory | `pwd` |
| `export` | Set environment variables | `export VAR=value` |
| `unset` | Remove environment variables | `unset VAR` |
| `env` | Display environment variables | `env` |
| `exit` | Exit the shell | `exit [code]` |

### Command Features
- **echo**: Supports `-n` flag and quote processing
- **cd**: Updates PWD and OLDPWD environment variables
- **export**: Validates variable names and handles quotes
- **exit**: Proper cleanup and numeric argument validation

---

## 🚀 Advanced Features

### Variable Expansion
```bash
# Environment variables
echo $HOME $USER $PATH

# Exit status
false
echo $?  # Outputs: 1

# Tilde expansion
cd ~/Documents
ls ~/.bashrc
```

### Quote Processing
```bash
# Single quotes (literal)
echo 'Hello $USER'  # Outputs: Hello $USER

# Double quotes (expansion)
echo "Hello $USER"  # Outputs: Hello john

# Quote continuation
echo "This is a
multi-line string"
```

### Complex Pipelines
```bash
# Multi-stage pipelines
ps aux | grep python | awk '{print $2}' | xargs kill

# Redirection with pipes
curl -s api.example.com | jq . > data.json 2>&1
```

---

## 📜 License

This project is part of the 42 School curriculum and is intended for educational purposes. The implementation follows POSIX standards and bash compatibility where specified.

---

## 🙏 Acknowledgments

- **42 School** for the comprehensive project requirements
- **GNU Bash** for reference implementation behavior
- **POSIX** standards for shell specification
- **Readline Library** for input handling capabilities

---

## 👥 Authors

<div align="center">

### **Ghazaleh Ansari**
[![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white)](https://github.com/Ghazaleh-ans)

### **Mukhammad Ibrokhimov**
[![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white)](https://github.com/MukhammadIbrokhimov)

</div>

---

<div align="center">
  <p><strong>Built with ❤️ by the Sadaf Team</strong></p>
  <p><em>"As beautiful as a shell, as powerful as bash"</em></p>
</div>
