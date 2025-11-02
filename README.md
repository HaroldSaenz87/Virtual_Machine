# Virtual_Machine (PM/0)


## Overview 
This project implements a **P-machine (PM/0)** - a stack based virtual machine that executes PM/0 assembly instructions.

It simulates a simple low-level computer architecture with support for arthmetic operations, procedure calls, stack management, and I/O.

Developed entirely in **C**, the VM operates without dynamic memory allocation or pointer arithmetic, following the PM/0 model described in course materials.

---

# Features

- Full PM/0 instructions set
    - LIT
    - OPR
    - LOD
    - STO
    - CAL
    - INC
    - JMP
    - JPC
    - SYS
- Stack based execution model
- Activation record management
- Static and Dynamic links
- Instruction tracing with formatted stack output
- system operations for I/O and halting
---
# Execution

./vm input.txt

input.txt is the file containing PM/0 instructions.
