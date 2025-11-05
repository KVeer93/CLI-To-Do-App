# CLI To-Do App

A simple **command-line to-do list application** written in C++. Tasks are stored in a JSON file (`tasks.json`) so your data persists between sessions. You can add, list, mark done, and clear tasks — all via the terminal.

---

## Features

- Add a single task
- Add multiple tasks at once
- List tasks with status (Done / Pending) and date
- Mark tasks as done (individually or all)
- Clear all tasks
- Fully CLI-based, no GUI

---

## Usage

Run the executable and type commands:

```bash
> add Buy Milk
> multi "Walk Dog" "Finish Project" "Call Mom"
> list
> done 1 3
> done all
> clear
> help
> exit
```

## Installation
Clone the repo:
```bash
git clone https://github.com/KVeer93/CLI-To-Do-App.git
```

Build the app (example with g++):
```bash
g++ -std=c++17 main.cpp -o tasks
```

Run the executable:
```
./tasks
```
---

## Notes

– Tasks are saved in tasks.json.
– Colored output supported for status (✔ Done / ⧗ Pending).
– Designed for learning and practicing C++ CLI + file handling + JSON.

---
