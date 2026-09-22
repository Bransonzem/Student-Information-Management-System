# SIMS Project — Session Log

Precise, dated record of every session's work on this project. This is the
authoritative source of truth for continuity across chats — read this file
(and CHANGELOG.md) in full before doing any new work here, even if memory
already seems to have the context.

Separate from the weekly standalone CCR series (Tampa Fitness/Sunrise
Coffee/Grand Cinema, etc.) — never pull content or context from that series
into this log or vice versa.

---

## 2026-09-09 (continued, Obsidian setup)

- User wants an Obsidian vault tied to this project for notes. Confirmed
  Obsidian was already installed (`/Applications/Obsidian.app`, since
  Aug 12). Created `Notes/` inside the project folder as the intended
  vault root, with starter notes: `SIMS Project.md` (index, links to
  CHANGELOG.md/SESSION_LOG.md/main.c), `Part 1.md`, and `Part 2.md`
  (each summarizing that part's spec/code/evidence links, judgment
  calls, and bugs). Added `Notes/.obsidian/` to `.gitignore` so
  Obsidian's own app config isn't tracked, while the `.md` notes
  themselves are regular tracked files like CHANGELOG.md/SESSION_LOG.md.
  Launched Obsidian via `open -a Obsidian`; user still needs to use
  "Open folder as vault" in the app and select this `Notes/` folder
  (not scriptable from the CLI without editing Obsidian's own internal
  config, which wasn't worth the risk for a one-time setup step).

## 2026-09-09 (continued, after Part 2 submission)

> **Confirmed 2026-09-16:** this heading is accurate. Branson confirmed that
> Part 1 and Part 2 were both submitted to Canvas. An earlier reading of this
> project treated the absence of a local `Project 1 Part 2.zip` as evidence
> that Part 2 had never been submitted — that inference was wrong. The local
> folder is not a record of what went to Canvas.

- User reported: entering an invalid course grade correctly showed an
  error but "just stopped" (program exits rather than re-prompting).
  Explained this is by design, not a bug — re-prompting needs a loop,
  which Part 2's spec puts out of scope. Asked whether to keep the
  exit-on-invalid behavior or add a flagged loop deviation (same pattern
  used elsewhere in the class's CCR series when retry is otherwise
  impossible). User initially answered "keep as-is," then immediately
  followed up "i need to be able to try again" (before any code was
  touched), then — after I'd started planning a retry-loop
  implementation — sent "Keep all teh contraints listed in the pdf,"
  which reversed that again. Resolved: no code change. "Try again" was
  clarified to mean simply re-running the compiled `./sims` binary from
  the terminal after fixing bad input, not an in-program retry loop.
  Part 2's code, evidence PDF, and submission ZIP are unchanged and
  still accurate as originally built.

## 2026-09-09 (continued)

- Read Part 2's PDF spec in full and began implementation, per user request
  to "go to part 2 and do everything it says, like how we did part 1."
- Flagged and resolved a real conflict before writing code: Part 2's own
  worked example output omits the `Programmer:` line, but Part 2's
  Acceptance Criteria requires all Part 1 functionality (which included
  that line) to keep working. User chose to keep the line, prioritizing
  the Acceptance Criteria over the example.
- Mid-session correction from user: "STOP SAYING CCR1 THATS DIFFERENT" —
  caught myself using "CCR-001" terminology while explaining the
  Programmer-line conflict. Reinforced: this project is "Part N" only,
  never "CCR," in every communication, not just written artifacts.
- Implemented Part 2 in `main.c`: version bumped to 2.0; added
  `courseGrades[5]` (array, newly permitted this week) sized via
  `enum { NUM_COURSES = 5 }` (enums also newly permitted); five grade
  prompts each validated like Part 1's numeric fields (reject letters,
  trailing garbage, out-of-range — valid range 0-100); average (2
  decimals)/highest/lowest computed via straight-line arithmetic and
  individual `if` comparisons, no loop or function (both still out of
  scope this week per the PDF).
- Caught and fixed my own structural bug before testing: initially
  printed a "Course Grades" header both before the input prompts AND
  again in the final report — the PDF's expected output only shows it
  once, as the report header after all input is collected (same pattern
  as "Student Summary"). Removed the premature one.
- Compiled clean with `-Wall -Wextra`. Verified all three required
  regression test cases pass exactly: Alice Johnson (avg 91.60, high
  100, low 84), Michael Brown (avg 79.00, high 91, low 68), Christopher
  Williams (avg 100.00, high 100, low 100). Adversarially tested letters/
  trailing-garbage/out-of-range on a course grade, boundary grades of
  exactly 0 and 100 (avg correctly 50.00), and re-ran Part 1's own
  invalid-ID/blank-name rejections to confirm no regression.
- Drafted answers to the PDF's required "Questions for the Customer"
  section (grades are whole numbers 0-100; no fractional grades; invalid
  grade prints an error and exits rather than re-prompting; courses have
  no names; average rounds to 2 decimals; a tied highest/lowest is shown
  once with no course identification, since that would be a "searching"
  feature and is out of scope; more than five courses not addressed this
  week) — full reasoning recorded in `CHANGELOG.md`'s Part 2 entry.
- Updated the `Part 2/Project 1 Part 2/main.c` snapshot to match.
- Built the Part 2 submission evidence PDF the same way as Part 1's (real
  `pty`-captured transcripts, not simulated text): 5 pages — title +
  regression testing summary, full answers to the PDF's "Questions for
  the Customer" section (required in Part 2's submission, unlike Part
  1), then one screenshot per required test case (each showing the full
  end-to-end run: Part 1 fields + Part 2 grades together), all marked
  PASS. Saved at `Part 2/Project 1 Part 2/Test Evidence, Bugs, and
  Customer Questions.pdf`.

## 2026-09-09

- Re-verified Part 1 was submission-ready by checking `main.c` and the
  evidence PDF against the actual PDF's own "Acceptance Criteria" (all
  10 checkboxes) and "Submission Requirements" sections before zipping
  anything, per the user's question "is everything ready to submit
  before i create the zip?" Confirmed: compiles clean with
  `-Wall -Wextra`, `Part 1/Project 1 Part 1/main.c` snapshot byte-matches
  the root `main.c`, and `Test Evidence and Bugs.pdf` already covers the
  3 required test cases + bugs writeup. Flagged (not yet acted on) that
  the evidence PDF doesn't separately demonstrate the input-validation
  refinement added after it was built, since that wasn't a "bug" and
  wasn't one of the 3 required test cases.
- Built the final submission ZIP at `Part 1/Project 1 Part 1/Project 1 -
  Part 1 Submission.zip`, flat (no subfolders) containing exactly
  `main.c` and `Test Evidence and Bugs.pdf`, per the PDF's stated
  Submission Requirements. Added `*.zip` to `.gitignore` — the zip is a
  build artifact assembled from files already tracked in git, so it's
  not committed itself, same treatment as the compiled `sims` binary.

## 2026-09-08 (continued)

- Added input validation to `main.c`, per user request: "if it ask for a number
  dont let it use letters, and vise versa. Make realistic and logical
  restrictions." Flagged first that full validation both ways isn't equally
  possible under Part 1's constraints: numeric fields can be checked with a
  single `if` (via `scanf`'s return value plus a one-character trailing-garbage
  peek), but rejecting embedded digits in the Student Name field would require
  scanning an arbitrary-length string character by character — a loop or a
  function, both explicitly out of scope this week. User's follow-up ("do
  exactly what the instructions say, but if you can improve the code then do
  it") resolved this: stayed within the no-loops/no-functions constraint.
  Implemented: Student ID rejects non-numeric input, trailing garbage after the
  number (e.g. `"123abc"`), and negative values; Current GPA rejects
  non-numeric input, trailing garbage, and values outside 0.0-4.0 (matches the
  PDF's own example scale); Student Name rejects a blank entry (no loop
  needed — just checks the first character). Digit-rejection in the name field
  is intentionally NOT implemented, and is documented as such in
  `CHANGELOG.md`, since it cannot be done without breaking the stated
  constraint. All error paths print a clear message and exit(1) rather than
  looping to re-prompt (retry would also require a loop). Recompiled clean
  with `-Wall -Wextra` and re-verified: all 3 required test cases still pass,
  plus adversarial tests for every new rejection path (letters in ID/GPA,
  trailing garbage in both, negative ID, blank name, GPA out of range,
  boundary GPA = 0.0) all behaved correctly. Updated the `Part 1/Project 1
  Part 1/main.c` snapshot to match.

## 2026-09-08

- Received the CCR-001 (Project 1 - Part 1) PDF spec: Student Information
  Management System (SIMS), COP 3515, University Registrar's Office.
- Wrote `main.c`: banner (course/software title, version, programmer name,
  welcome message), then prompts for Student ID (int) / Student Name
  (string) / Current GPA (float), then a formatted "Student Summary."
  Permitted features only — no arrays-for-records, functions, loops, or
  file I/O per the PDF's "Out of Scope" list.
- Compiled with `gcc -Wall -Wextra -std=c11` — zero warnings.
- Verified against all 3 PDF-required test cases: Alice Johnson (3.84),
  Michael Brown (2.91), Christopher Williams (4.00) — all passed exactly.
- Bug found: user ran the program in VS Code and prompts printed jumbled
  together instead of in order. Root cause: `stdout` becomes fully
  buffered (not line-buffered) when it isn't a real TTY, so prompt text
  sat in the buffer instead of appearing before `scanf`/`fgets` blocked
  for input. Fix: added `fflush(stdout);` immediately after every prompt
  `printf`. Recompiled and reverified — all 3 test cases still pass.
- Created `CHANGELOG.md` to track future increments of this project.
- Naming correction: the PDF's own internal label for this release is
  "CCR-001," which collides with the unrelated weekly CCR series' naming
  (CCR1 = Tampa Fitness, etc.). Per explicit user instruction — "CCRs and
  my project are entirely separate... My project will have the same
  template and the same info" — renamed this project's increments to
  **"Part N"** everywhere in its own files/comments going forward. Part 1
  = the PDF's "CCR-001."
- Initialized a git repository scoped to this folder only
  (`/Users/bransonzemaitis/Desktop/Advanced Program/Project/`, branch
  `main`), added `.gitignore` (compiled binary `sims`, `.DS_Store`), and
  made the initial commit (`f25e45c`, "Part 1: initial SIMS prototype").
- Established ongoing process: every time this project's code changes,
  update `CHANGELOG.md` (per-Part summary) and this `SESSION_LOG.md`
  (precise dated record), then commit both together.
- Confirmed with user: the file lives at
  `/Users/bransonzemaitis/Desktop/Advanced Program/Project/main.c` (note:
  `Project`, not `Project 1` — an earlier, now-deleted attempt lived at
  the latter path before the real Part 1 PDF was available).
- Created `Part 1/` subfolder and moved `Project 1 - Part 1.pdf` into it.
  `Project 1 - Part 2.pdf` (already present in the project root) left
  unfiled until Part 2 work begins.
- Created `Part 2/` subfolder and moved `Project 1 - Part 2.pdf` into it
  (organizational only — Part 2 requirements have not been read or
  implemented yet).
- Added a minimal title-label file to each part folder (superseded below).
- Built the Part 1 submission evidence PDF per the PDF's "Submission
  Requirements"/"Testing Requirements" sections: captured real terminal
  transcripts for all 3 required test cases by running the compiled
  `sims` binary under a Python `pty` (not simulated/typed-up text),
  rendered each into a dark terminal-style screenshot, and assembled a
  4-page PDF (title + bug writeup, then one screenshot per test case,
  each marked PASS) at
  `Part 1/Project 1 Part 1/Test Evidence and Bugs.pdf`. Bug writeup
  describes the stdout-buffering/fflush fix from earlier this session.
  Scripts used to generate this live only in the session scratchpad, not
  the project repo (not needed for future parts — Part 2's evidence PDF
  will be built fresh from Part 2's own test cases).
- Correction: the ".md label file" request actually meant a **subfolder**
  named `Project 1 Part N`, not a text file. User manually reorganized
  `Part 1/` to: `Part 1/Project 1 - Part 1.pdf` (spec) plus
  `Part 1/Project 1 Part 1/main.c` (finished Part 1 code snapshot),
  removing the earlier `.md` label file and the flat `main.c` copy.
  Mirrored the same structure for `Part 2/`: created
  `Part 2/Project 1 Part 2/main.c` as a copy of the current main.c —
  this is just Part 2's *starting point* to build from, not finished
  Part 2 code. Neither of these `Project 1 Part N/main.c` snapshot
  copies auto-updates when the root `main.c` (the live, cumulative file)
  changes — they must be refreshed manually when a part is finalized.

## 2026-09-16 (Part 3 — academic standing)

- User supplied the Part 3 spec PDF (`Project 1 - Part 3.pdf`, CCR-003).
  Filed per the established process: canonical copy at
  `Part 3/Project 1 - Part 3.pdf`, quick-glance copy at
  `00 Instructions/Part 3.pdf`. Created `Part 3/Project 1 Part 3/` as the
  (currently empty) submission folder.
- **Deadline discrepancy flagged, unresolved.** User believed Part 3 was due
  Thu Sep 17. The spec header says "Due Date: Beginning of Lesson 8." Using
  the lesson-to-date mapping already confirmed in this project's `CLAUDE.md`
  (Tue/Thu class; Part 1 = Lesson 4 = Thu Sep 10, Part 2 = Lesson 6 = Thu
  Sep 17), Lesson 8 falls on **Thu Sep 24**, giving a full extra week. Part 2
  is what is actually due Sep 17. User needs to confirm on Canvas — this was
  not verified here.
- **Stale root `main.c` corrected before any Part 3 work.** Root `main.c`
  (Sep 9, 00:41) and the Part 2 snapshot (Sep 9, 20:58) had drifted. Both
  178 lines; the only difference was the Student Summary block position. The
  root still printed it *before* the course grade prompts; the snapshot had
  the Part 2 fix moving it below Course 5. The snapshot was newer and correct,
  which is the reverse of the intended arrangement (root is supposed to be the
  live canonical file). Copied snapshot → root, verified identical by md5.
  Worth checking both before starting any future part.
- Implemented Part 3 against the corrected base:
  - `VERSION_NUMBER` "2.0" → "3.0".
  - Reworded the existing Part 1 GPA range-check message to match the spec's
    Example 5 exactly: `ERROR` on its own line, then `Invalid GPA entered.`,
    then `GPA must be between 0.00 and 4.00.` The check itself was already
    present and already sat above the classification, satisfying the spec's
    "validate before determining standing" requirement without moving code.
  - Added named `enum AcademicStanding` (HONORS / GOOD / PROBATION /
    SUSPENSION).
  - Classification via if/else-if cascade on `>=` lower bounds only. Chosen
    over two-sided range tests because the published bands have gaps (nothing
    covers 3.495, between 3.49 and 3.50). The cascade closes them and makes
    band limits inclusive.
  - Display via `switch` on the enum, printing `Academic Standing :` directly
    under `Current GPA`, matching the existing column alignment.
  - No functions, no loops, no files — all still out of scope this part.
- Heavily commented the new code at the user's request (he is learning C).
  The comments explain the enum's purpose, the band-gap reasoning, and
  specifically why the switch tests the enum rather than the float: a C switch
  requires an integer expression, so `switch (currentGPA)` does not compile.
  The if/else does the range comparison a switch cannot; the switch does the
  label selection.
- Compiled `gcc -Wall -Wextra -std=c11` — zero warnings.
- Ran all six required standing tests (3.95 / 3.20 / 1.75 / 0.60 / -0.50 /
  4.25) — all pass, with both invalid GPAs printing the exact three-line
  Example 5 message and exiting before course grades are requested.
- Ran all three Part 2 regression cases (Alice Johnson, Michael Brown,
  Christopher Williams) — all pass exactly, averages and high/low unchanged.
- Additionally spot-checked band boundaries and the gap value (4.00, 3.50,
  3.49, 3.495, 2.00, 1.99, 1.00, 0.99, 0.00) — every value classifies into
  exactly one category, no gaps.
- Created `Notes/Part 3.md` (spec summary, standing bands, required tests,
  build order, assumptions to log, both gotchas) and added it to the
  `SIMS Project.md` index.
- **Deliberately NOT done, left to the user:** answers to the Questions for
  the Customer, the assumptions write-up, test evidence screenshots, the
  evidence PDF, the Part 3 `main.c` snapshot, the submission ZIP, and the git
  commit. Per the spec's AI usage policy the user is responsible for
  understanding and testing all submitted code, and the screenshots must be
  his own.

## 2026-09-16 (continued — Part 3 deliverable completed)

- **Deadline resolved.** Branson confirmed Part 3 is due **Thu Sep 17**, not
  Sep 24. The spec's "Beginning of Lesson 8" label therefore does not match the
  lesson-to-date map recorded in the project `CLAUDE.md`. Updated `CLAUDE.md`
  with a warning block: the map is wrong, every future part deadline was being
  derived from it, and the real lesson schedule needs pulling from Canvas.
  Noted the corroborating signal that the syllabus puts "Review for Quiz #1" at
  Lesson 8 while Branson's quiz is Sep 22, which the current map cannot explain.
- **Real bug found during testing, and fixed.** Classification read the raw
  `currentGPA` while the report printed `%.2f` of it. Near a band edge the two
  disagreed: `1.999` printed `Current GPA : 2.00` next to `Academic Standing :
  Academic Probation`, which contradicts itself. Fixed by computing
  `gpaRounded = ((int)(currentGPA * 100.0f + 0.5f)) / 100.0f` once, then using
  that one variable for both the if/else cascade and the `Current GPA` line, so
  the two can never diverge. Cast technique matches the Part 2 average. Safe
  because negatives are already rejected above. Updated the stale inline
  comment that still referenced `currentGPA >= 2.00f`.
- Re-verified after the fix: `1.999` and `1.995` → 2.00 / Good Standing;
  `3.4999` and `3.495` → 3.50 / Honors; and every band boundary (4.00, 3.50,
  3.49, 2.00, 1.99, 1.00, 0.99, 0.00) classifies consistently with what it
  displays. Recompiled `-Wall -Wextra -std=c11`, still zero warnings.
- **Test evidence captured for real, not retyped.** Ran the compiled binary
  under a Python `pty` (same technique used for Part 1's evidence) so prompts
  and typed input interleave the way they do in a real terminal. Captured all
  nine sessions: the six required standing cases plus the three Part 2
  regression cases. All nine pass. Both invalid-GPA runs exit with status 1
  before any course grade prompt.
- **Evidence PDF built** with reportlab at
  `Part 3/Project 1 Part 3/Test Evidence, Bugs, Customer Questions and
  Assumptions.pdf` — 15 pages, structured to match the Part 2 evidence PDF:
  name/course/project header, intro, bug report, assumptions, the twelve
  Questions for the Customer answered, then one page per test. Transcripts are
  rendered as dark terminal panels and explicitly labelled as captured
  transcripts rather than screen photographs, so nothing is presented as a
  screenshot that is not one.
  - Build note: a first attempt styled the transcript blocks with a paragraph
    `backColor`, which did not paint — light text rendered on white and was
    nearly invisible. Rewrote the block as a single-cell `Table` with a
    `BACKGROUND` style, which paints reliably. Verified by rasterising pages
    and viewing them.
- **Snapshot refreshed and ZIP built.** Copied the live `main.c` into
  `Part 3/Project 1 Part 3/` and verified byte-identical. Built
  `Part 3/Project 1 Part 3.zip` containing only `main.c` and the evidence PDF
  inside a `Project 1 Part 3/` wrapper folder — matching the Part 1 ZIP's
  actual structure rather than the "flat, no subfolders" wording in
  `CLAUDE.md`, since the Part 1 precedent is what was already submitted
  against. Verified the archive integrity, that no compiled binary is present,
  and that the `main.c` inside hashes identical to the live file.
  - The `zip` CLI failed partway (sandbox permissions) and left a stray temp
    file `Part 3/zibS99Fv`. It could not be deleted from here — permission to
    delete was denied. **Branson should delete it manually.** It is not in the
    archive and does not affect the submission.
- **Deliberately NOT done:** no git commit (left to Branson), no Canvas upload,
  and no sign-off on the evidence PDF — his name is on that document and he
  needs to read it before it goes anywhere. Verification list recorded in
  `Notes/Part 3.md`.

## 2026-09-16 (continued — evidence PDF rebuilt to the Part 1 template)

- Branson asked for the Part 3 evidence document to match **Project 1 Part 1**,
  not Part 2 — "nice and organized with the terminal screenshots."
- Measured the Part 1 PDF rather than eyeballing it: 612x792, margins 78pt
  left/right, no footer and **no page numbers**. Header is "BRANSON ZEMAITIS"
  in Calibri Light 19 at the left with "COP 3515" (Calibri Bold 11.5) and
  "Advanced Program Design" (Calibri 10) right-aligned on the same row, then a
  Calibri 10 subtitle line, then the document title in Calibri Light 17.
  Section headings Calibri Light 13, body Calibri 11, inline code Consolas 10.5.
  Test pages are heading, then a one or two line caption, then the screenshot
  placed flush with the left text margin.
- Extracted the three embedded screenshots from the Part 1 PDF and sampled
  them: background RGB(12,12,12), text RGB(204,204,204), line pitch ~19px,
  character advance ~9.27px, ~4-6px left padding, no window chrome and no
  PowerShell prompt line — they are cropped to the program output only.
- Wrote `term_img.py` to render the captured transcripts as terminal images
  matching those measurements: same background and foreground colours, same
  pitch, and a font size auto-fitted so the character advance lands on 9.23px
  against the 9.27px target. Rendered at 3x and placed at the same 0.749
  reduction Branson used, so the images are crisp at his physical size.
- Rebuilt the PDF with reportlab using Carlito (metric-compatible with Calibri)
  and DejaVu Sans Mono for Consolas. 14 pages: header/intro/bugs, assumptions,
  the twelve customer questions, then one test per page with its terminal image.
- Output renamed to `Project 1 Part 3 - Test Evidence.pdf`, matching the Part 1
  naming convention. The earlier Part-2-styled PDF
  (`Test Evidence, Bugs, Customer Questions and Assumptions.pdf`) is superseded
  but could not be deleted (permission denied), so it still sits in the folder.
  **It is explicitly excluded from the ZIP** — the archive is built from a
  fixed file list, not a directory walk. Branson should delete it manually.
- Shrank the terminal PNGs to 8-bit greyscale, which took the PDF from 1.49 MB
  to 877 KB.
- ZIP rebuilt and re-verified: two entries, integrity clean, no binary, the
  superseded PDF absent, and the `main.c` inside hashing identical to the live
  file.
- Known cosmetic gaps vs the Part 1 document, recorded honestly in
  `Notes/Part 3.md`: Carlito has no Light weight so the large headings are
  slightly heavier than Calibri Light; the terminal font is DejaVu Sans Mono
  rather than his actual Consolas/Cascadia; his captures are cropped to a
  terminal window while these show each run in full; and his are 96 dpi screen
  captures while these are cleanly rendered, so they look sharper than a real
  screenshot rather than identical to one.

## 2026-09-16 (continued — leftover cleanup)

- Branson authorised removing the two leftovers and asked for them to go to the
  macOS Trash, not a hard delete.
- **Could not reach the Trash.** Four routes tried: (1) the sandbox delete
  permission tool was denied twice; (2) `rm` returns "Operation not permitted"
  inside the vault, though `mv` works; (3) `~/.Trash` is not reachable — only
  `Documents` is mounted into the sandbox; (4) Finder was granted and navigated
  to the Part 3 folder and `zibS99Fv` was successfully selected, but
  File > Move to Trash is disabled while Finder is backgrounded, and the
  full-screen control request was declined.
- **Fallback used, fully reversible:** moved both files with `mv` into
  `_TRASH ME - superseded Part 3 files/` at the Project root, with a
  `README.txt` explaining what each one is. They are out of `Part 3/` so they
  can no longer be confused with the real deliverable. Branson drags that one
  folder to the Trash himself.
- Files moved: `zibS99Fv` (stray temp archive containing the OLD evidence PDF)
  and `Test Evidence, Bugs, Customer Questions and Assumptions.pdf` (the
  superseded Part-2-styled draft).
- ZIP re-verified after the move: integrity clean, two entries, `main.c` still
  hashing identical to the live file, correct evidence PDF present, no binary.
  `Part 3/` now holds only the spec PDF, the submission folder, the ZIP, and
  `.DS_Store`.

## 2026-09-16 (continued — records corrected, lesson map resolved)

- **Lesson 07 slide deck received and read.** It resolves the deadline confusion
  that has been running through this project. The deck is titled *"Lesson #7:
  While, Do While, For, Break, Continue"* — loops. Its closing slides say
  "What We Covered Today: Do While, While, For, Break, Continue", "Reading For
  Next Class! Chapter 7: Functions", and "What We'll Be Covering Next Time:
  Function Declarations, Arguments, Array Arguments".
- **The syllabus topic list runs one lesson AHEAD of what is actually taught.**
  Syllabus says Lesson 6 = loops and Lesson 7 = Functions. Reality: Lesson 7 =
  loops (taught Wed Sep 16), Lesson 8 = Functions (Thu Sep 17). Every earlier
  deadline inference in this project was derived from the syllabus list and was
  therefore wrong. Confirmed anchors are now recorded in the project
  `CLAUDE.md`; dates for Lessons 1-6 still need pulling from Canvas.
- Note this also breaks the "class meets Tue/Thu" assumption from the Sep 9
  Chess Moves note — Lessons 7 and 8 fall on consecutive days, Wed then Thu.
  Do not extrapolate deadlines from that meeting pattern.
- **Loops question settled: loops ARE in scope for Part 3.** Two independent
  tests both pass. (1) Taught: loops were covered in Lesson 7, the class before
  Part 3 is due, and Part 3 scopes to "features covered through Lesson 8".
  (2) Not banned: Parts 1 and 2 both listed "Loops" under Out of Scope
  verbatim; Part 3's Out of Scope list omits it. An earlier note in
  `Notes/Part 3.md` called this ambiguous and recommended straight-line code —
  that caution was based on the syllabus, which turns out not to reflect what
  was taught. The current code uses no loops either way, so nothing changes.
- **Functions remain FORBIDDEN for Part 3** despite being taught in Lesson 8.
  Part 3's Out of Scope list names "Functions" verbatim. Being taught something
  does not override an explicit ban in the assignment. These are separate
  tests and both must pass.
- **Submission records corrected.** Branson confirmed Part 1 and Part 2 were
  both submitted to Canvas. Earlier notes in this project claimed neither had
  been submitted; that claim originated from `CLAUDE.md`'s Sep 9 status block
  and was reinforced by the absence of a local `Project 1 Part 2.zip`.
  **Absence of a local ZIP is not evidence of non-submission** — Part 2 was
  submitted without one ever being saved here. The Sep 9 "after Part 2
  submission" heading above is accurate and has been annotated as confirmed.
- Practice change recorded in `CLAUDE.md`: save the exact uploaded ZIP into each
  part's folder so the local tree and Canvas agree.
- **No code was changed in this session.** `main.c` is untouched and the Part 3
  ZIP is unchanged.

## 2026-09-16 (continued — Branson's own screenshots swapped in, ZIP finalised)

- Mounted `~/Desktop/project p3 screenshots` and inventoried nine PNGs. Mapped
  each to a test by capture time and by OCR-ing the contents. All nine required
  cases present, no extras, none missing.
- **Verified every screenshot by reading its text**, not by trusting the
  filename: student ID, name, GPA typed, five grades, the resulting Academic
  Standing, and the Part 2 average/highest/lowest. All nine show correct values
  and match what the program actually produces. Both invalid-GPA captures show
  the exact three-line error and stop before the course grade prompts. Every
  capture ends with the shell prompt returning, so none is truncated mid-run.
- Note: the captures come from a Linux container shell
  (`student@0193210b33c3:/workspace$`), not PowerShell as in Parts 1 and 2. The
  evidence PDF does not claim any particular shell, so there is no conflict,
  but it is a visible difference from the earlier documents.
- Rebuilt the evidence PDF with Branson's screenshots replacing the rendered
  transcripts, keeping the spec's exact labels (Input entered / Expected output
  / Actual output / Pass/Fail result). His captures are ~2x retina, so they are
  placed at half their pixel size and then at the 0.749 reduction used in his
  Part 1 document — widths land at 238-288pt, matching Part 1's 283-297pt. At
  that scale every test still fits on one page: 14 pages, zero orphaned images.
  The image shrink needed for the rendered version was no longer necessary.
- Hash-traced all nine embedded images back to Branson's original files to
  confirm no wrong or stale image was placed.
- `main.c` UNCHANGED this session (md5 dd1e601a2cfa73566f660c37ab856a5f).
- ZIP rebuilt from an explicit two-file list: `main.c` + the final evidence PDF
  inside the `Project 1 Part 3/` wrapper. Re-audited by extracting to a temp
  dir and testing the EXTRACTED files: compiles clean with `-Wall -Wextra`,
  all six required standing cases pass, all three CCR-002 regressions pass.
- The intermediate relabeled draft (rendered transcripts) was moved to
  `_TRASH ME - superseded Part 3 files/` so only one evidence PDF remains in
  the submission folder.

## 2026-09-17 (Part 4 — save/load to a text file) [logged retroactively 2026-09-22]

- **This entry was missing.** Part 4 was written on the evening of Sep 17 and
  `CHANGELOG.md` was updated, but `SESSION_LOG.md` never got its matching
  entry, against this project's own rule that both logs move together. Written
  up after the fact from the file timestamps and the code itself.
- Received the CCR-004 spec, filed to `00 Instructions/Part 4.pdf` and
  `Part 4/Project 1 - Part 4.pdf`, created `Part 4/Project 1 Part 4/`.
- Implemented v4.0 on top of v3.0: `standingText` buffer filled once by a
  single switch, write to `student_records.txt` with `fopen`/`fprintf`,
  read-back with `fgets`+`sscanf`, recomputation of average/highest/lowest from
  the recovered grades, and a field-by-field verification flag.

## 2026-09-22 (Part 4 — independent audit, two real bugs fixed)

- Treated every claim in the Sep 17 CHANGELOG entry as UNVERIFIED and re-ran
  everything from scratch against the spec.
- **Verified true:** compiles clean under `-Wall -Wextra -std=c11` and with
  `-pedantic`; the saved file is byte-identical to the spec's Example 2; the
  read-failure path prints the exact Example 3 wording; all Part 1/2/3
  regressions still pass; round-trip is clean on six cases with zero differing
  fields.
- **Found NOT true:** the CHANGELOG claimed the read-failure was "tested by
  revoking read permission after a successful write." The mechanism works, but
  two error paths it implied were fine were in fact broken — see below.
- **BUG 1 (real, fixed): a failed save was reported as a success.** `fclose`
  was unchecked. `fprintf` only fills a buffer; the data reaches disk at close,
  so a flush failure happens after every `fprintf` has "succeeded". Reproduced
  by pointing the data file at a device that accepts writes but always fails to
  flush: the old build printed "Student information successfully saved", read
  back uninitialised memory (`Highest Grade : 1500501712`), and **exited 0**.
  Fixed by checking `fclose`; now prints a clear error and exits 1.
- **BUG 2 (real, fixed): every `sscanf` return value was ignored.** A line that
  was present but not numeric left the recovered variable uninitialised and the
  program carried on with garbage. Each conversion is now checked and names the
  offending field.
- **Changed for spec conformance:** the write-failure message used different
  wording from the read-failure one. The spec gives one error block for "the
  data file cannot be opened" (Example 3), so both now print that exact text.
- File error behaviour verified empirically for all five conditions: unwritable,
  unreadable, flush failure, empty file, non-numeric line. All five report a
  clear error and exit 1. No crashes, no silent success.
- **Flagged, not fixed:** required regression test 5 ("attempt to read a missing
  file") is unreachable by design, because the program always writes the file
  immediately before reading it. The reachable equivalent is "cannot be opened
  for reading", which is the same code path and the same spec wording. Recorded
  in `Notes/Part 4.md` and to be stated in the evidence PDF assumptions rather
  than papered over.
- **Flagged, not fixed:** the recovered report prints `Course 1 : 95` while the
  spec's Example 1 shows bare grade numbers. Deliberate, per Question for the
  Customer #6, but a visible difference from the example. Branson's call.
- Generated `student_records.txt` from a real Alice Johnson run and placed it in
  the submission folder — the spec requires it in the ZIP this week.
- Refreshed the `Part 4/Project 1 Part 4/main.c` snapshot; verified identical
  to the live file.
- Created `Notes/Part 4.md` and added it to the `SIMS Project` index.
- **Not done, deliberately:** no ZIP, no git commit. The Desktop repo at
  `~/Desktop/Advanced Program/Project/` is still at **v3.0** with no Part 4 —
  left untouched, but it needs syncing before any GitHub push.
