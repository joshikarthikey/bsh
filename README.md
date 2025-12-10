# BSH (Better Shell History)

![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg) ![Language: C++20](https://img.shields.io/badge/Language-C%2B%2B20-orange) ![Platform: Linux%20%7C%20macOS](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS-lightgrey)

**A High-Performance, Git-Aware, Predictive Terminal History Tool**

## 1. Executive Summary
BSH (Better Shell History) is a command-line interface (CLI) middleware designed to modernize the terminal experience by replacing standard flat history files (e.g., `.bash_history`) with a structured, local SQLite database.

Unlike traditional shells, BSH introduces a **"Live Predictive" TUI** (Text User Interface) that mimics IDE IntelliSense. It offers context-aware suggestions based on the current directory and Git branch as the user types, improving retrieval speed and reducing cognitive load.

---

## 2. Problem Statement
Standard shell history mechanisms (Bash/Zsh) suffer from three critical limitations in modern development workflows:

1.  **Context Blindness:** History is environment-agnostic. Commands executed within specific Git feature branches are intermixed with global commands, complicating the retrieval of branch-specific operations.
2.  **Reactive Searching:** Users are forced to actively initiate searches (e.g., `Ctrl+R`). There is no passive discovery or auto-completion based on historical frequency.
3.  **Pollution:** Failed commands (typos or non-zero exit codes) clutter search results, reducing efficiency.

---

## 3. Technical Architecture

### 3.1 Technology Stack
BSH is engineered for minimal latency (<5ms) using Modern C++ to ensure compliance with performance-critical CLI environments.

| Component | Library/Tool | Justification |
| :--- | :--- | :--- |
| **Core Language** | C++17 / C++20 | High performance and low latency. |
| **Database** | SQLite3 | Serverless, reliable, and supports concurrency. |
| **DB Wrapper** | SQLiteCpp | RAII wrapper for memory safety. |
| **TUI Renderer** | FTXUI | Rendering the "Live Prediction" overlay. |
| **Context Utils** | libgit2 | Efficient resolution of Git branch context. |

### 3.2 Data Model
BSH utilizes a relational schema to optimize storage and query performance.

* **`commands` Table:** Stores unique command strings to prevent redundancy.
* **`executions` Table:** Tracks the execution timeline, including:
    * **Session ID & Timestamp**
    * **CWD (Current Working Directory)**
    * **Git Branch:** Commands are tagged with the active branch.
    * **Exit Code:** Enables filtering of failed commands.
    * **Duration:** Execution time in milliseconds.

---

## 4. Key Features

### Live Predictive TUI
Integrated via the Zsh Line Editor (ZLE), BSH intercepts keystrokes to display a "Top 10" relevance list below the cursor in real-time.

### Deep Git Integration
BSH treats Git branches as distinct contexts. Users can filter history specifically by the current branch, solving the "Context Blindness" issue inherent in flat files.

### Success Filtering
A toggleable filter excludes commands with non-zero exit codes (`Exit Code ≠ 0`), ensuring that search results contain only valid, successful operations.

### Zero-Cloud Privacy
BSH operates with 100% local execution using C++ and SQLite. No data is transmitted to external servers, ensuring compliance with strict enterprise security policies.

---

## 5. Usage & Controls

The BSH TUI provides the following interaction modes:

| Key Binding | Action |
| :--- | :--- |
| **`Enter`** | Executes the Top (0th) suggestion or the current buffer if selected. |
| **`Alt` + `0-9`** | Instantly executes the corresponding suggestion from the Top 10 list. |
| **`Ctrl` + `Arrows`** | Cycles search context: **Global** $\leftrightarrow$ **Directory** $\leftrightarrow$ **Git Branch**. |
| **`Alt` + `S`** | **Toggle Success Filter**: Show/hide failed commands (Exit code ≠ 0). |

---

## 6. Build Instructions

### Prerequisites
* C++ Compiler (GCC 9+ or Clang 10+)
* CMake (3.15+)
* Dependencies: `libsqlite3`, `libgit2`

### Installation
```bash
git clone [https://github.com/username/bsh.git](https://github.com/username/bsh.git)
cd bsh
mkdir build && cd build
cmake ..
make
sudo make install