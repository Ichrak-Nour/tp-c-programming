# tp-c-programming
# 📚 Linguistic Data Processing — Set Theory in C

> **Authors:** Nourelichrak MEHSAS & Rihab HAMMADOUCHE  
> **Institution:** École Nationale Supérieure d'Informatique (ESI) — Algiers  
> **Language:** C  

---

## 📌 About the Project

This project implements a **Linguistic Data Processing** system based on **Set Theory** concepts. It allows the user to load text files, parse them into paragraphs and sentences, and perform classical set operations on them.

---

## 📁 File Structure

| File | Description |
|---|---|
| `main.c` | Main program — menu, user interaction, file loading |
| `library.c` | Core functions — parsing, display, search |
| `library.h` | Header for library functions |
| `abstract.c` | Set operations — union, intersection, difference, subset |
| `abstract.h` | Header for abstract/set functions |
| `file1.txt` | Sample text file 1 |
| `file2.txt` | Sample text file 2 |

---

## ⚙️ Features

- 📂 **Load multiple text files** (up to 10)
- 📋 **Display** all loaded paragraphs and sentences
- 🔢 **Set Operations** on paragraphs and sentences:
  - ∪ **Union** (A ∪ B)
  - ∩ **Intersection** (A ∩ B)
  - **Difference** (A - B)
  - **Subset check** (Is A ⊆ B?)
- 🔄 **Complement** of a paragraph within a file
- 🔍 **Search** for an expression inside a loaded file

---

## 🚀 How to Compile & Run

```bash
gcc main.c library.c abstract.c -o linguistic
./linguistic
```

---

## 🖥️ Main Menu

```
============================================
     ***    LINGUISTIC DATA PROCESSING  ***
============================================
  1. Load file(s)
  2. Display loaded files
  3. Set operations  (union / intersection / difference / subset)
  4. Complement operation
  5. Search expression in file
  0. Exit
```

---

## 🧠 Data Structure

The program uses **linked lists** to represent:
- `Paragraph` — a labeled group of sentences
- `sentence` — a single sentence within a paragraph

---

## 📊 Set Operations Explained

| Operation | Description |
|---|---|
| Union | All sentences that belong to A **or** B |
| Intersection | Sentences common to **both** A and B |
| Difference | Sentences in A but **not** in B |
| Subset | Checks if every element of A is in B |
| Complement | All sentences in the file **except** those in the selected paragraph |

---

*Made with ❤️ at ESI Algiers*
