# SIMS — Student Information Management System

A command-line C program for COP 3515 (Advanced Programming, University of South Florida). Built incrementally, one weekly assignment ("Part") at a time — each part adds new functionality on top of the previous week's code, and each is restricted to only the C language features that had been taught in class by that point.

**Status: Parts 1–4 complete.** `main.c` is at version 4.0.

## What it does (as of Part 4)

- Takes in a student's ID, name, and GPA, with input validation (rejects bad IDs, blank names, out-of-range GPAs)
- Records five course grades and computes the average, highest, and lowest
- Classifies the student's GPA into an academic standing — Honors, Good Standing, Academic Probation, or Academic Suspension
- Prints a formatted Student Summary report combining all of the above
- Saves the full record to a text file, reads it straight back, and verifies the recovered data matches what was entered

## Build & run

```bash
gcc -Wall -Wextra -std=c11 -o sims main.c
./sims
```

Compiles clean with zero warnings under `-Wall -Wextra`.

## Part-by-part

| Part | Added | Status |
|---|---|---|
| [Part 1](Part%201) | Student ID/name/GPA intake, input validation, formatted summary | ✅ Submitted — scored 100% |
| [Part 2](Part%202) | 5 course grades, average/highest/lowest | ✅ Submitted |
| [Part 3](Part%203) | GPA-based academic standing classification | ✅ Submitted |
| [Part 4](Part%204) | Save/load the student record to a text file, with recovery verification | Built, evidence-packaged — due Sep 24 |

Each `Part N/` folder contains that week's spec PDF, a snapshot of `main.c` as it was submitted, and the test-evidence PDF (screenshots proving every required test case passes).

## Why the code looks the way it does

This is a teaching project — each week only permits the C features covered in class up to that point. Early parts intentionally avoid loops and functions even where they'd normally be the obvious tool, because those hadn't been introduced yet. Later parts build on top without rewriting what came before. The full reasoning behind every design decision, bug fix, and edge case is logged in [`CHANGELOG.md`](CHANGELOG.md) (what shipped each week and why) and [`SESSION_LOG.md`](SESSION_LOG.md) (a detailed dated record of the actual work).

## Structure

```
main.c            ← current, cumulative source (all parts combined)
CHANGELOG.md      ← per-part summary: requirements, implementation, bugs, tests
SESSION_LOG.md    ← detailed dated log of every work session
Part 1/, Part 2/, Part 3/, Part 4/   ← that week's spec PDF + submitted main.c snapshot + test evidence
Notes/            ← supplementary planning notes per part
```
