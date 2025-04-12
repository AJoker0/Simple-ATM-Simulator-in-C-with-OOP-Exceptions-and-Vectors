# 💶 C++ ATM Simulator

A console-based ATM simulation in **C++**, built as part of a university assignment.  
This project demonstrates object-oriented programming, exception handling, STL containers (`vector`, `tuple`), and basic command-line interaction.

---

## 📋 Features

- ✅ Load banknotes into ATM boxes with different denominations (10, 20, 50, 100 EUR)
- ✅ Withdraw up to 3000 EUR using available denominations
- ✅ View current ATM box status and total cash
- ✅ Uses C++ classes, no traditional C-style arrays or standalone functions
- ✅ Exception handling for invalid inputs and edge cases

---

## 📦 Structure

- `Box` class: represents an individual banknote box
- `ATM` class: manages all ATM logic and user commands
- `main()`: entry point that reads commands and delegates execution

---

## 💻 Commands

| Command        | Description                               |
|----------------|-------------------------------------------|
| `L#box#nominal#count` | Load banknotes into a box            |
| `W#amount`     | Withdraw money                            |
| `P`            | Print current status of all boxes         |

---

## 🧪 Example

```bash
> L#1#100#10
> L#2#50#20
> L#3#20#30
> L#4#10#40
> W#340
