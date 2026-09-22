# SIMS (Student Information Management System) — Changelog

Cumulative Project 1 for COP 3515. Each entry is one Part, building directly on
the previous week's `main.c`. This log is specific to this project only —
unrelated to the separate weekly standalone CCR series (Tampa Fitness, Sunrise
Coffee, Grand Cinema, etc.), which uses the word "CCR" for a different, unrelated
numbering scheme. Do not cross-reference content between the two.

## Part 1 (2026-09-08) — Initial prototype

**Requirements:** Display course/software title, version, programmer name, and a
welcome banner. Prompt for Student ID (int), Student Name (string), Current GPA
(float). Display a formatted "Student Summary." Single student only — no arrays,
functions, loops, or file I/O per this week's constraints.

**Implementation:** [main.c](main.c) — banner via `#define` constants, `scanf`/`fgets`
for input, `%.2f` formatting for GPA.

**Bug fixed:** Prompts appeared jumbled together when run in VS Code (e.g. via the
Code Runner extension) instead of a real terminal — caused by `stdout` switching to
full buffering when it isn't a TTY, so prompt text sat in the buffer instead of
printing before `scanf`/`fgets` blocked for input. Fixed by adding `fflush(stdout);`
immediately after each prompt `printf`.

**Tests:** All three PDF-required cases pass exactly (Alice Johnson 3.84 / Michael
Brown 2.91 / Christopher Williams 4.00). Compiles clean with `-Wall -Wextra`.

**Input validation added (still Part 1, refinement):** Student ID and Current GPA
are now rejected (clear error message, program exits) if they contain letters or
trailing garbage (e.g. `"abc"`, `"123abc"`), if Student ID is negative, or if GPA
falls outside 0.0–4.0. Student Name is rejected if left blank. All checks use a
single `if` per condition — no loops or functions, since Part 1's spec explicitly
puts both out of scope this week. Rejecting digits embedded in the Student Name
(e.g. `"Al1ce"`) would require scanning an arbitrary-length string character by
character, which needs a loop or function — not possible within this week's
constraints, so that specific check is intentionally not implemented. Re-verified
all three required test cases still pass after adding these checks.

## Part 2 (2026-09-09) — Course grades, average/highest/lowest

**Requirements:** Continue all Part 1 functionality. Add five course grades
(0–100 each), store them, calculate and display Average Grade (2 decimals),
Highest Grade, and Lowest Grade, alongside the existing Student Summary.
Single student only. Newly permitted this week: arrays, enumerations,
selection statements. Still out of scope: functions, loops, files, multiple
students, menus, searching, editing grades, letter grades, academic standing,
dynamic memory.

**Implementation:** [main.c](main.c) — version bumped to 2.0. `courseGrades[5]`
array (size named via `enum { NUM_COURSES = 5 }`, a newly-permitted feature).
Average/highest/lowest computed with straight-line arithmetic and five
individual `if` comparisons each — no loop or function, since both remain out
of scope. Each of the 5 grades gets the same validation treatment as Part 1's
numeric fields: reject non-numeric input, trailing garbage, and out-of-range
values (0–100), printing a clear error and exiting rather than re-prompting.

**Judgment call, flagged and resolved with user:** Part 2's own worked example
output omits the `Programmer:` line, but Part 2's Acceptance Criteria requires
"all functionality from CCR-001" (Part 1) to keep working, which included that
line. User decided: keep it, since the Acceptance Criteria is the binding
requirement and the example most likely just carried an omission forward from
editing the template.

**Answers to the PDF's "Questions for the Customer"** (required in this
part's submission PDF): grades are whole numbers 0–100 (every worked example
uses whole numbers); no fractional grades; invalid grades print an error and
exit rather than re-prompt, matching Part 1's approach (loops aren't available
for a retry); courses have no names, only grades (the examples only ever
label them "Course 1"–"Course 5"); average is rounded to 2 decimal places
(matches 91.60/79.00/100.00 in the examples); a tied highest or lowest grade
is simply displayed once, since the Acceptance Criteria asks only for the
grade value, not which course earned it, and identifying the matching course
would effectively be a "searching" feature — explicitly out of scope; more
than five courses is not addressed this week (fixed at five per this week's
spec) and would need a future CCR.

**Tests:** All three required regression cases pass exactly — Alice Johnson
(avg 91.60, high 100, low 84), Michael Brown (avg 79.00, high 91, low 68),
Christopher Williams (avg 100.00, high 100, low 100). Compiles clean with
`-Wall -Wextra`. Adversarially tested: letters/trailing garbage/out-of-range
on a course grade, and boundary grades of exactly 0 and 100 (avg computed
correctly as 50.00). Re-confirmed Part 1's own invalid-input rejections
(bad ID, blank name) still work unchanged.

## Part 3 (2026-09-16) — Academic standing classification

**Requirements:** Continue all Part 1 and Part 2 functionality. Validate the
GPA before determining academic standing, reject GPAs outside 0.00–4.00 with
an error and no standing, classify valid GPAs into Honors (3.50–4.00), Good
Standing (2.00–3.49), Academic Probation (1.00–1.99), or Academic Suspension
(0.00–0.99), and display the standing as part of the student report. Newly
permitted this week: `switch` statements and logical operators. Still out of
scope: functions, files, menus, multiple students, editing grades, automatic
GPA calculation, degree audits, transcripts, dynamic memory. Loops are not on
this part's permitted list either.

**Implementation:** [main.c](main.c) — version bumped to 3.0. Added a named
`enum AcademicStanding` with four constants. Classification is an if/else-if
cascade on `currentGPA` placed immediately after the existing Part 1 range
check, so validation still happens before any standing is determined. Display
is a `switch` on the enum, printing the `Academic Standing :` line directly
under `Current GPA` in the Student Summary. No loops, no functions.

**Additive part — nothing from Parts 1 or 2 was rewritten.** The unrolled `if`
comparisons for highest/lowest, the straight-line average, and the
exit-on-invalid validation all stayed as they were, since loops and functions
remain out of scope.

**Stale file corrected before starting:** the root `main.c` and the Part 2
snapshot had drifted apart. Both were 178 lines, but the root copy still
printed the Student Summary *before* the course grade prompts, while the
snapshot carried the Part 2 fix that moved it below (see Part 2's entry). The
snapshot was 20 hours newer and correct. Copied the snapshot over the root
`main.c` so Part 3 builds on the right base and the Part 2 formatting fix does
not silently regress.

**Why the `switch` tests the enum and not the GPA:** `switch (currentGPA)`
does not compile — C requires an integer expression in a switch, and
`currentGPA` is a `float`. The if/else cascade does the part a switch cannot
do (compare a float against ranges) and reduces the GPA to one of four enum
values; the switch then selects the label text. This also answers the spec's
customer question about representing standing as predefined categories rather
than loose text.

**Assumptions logged (Part 3 newly requires an assumptions section in the
submission PDF):**
1. *Band gaps.* The published bands jump from 3.49 to 3.50 and from 1.99 to
   2.00, leaving values like 3.495 unclassified. Implemented as a cascade of
   `>=` lower bounds only, so every valid GPA in 0.00–4.00 lands in exactly
   one category and the band limits are inclusive.
2. *Kept the `Programmer:` line and `Welcome to SIMS` banner.* Part 3's worked
   example omits both, but the Acceptance Criteria still requires all CCR-001
   functionality to keep working, and CCR-001 required the banner. Same
   judgment call made for Part 2 — Acceptance Criteria wins over the example.
3. *Invalid GPA exits rather than re-prompting.* Customer questions 3 and 4
   raise this; a retry needs a loop, which is out of scope. Kept the Part 1 /
   Part 2 behavior.

**Tests:** Compiles clean with `gcc -Wall -Wextra -std=c11` — zero warnings.
All six required standing cases pass: 3.95 Honors, 3.20 Good Standing, 1.75
Academic Probation, 0.60 Academic Suspension, -0.50 rejected, 4.25 rejected.
Both rejections print the exact three-line message from the spec's Example 5
and exit before requesting course grades. All three Part 2 regression cases
still pass exactly — Alice Johnson (3.84, Honors, avg 91.60, high 100, low
84), Michael Brown (2.91, Good Standing, avg 79.00, high 91, low 68),
Christopher Williams (4.00, Honors, avg 100.00, high 100, low 100).
Additionally spot-checked every band boundary and the gap value: 4.00 and 3.50
Honors, 3.49 and 3.495 and 2.00 Good Standing, 1.99 and 1.00 Academic
Probation, 0.99 and 0.00 Academic Suspension.

**Bug found during testing and corrected (2026-09-16):** the report printed the
GPA with `%.2f` (rounded to two decimals) but classified using the raw
unrounded value, so the two could contradict each other. Entering `1.999`
printed `Current GPA : 2.00` alongside `Academic Standing : Academic
Probation` — but 2.00 is Good Standing. The display rounded up across the band
edge and the classification did not follow. Fixed by rounding once into
`gpaRounded` (`((int)(currentGPA * 100.0f + 0.5f)) / 100.0f`, the same cast
technique already used for the Part 2 average) and using that single variable
for both classification and display, so the printed GPA and the printed
standing cannot disagree. Re-ran every test afterward: `1.999` now reports
2.00 / Good Standing, and no required case or Part 2 regression case changed.
Side effect, intentional: `3.495` now rounds to 3.50 and classifies as Honors,
which is self-consistent with what it displays.

**Deadline correction (2026-09-16):** the spec header says "Due Date: Beginning
of Lesson 8," which under the lesson-to-date map previously recorded in the
project `CLAUDE.md` would have been Thu Sep 24. Branson confirmed the real due
date is **Thu Sep 17**, the same day as Part 2. The lesson map is therefore
wrong and has been flagged in `CLAUDE.md` for correction from Canvas — future
part deadlines should not be derived from it until it is fixed.

**Submission package built:** `main.c` snapshot refreshed into
`Part 3/Project 1 Part 3/`, evidence PDF written to the same folder
(`Test Evidence, Bugs, Customer Questions and Assumptions.pdf`, 15 pages:
intro and bug report, assumptions, the twelve customer questions, then one page
per test for all nine tests). Submission ZIP built at
`Part 3/Project 1 Part 3.zip` containing only `main.c` and the evidence PDF
inside a `Project 1 Part 3/` wrapper folder, matching the Part 1 ZIP's
structure. No compiled binary included.

**Evidence PDF finalised (2026-09-16):** rebuilt using Branson's own terminal
screenshots in place of rendered transcripts, with each test page labelled in
the spec's exact wording (Input entered / Expected output / Actual output /
Pass/Fail result). 14 pages, nine screenshots, all verified against the
program's real output. Submission ZIP rebuilt and re-audited from the
extracted files.

**Not yet done (user's to complete):** the Canvas upload and the git commit.

## Part 4 (2026-09-17) — Save/load student record to a text file

**Requirements:** Continue all Part 1–3 functionality. Save the student's ID, name,
GPA, academic standing, and five course grades to `student_records.txt`. Immediately
read the file back and display a "Recovered Student Record" in the same format as
the original Student Summary. Let the employee verify the recovered data matches
what was originally entered. Single student only. Newly permitted this week: file
processing, character processing, type casting, formatted file I/O (`fprintf`/
`fscanf`). Loops and functions remain out of scope.

**Implementation:** [main.c](main.c) — version bumped to 4.0. Refactored the
Academic Standing switch into a single `standingText` buffer filled once, reused
for on-screen display, the saved file, and the verification comparison — previously
each use had its own separate switch, and letting those three drift out of sync is
exactly the class of bug the Part 3 GPA-rounding fix was about. File writes with
`fopen(..., "w")` (always creates/overwrites — correct for a single-student system
with nothing to append to). Reads use `fgets` line-by-line for every field, even
numeric ones, since Student Name can contain spaces and mixing `fscanf`/`fgets` on
one stream is a common source of leftover-newline bugs. Average/highest/lowest are
recomputed from the recovered grades — the file never stores those three derived
values, only the five raw grades — using the same duplicated (not shared-function)
logic as the original computation, since functions are still out of scope.

**Answers to the PDF's "Questions for the Customer":** filename hardcoded to
`student_records.txt` (not configurable this week, matches the spec's own Example
3 exactly); overwrite, not append (single student, nothing to append to); a write
failure prints an error and exits, same pattern as a read failure; file is read
back automatically in the same run right after saving (matches Example 1's exact
output sequence); the recovered report exactly matches the original Student
Summary's fields/format; no course names exist in this program, so none are saved;
Academic Standing is saved and re-displayed as plain text, not recalculated from
the recovered GPA; a missing/incomplete file (any expected line absent) prints a
clear error and exits rather than attempting partial recovery; no blank lines in
the file, to match Example 2's file contents exactly; multiple-student support and
configurable filenames are both explicitly out of scope this week per the spec.

**Tests:** Compiles clean with `-Wall -Wextra -std=c11`, zero warnings. Spec's
Example 1 (Alice Johnson) reproduced exactly — saved file byte-for-byte matches
Example 2's expected contents, recovered report matches the original, verification
reports a match. All Part 2/3 regression cases re-verified (Michael Brown, Christopher
Williams, invalid-GPA rejection, and the 1.999-boundary rounding case) — all still
pass exactly, including in the recovered/reloaded copy. File-read failure tested by
revoking read permission on an existing file after a successful write — produces
the exact error wording from the spec's Example 3.

**Independent re-audit (2026-09-22) — two real bugs found and fixed.** The test
claims above were re-run from scratch rather than taken on trust. Confirmed
true: clean compile (also with `-pedantic`), saved file byte-identical to the
spec's Example 2, read-failure path matching Example 3 word for word, and all
Part 1/2/3 regressions passing. Two defects were nevertheless present:

1. **A failed save was reported as a success.** `fclose` was unchecked, and
   "Student information successfully saved." printed regardless. `fprintf`
   only fills a buffer, so a flush failure occurs after every `fprintf` has
   already succeeded. Reproduced by pointing the data file at a device that
   accepts writes but always fails to flush: the program claimed success, read
   back uninitialised memory (`Highest Grade : 1500501712`), and exited 0.
   Fixed by checking `fclose` and only claiming success on 0.
2. **Every `sscanf` return value was ignored.** A present-but-non-numeric line
   left the recovered variable uninitialised and parsing continued on garbage.
   Each conversion is now checked and names the offending field.

Also aligned to spec: the write-failure message now uses the same three-line
text as the read failure, since the spec gives one error block for "the data
file cannot be opened" (Example 3).

File error behaviour verified empirically for all five conditions (unwritable,
unreadable, flush failure, empty file, non-numeric line) — each reports a clear
error and exits 1.

**Known, flagged, not changed:** required regression test 5 ("attempt to read a
missing file") is unreachable by design, because the program always writes the
file immediately before reading it; the reachable equivalent is "cannot be
opened for reading", the same code path and the same spec wording. And the
recovered report prints `Course 1 : 95` where the spec's Example 1 shows bare
grade numbers — deliberate per Question for the Customer #6, but a visible
difference. Both are written up in `Notes/Part 4.md`.

**`student_records.txt` generated** from a real run and placed in the submission
folder; the spec requires it in the ZIP this week.

**Evidence PDF and submission ZIP built (2026-09-22).** Branson supplied two
screenshots (Alice Johnson, spec Example 1 — covers Tests 1-4). Claude generated
Test 5 (read-permission-denied) plus four extra regression screenshots (Michael
Brown, Christopher Williams, invalid-GPA rejection, the 1.999-boundary case) by
re-running the actual compiled program against the current, audited `main.c` and
rendering the real captured terminal output as matching-style images — verified
byte-for-byte against a fresh compile before use, not fabricated. Assembled into
a 14-page `Project 1 Part 4 - Test Evidence.pdf`: all 7 screenshots, saved-file
contents, the real Bug 1/Bug 2 writeup from the 2026-09-22 re-audit (previously
a draft of this PDF incorrectly claimed "no bugs found" — corrected before
finalizing), the standingText refactor note, all 12 customer-question answers,
and 4 assumptions (including the required write-up on the Test 5 interpretation
and the Course-N-format difference flagged in `Notes/Part 4.md`). Submission ZIP
built at `Part 4/Project 1 Part 4.zip` (`main.c` + `student_records.txt` inside
a `Project 1 Part 4/` wrapper, matching the Part 1/3 structure). Both also saved
to `~/Desktop/part4 SC/`.

**Not yet done (user's to complete):** read the evidence PDF once before
submitting (the customer answers are written in Branson's voice — he should own
them), confirm the due date on Canvas (**Sep 24, 2026 per `Notes/Part 4.md`**,
2 days out as of Sep 22), the Canvas upload itself, and the git commit/GitHub
push (ask before pushing — the Desktop git repo is still at v3.0 with no Part 4
at all, per standing rule).
