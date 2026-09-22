# Part 4 — Save and Load to a Text File

Spec: [[../Part 4/Project 1 - Part 4.pdf|Project 1 - Part 4.pdf]]
Code snapshot: [[../Part 4/Project 1 Part 4/main.c|main.c]]
Data file: [[../Part 4/Project 1 Part 4/student_records.txt|student_records.txt]]
Evidence: built — [[../Part 4/Project 1 Part 4/Project 1 Part 4 - Test Evidence.pdf|Project 1 Part 4 - Test Evidence.pdf]] (14 pages). Submission ZIP built too.

## Deadline

**Thursday Sep 24, 2026**, confirmed by me. The spec header says "Due Date:
Beginning of Lesson 10." Lesson 7 = Wed Sep 16 (loops), Lesson 8 = Thu Sep 17
(functions), so Lesson 10 lands on Thu Sep 24. That lines up.

## Summary

Version bumps to 4.0. The program now saves the student record to
`student_records.txt`, reads it straight back, prints a Recovered Student
Record, and tells me whether the recovered data matches what I entered.

File format is one value per line, no blank lines:

```
1234567          <- Student ID
Alice Johnson    <- Student Name
3.84             <- GPA
Honors           <- Academic Standing
95 88 91 84 100  <- the five grades, one per line
```

Verified byte-identical to the spec's Example 2 expected contents.

### What changed in the code

1. `VERSION_NUMBER` → `"4.0"`, new `DATA_FILE_NAME` constant.
2. The Academic Standing switch now fills a single `standingText` buffer once.
   It is needed in three places (screen, file, verification), and three
   separate switches would be free to drift apart.
3. Write with `fopen(..., "w")`, nine `fprintf` lines, checked `fclose`.
4. Read with `fgets` line by line, `sscanf` to convert, every read checked.
5. Average/highest/lowest are **recomputed** from the recovered grades — the
   file only stores the five raw grades, never the derived values.
6. Field-by-field comparison sets a `recordsMatch` flag and prints the verdict.

### Still no loops and no functions

Part 4's Out of Scope list does not ban loops, and both loops and functions
have now been taught. But functions ARE banned by name, and loops are absent
from the permitted list, so the code stays straight-line and uses neither.
Safe under every reading. `strcmp`, `strcpy`, `fopen` etc. are standard library
calls, not functions I wrote.

## Bugs found and fixed (2026-09-22)

Both were found by testing, not by reading the code.

### 1. A failed save was reported as a success

`fclose` was called without checking the result, and the program printed
"Student information successfully saved." no matter what. That is wrong,
because `fprintf` only fills a buffer — the data actually reaches the disk when
the file is closed. A failure at that moment (disk full, quota, device
removed) happens *after* every `fprintf` has already succeeded.

Reproduced by pointing `student_records.txt` at a device that accepts writes
but always fails to flush. The old build printed "successfully saved", read the
file back, printed **garbage from uninitialised memory** (`Highest Grade :
1500501712`), and exited with a **success** status.

Fixed by checking `fclose` and only claiming success when it returns 0.

### 2. Unchecked `sscanf` on every numeric line

The return value was ignored, so a line that was present but not a number left
the recovered variable uninitialised and the program carried on using whatever
was in memory. Now each conversion is checked and a bad line is reported by
name, e.g. `student_records.txt contains invalid data (Student ID).`

### Also changed, not a bug

The write-failure message used different wording from the read-failure one.
The spec gives **one** error block for "the data file cannot be opened"
(Example 3), so both paths now print that exact three-line text.

## File error behaviour — verified, not assumed

| Situation | Actual behaviour | Exit |
|---|---|---|
| Cannot open for **writing** | `ERROR` / `Unable to open student_records.txt` / `Please verify that the file exists and that you have permission to access it.` | 1 |
| Cannot open for **reading** | same three lines, exact spec Example 3 wording | 1 |
| Save fails at flush | `ERROR` / `Unable to save student_records.txt` / `The file may be incomplete...` | 1 |
| File empty at read | `ERROR` / `student_records.txt is missing or incomplete.` | 1 |
| Line present but not a number | `ERROR` / `student_records.txt contains invalid data (Course N Grade).` | 1 |

## ⚠️ The "missing file" regression test needs care

Required regression test 5 is *"Attempt to read a missing file."* **With this
design that cannot happen in a normal run**, because the program always writes
the file immediately before reading it, and `"w"` mode creates it. So the file
is never missing at read time.

The reachable equivalent is **the file cannot be opened for reading**, which is
the same code path and prints the exact error the spec's Example 3 asks for. To
demonstrate it: create `student_records.txt` with no read permission before
running. See the shot list.

This needs saying plainly in the assumptions section of the evidence PDF, not
glossed over.

## Recovered report format — a known difference from the spec example

The spec's Example 1 shows the recovered Course Grades as bare numbers:

```
Course Grades
95
88
```

My program prints `Course 1 : 95` instead, matching the original Student
Summary exactly. That is a deliberate answer to Question for the Customer #6
("should the recovered report exactly match the original report?" — yes), and
the Acceptance Criteria only asks that the recovered report be "neatly
formatted". Same call I made in Part 2 and Part 3: Acceptance Criteria over
worked example.

**But a grader diffing against Example 1 will see it.** It is written up in the
assumptions.

**Confirmed 2026-09-22:** Branson reviewed this and chose to keep it as-is,
matching the same "Acceptance Criteria over worked example" call made in
Parts 2 and 3. Not changing before submitting.

## Submission requirements — note the third file

The ZIP needs **three** things this week, not two:

- `main.c`
- any additional source files
- **`student_records.txt` generated by the program**

Plus the PDF with execution screenshots, **file contents screenshots**,
the five regression tests, bugs, customer answers, and assumptions.

ZIP structure stays the `Project 1 Part 4/` wrapper — confirmed acceptable
since Part 1 scored 100% with it.

## Verify before submitting

1. Read the whole evidence PDF; the customer answers are in my voice.
2. Be able to explain the `fclose` bug out loud — it is the interesting one.
3. Decide on the recovered-report format difference above.
4. Confirm the due date on Canvas.
5. **Push to GitHub.** The Desktop repo at `~/Desktop/Advanced Program/Project/`
   is still on **v3.0** and has no Part 4 at all. Nothing auto-syncs.

Back to [[SIMS Project]]
