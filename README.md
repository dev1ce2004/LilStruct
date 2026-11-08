# LilStruct

Data structures exercises in C, written for a university data structures course at FESB Split. The task list (in Croatian) is in [`task.txt`](task.txt).

## What's inside

| Folder | Exercise |
| --- | --- |
| `v1/` | Read student records from a file into a dynamically allocated array of structs; print absolute and relative scores |
| `v4/` | Add and multiply polynomials read from files (linked lists, unsorted exponents) |
| `v5/` | Evaluate a postfix expression from a file using a stack built on a linked list |
| `v6/` | Receipts: a date-sorted linked list of receipts, each holding a name-sorted list of items, with spending queries |
| `lilproject_reupload/` | The receipts project restructured into `src/`, `include/` and `data/` |
| `old_ver/` | An earlier version of the receipts project |

Folders `v2/` and `v3/` hold further exercises from the same task list.

## Build and run

Each exercise is plain C with no external dependencies. For example:

```bash
cd v5
gcc vjezbica5.c -o vjezbica5
./vjezbica5
```

For the multi-file project:

```bash
cd lilproject_reupload
gcc -Iinclude src/*.c -o lilproject
./lilproject
```

Run programs from their own folder so they find their input `.txt` files.

## Topics covered

Dynamic memory allocation, structs, singly linked lists (insert, delete, search, sort), stacks, polynomial arithmetic, file I/O, and nested linked lists.
