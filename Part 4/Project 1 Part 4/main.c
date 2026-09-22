#include <stdio.h>
#include <string.h>

#define PROGRAM_TITLE "Student Information Management System"
#define VERSION_NUMBER "4.0"
#define PROGRAMMER_NAME "Branson Zemaitis"
#define DATA_FILE_NAME "student_records.txt"

enum { NUM_COURSES = 5 };

/* Part 3: the four academic standing categories the Registrar defined.
 *
 * Why an enum instead of just printing text inside the if/else chain?
 *   1. An enum IS an integer type under the hood (HONORS is 0, GOOD is 1,
 *      and so on). That matters below, because a C switch statement can
 *      only test an integer - it cannot test a float. See the switch in
 *      the Student Summary section for the full explanation.
 *   2. It separates the DECISION (which category does this GPA fall into)
 *      from the DISPLAY (what words do we print). If the Registrar later
 *      changes the wording from "Honors" to "Dean's List", only the switch
 *      changes, not the classification logic.
 *   3. It answers the spec's Question for the Customer #10 - standing is
 *      represented internally as a predefined category, not loose text.
 */
enum AcademicStanding {
    STANDING_HONORS,
    STANDING_GOOD,
    STANDING_PROBATION,
    STANDING_SUSPENSION
};

int main(void) {
    int studentID;
    char studentName[100];
    float currentGPA;
    int courseGrades[NUM_COURSES];

    printf("----------------------------------------\n");
    printf("%s\n", PROGRAM_TITLE);
    printf("Version %s\n\n", VERSION_NUMBER);
    printf("Programmer: %s\n\n", PROGRAMMER_NAME);
    printf("Welcome to SIMS\n");
    printf("----------------------------------------\n\n");

    printf("Enter Student ID: ");
    fflush(stdout);
    if (scanf("%d", &studentID) != 1) {
        printf("Error: Student ID must be a whole number (digits only, no letters).\n");
        return 1;
    }
    int idNextChar = getchar();
    if (idNextChar != '\n') {
        printf("Error: Student ID must contain only digits (no extra characters).\n");
        return 1;
    }
    if (studentID < 0) {
        printf("Error: Student ID cannot be negative.\n");
        return 1;
    }

    printf("Enter Student Name: ");
    fflush(stdout);
    fgets(studentName, sizeof(studentName), stdin);
    studentName[strcspn(studentName, "\n")] = '\0';
    if (studentName[0] == '\0') {
        printf("Error: Student Name cannot be blank.\n");
        return 1;
    }

    printf("Enter Current GPA: ");
    fflush(stdout);
    if (scanf("%f", &currentGPA) != 1) {
        printf("Error: Current GPA must be a number (no letters).\n");
        return 1;
    }
    int gpaNextChar = getchar();
    if (gpaNextChar != '\n' && gpaNextChar != EOF) {
        printf("Error: Current GPA must contain only digits and a decimal point (no extra characters).\n");
        return 1;
    }
    /* Part 3: the GPA range check below already existed in Part 1. The spec
     * requires the GPA to be validated BEFORE any standing is determined, so
     * this check stays exactly where it is - above the classification. Only
     * the wording of the message changed, to match the spec's Example 5.
     *
     * The program exits here rather than asking again. Re-prompting would
     * need a loop, and loops are not on this part's permitted feature list.
     * "Try again" means running the program again.
     */
    if (currentGPA < 0.0f || currentGPA > 4.0f) {
        printf("ERROR\n");
        printf("Invalid GPA entered.\n");
        printf("GPA must be between 0.00 and 4.00.\n");
        return 1;
    }

    /* Part 3: classify the GPA into an academic standing.
     *
     * The Registrar's published bands are:
     *     3.50 - 4.00  Honors
     *     2.00 - 3.49  Good Standing
     *     1.00 - 1.99  Academic Probation
     *     0.00 - 0.99  Academic Suspension
     *
     * Notice those bands have GAPS. Nothing covers a GPA of 3.495, which sits
     * between the top of "Good Standing" (3.49) and the bottom of "Honors"
     * (3.50). The bands were clearly written assuming two decimal places.
     *
     * Rather than test both ends of every band (>= 2.00 && <= 3.49), which
     * would leave those gaps unhandled, this uses a CASCADE of lower bounds
     * only. Each else-if is only reached when every test above it already
     * failed, so "else if (gpaRounded >= 2.00f)" already implies the GPA is
     * below 3.50 - no upper bound needed. Every valid GPA from 0.00 to 4.00
     * lands in exactly one category, and the band limits are inclusive.
     *
     * The final else is safe: the range check above already rejected anything
     * below 0.00, so by the time we reach it the GPA must be 0.00 to 0.99.
     */
    /* BUG FOUND DURING TESTING (2026-09-16), fixed here.
     *
     * The report used to print the GPA rounded to two decimals (%.2f, carried
     * over from Part 1) but classify using the raw unrounded value. Those two
     * can disagree. Entering 1.999 printed:
     *
     *     Current GPA : 2.00
     *     Academic Standing : Academic Probation
     *
     * which contradicts itself - 2.00 is Good Standing. The display rounded
     * up across the band edge while the classification did not.
     *
     * Fix: round the GPA to two decimals ONCE, into gpaRounded, then use that
     * same value for both the classification below and the display in the
     * Student Summary. Because both now read the same variable, the printed
     * GPA and the printed standing can never disagree.
     *
     * How the rounding works: multiply by 100 to shift the two decimals up
     * (1.999 -> 199.9), add 0.5 so the cast rounds to nearest instead of
     * chopping off (200.4), cast to int to drop the remainder (200), then
     * divide by 100.0f to shift back (2.00). The cast is the same technique
     * already used for the average grade in Part 2. Adding 0.5 is safe here
     * because the range check above already rejected every negative GPA.
     */
    float gpaRounded = ((int) (currentGPA * 100.0f + 0.5f)) / 100.0f;

    enum AcademicStanding standing;

    if (gpaRounded >= 3.50f) {
        standing = STANDING_HONORS;
    } else if (gpaRounded >= 2.00f) {
        standing = STANDING_GOOD;
    } else if (gpaRounded >= 1.00f) {
        standing = STANDING_PROBATION;
    } else {
        standing = STANDING_SUSPENSION;
    }

    /* Part 4: the standing's display text used to be built inline inside a
     * switch statement wherever it was needed. Part 4 needs that same text
     * in three places now (on-screen summary, saved to the file, and
     * compared during verification), so it is written into standingText
     * ONCE here via a single switch, and every later use just reads that
     * array. This avoids three separate switch statements silently drifting
     * out of sync with each other - the exact kind of duplication risk the
     * Part 3 GPA/standing bug (above) was caused by. */
    char standingText[30];
    switch (standing) {
        case STANDING_HONORS:
            strcpy(standingText, "Honors");
            break;
        case STANDING_GOOD:
            strcpy(standingText, "Good Standing");
            break;
        case STANDING_PROBATION:
            strcpy(standingText, "Academic Probation");
            break;
        case STANDING_SUSPENSION:
            strcpy(standingText, "Academic Suspension");
            break;
    }

    printf("\nEnter Course 1 Grade: ");
    fflush(stdout);
    if (scanf("%d", &courseGrades[0]) != 1) {
        printf("Error: Course 1 Grade must be a whole number (digits only, no letters).\n");
        return 1;
    }
    int grade1NextChar = getchar();
    if (grade1NextChar != '\n') {
        printf("Error: Course 1 Grade must contain only digits (no extra characters).\n");
        return 1;
    }
    if (courseGrades[0] < 0 || courseGrades[0] > 100) {
        printf("Error: Course 1 Grade must be between 0 and 100.\n");
        return 1;
    }

    printf("Enter Course 2 Grade: ");
    fflush(stdout);
    if (scanf("%d", &courseGrades[1]) != 1) {
        printf("Error: Course 2 Grade must be a whole number (digits only, no letters).\n");
        return 1;
    }
    int grade2NextChar = getchar();
    if (grade2NextChar != '\n') {
        printf("Error: Course 2 Grade must contain only digits (no extra characters).\n");
        return 1;
    }
    if (courseGrades[1] < 0 || courseGrades[1] > 100) {
        printf("Error: Course 2 Grade must be between 0 and 100.\n");
        return 1;
    }

    printf("Enter Course 3 Grade: ");
    fflush(stdout);
    if (scanf("%d", &courseGrades[2]) != 1) {
        printf("Error: Course 3 Grade must be a whole number (digits only, no letters).\n");
        return 1;
    }
    int grade3NextChar = getchar();
    if (grade3NextChar != '\n') {
        printf("Error: Course 3 Grade must contain only digits (no extra characters).\n");
        return 1;
    }
    if (courseGrades[2] < 0 || courseGrades[2] > 100) {
        printf("Error: Course 3 Grade must be between 0 and 100.\n");
        return 1;
    }

    printf("Enter Course 4 Grade: ");
    fflush(stdout);
    if (scanf("%d", &courseGrades[3]) != 1) {
        printf("Error: Course 4 Grade must be a whole number (digits only, no letters).\n");
        return 1;
    }
    int grade4NextChar = getchar();
    if (grade4NextChar != '\n') {
        printf("Error: Course 4 Grade must contain only digits (no extra characters).\n");
        return 1;
    }
    if (courseGrades[3] < 0 || courseGrades[3] > 100) {
        printf("Error: Course 4 Grade must be between 0 and 100.\n");
        return 1;
    }

    printf("Enter Course 5 Grade: ");
    fflush(stdout);
    if (scanf("%d", &courseGrades[4]) != 1) {
        printf("Error: Course 5 Grade must be a whole number (digits only, no letters).\n");
        return 1;
    }
    int grade5NextChar = getchar();
    if (grade5NextChar != '\n' && grade5NextChar != EOF) {
        printf("Error: Course 5 Grade must contain only digits (no extra characters).\n");
        return 1;
    }
    if (courseGrades[4] < 0 || courseGrades[4] > 100) {
        printf("Error: Course 5 Grade must be between 0 and 100.\n");
        return 1;
    }

    int gradeSum = courseGrades[0] + courseGrades[1] + courseGrades[2] + courseGrades[3] + courseGrades[4];
    float averageGrade = (float) gradeSum / NUM_COURSES;

    int highestGrade = courseGrades[0];
    if (courseGrades[1] > highestGrade) highestGrade = courseGrades[1];
    if (courseGrades[2] > highestGrade) highestGrade = courseGrades[2];
    if (courseGrades[3] > highestGrade) highestGrade = courseGrades[3];
    if (courseGrades[4] > highestGrade) highestGrade = courseGrades[4];

    int lowestGrade = courseGrades[0];
    if (courseGrades[1] < lowestGrade) lowestGrade = courseGrades[1];
    if (courseGrades[2] < lowestGrade) lowestGrade = courseGrades[2];
    if (courseGrades[3] < lowestGrade) lowestGrade = courseGrades[3];
    if (courseGrades[4] < lowestGrade) lowestGrade = courseGrades[4];

    printf("\nStudent Summary\n\n");
    printf("Student ID : %d\n", studentID);
    printf("Student Name : %s\n", studentName);
    printf("Current GPA : %.2f\n", gpaRounded);
    printf("Academic Standing : %s\n", standingText);

    printf("----------------------------------------\n");
    printf("Course Grades\n\n");
    printf("Course 1 : %d\n", courseGrades[0]);
    printf("Course 2 : %d\n", courseGrades[1]);
    printf("Course 3 : %d\n", courseGrades[2]);
    printf("Course 4 : %d\n", courseGrades[3]);
    printf("Course 5 : %d\n", courseGrades[4]);

    printf("\n----------------------------------------\n");
    printf("Average Grade : %.2f\n", averageGrade);
    printf("Highest Grade : %d\n", highestGrade);
    printf("Lowest Grade : %d\n", lowestGrade);

    /* Part 4: persist the student's record to a text file, then immediately
     * read it back and display a "Recovered Student Record" so an employee
     * can visually confirm the saved data matches what was entered. Only one
     * student is stored - see the Business Requirements; future CCRs will
     * add multiple-student support (Question for the Customer #11).
     *
     * File name is hardcoded to "student_records.txt" (Question #1 and #12 -
     * not configurable this week), matching the exact filename used in this
     * part's own File Error example. Writing uses "w" mode, which creates
     * the file if it does not exist and overwrites it if it does (Question
     * #2 and #3) - overwrite is the only sensible choice for a single-
     * student system with nothing to append to.
     *
     * File layout is one value per line, no blank lines (Question #10 -
     * kept compact to match the spec's own Example 2 file contents exactly):
     * Student ID, Student Name, GPA, Academic Standing (as text), then the
     * five course grades.
     *
     * Course names are not saved (Question #7) - this program has never had
     * course names, only grade values, so there is nothing to save.
     *
     * Academic Standing is saved as text and read back as text, not
     * recalculated from the recovered GPA (Question #8) - the simplest
     * choice that still satisfies "display what was read from the file."
     */
    /* The spec's File Error example (Example 3) gives ONE error block for
     * "the data file cannot be opened", so the same three lines are used for
     * both the write failure here and the read failure further down, word for
     * word, rather than inventing a separate write-specific message. */
    FILE *outFile = fopen(DATA_FILE_NAME, "w");
    if (outFile == NULL) {
        printf("\nERROR\n");
        printf("Unable to open %s\n", DATA_FILE_NAME);
        printf("Please verify that the file exists and that you have permission to access it.\n");
        return 1;
    }

    fprintf(outFile, "%d\n", studentID);
    fprintf(outFile, "%s\n", studentName);
    fprintf(outFile, "%.2f\n", gpaRounded);
    fprintf(outFile, "%s\n", standingText);
    fprintf(outFile, "%d\n", courseGrades[0]);
    fprintf(outFile, "%d\n", courseGrades[1]);
    fprintf(outFile, "%d\n", courseGrades[2]);
    fprintf(outFile, "%d\n", courseGrades[3]);
    fprintf(outFile, "%d\n", courseGrades[4]);

    /* BUG FOUND DURING TESTING (2026-09-22), fixed here.
     *
     * fclose() was being called without checking what it returned, and the
     * program printed "successfully saved" regardless. That is wrong: data
     * written with fprintf sits in a buffer and is only pushed to the disk
     * when the file is closed, so a failure at that moment (disk full, quota
     * exceeded, device removed) happens AFTER every fprintf has already
     * "succeeded". The old code would report a successful save on a file that
     * was actually incomplete or empty, which is the opposite of handling a
     * file error gracefully.
     *
     * Reproduced by pointing student_records.txt at a device that accepts
     * writes but always fails to flush. The old build printed "successfully
     * saved", then read the file back, printed garbage values from
     * uninitialised memory (e.g. "Highest Grade : 1500501712"), and still
     * exited with a success status. Fixed by checking fclose and only
     * claiming success when it returns 0. */
    if (fclose(outFile) != 0) {
        printf("\nERROR\n");
        printf("Unable to save %s\n", DATA_FILE_NAME);
        printf("The file may be incomplete. Please check available disk space and permissions.\n");
        return 1;
    }
    printf("\nStudent information successfully saved.\n");

    /* Read the file back immediately (Question #5 - yes, in the same run, so
     * the employee sees the comparison right away instead of restarting the
     * program). Every line is read with fgets, even the numeric ones,
     * because Student Name can contain spaces and fscanf("%s", ...) would
     * stop at the first space. Reading everything the same way (one line at
     * a time, then converting where needed with sscanf) keeps the parsing
     * consistent and avoids mixing fscanf and fgets on the same stream,
     * which is a common source of leftover-newline bugs.
     *
     * Question #9 (invalid/incomplete file data): if the file is missing a
     * line, fgets returns NULL instead of crashing. Each read is checked
     * individually and exits with a clear error on the first failure -
     * out of scope this week is recovering from a *partially* corrupt file
     * (only encryption/databases/dynamic memory are explicitly out of
     * scope, but fixing a bad file's contents would need more than this
     * week's permitted features), so "stop and report" is the assumption.
     *
     * Each sscanf is also checked (added 2026-09-22). Previously the return
     * value was ignored, so a line that was present but not a number left
     * the recovered variable uninitialised and the program carried on using
     * whatever garbage was in memory. Now a non-numeric line is reported by
     * name and the program stops.
     */
    printf("\nReading student information...\n");

    FILE *inFile = fopen(DATA_FILE_NAME, "r");
    if (inFile == NULL) {
        printf("\nERROR\n");
        printf("Unable to open %s\n", DATA_FILE_NAME);
        printf("Please verify that the file exists and that you have permission to access it.\n");
        return 1;
    }

    char lineBuffer[100];
    int recoveredID;
    char recoveredName[100];
    float recoveredGPA;
    char recoveredStanding[30];
    int recoveredGrades[NUM_COURSES];

    if (fgets(lineBuffer, sizeof(lineBuffer), inFile) == NULL) {
        printf("\nERROR\n");
        printf("%s is missing or incomplete.\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }
    if (sscanf(lineBuffer, "%d", &recoveredID) != 1) {
        printf("\nERROR\n");
        printf("%s contains invalid data (Student ID).\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }

    if (fgets(recoveredName, sizeof(recoveredName), inFile) == NULL) {
        printf("\nERROR\n");
        printf("%s is missing or incomplete.\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }
    recoveredName[strcspn(recoveredName, "\n")] = '\0';

    if (fgets(lineBuffer, sizeof(lineBuffer), inFile) == NULL) {
        printf("\nERROR\n");
        printf("%s is missing or incomplete.\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }
    if (sscanf(lineBuffer, "%f", &recoveredGPA) != 1) {
        printf("\nERROR\n");
        printf("%s contains invalid data (Current GPA).\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }

    if (fgets(recoveredStanding, sizeof(recoveredStanding), inFile) == NULL) {
        printf("\nERROR\n");
        printf("%s is missing or incomplete.\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }
    recoveredStanding[strcspn(recoveredStanding, "\n")] = '\0';

    if (fgets(lineBuffer, sizeof(lineBuffer), inFile) == NULL) {
        printf("\nERROR\n");
        printf("%s is missing or incomplete.\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }
    if (sscanf(lineBuffer, "%d", &recoveredGrades[0]) != 1) {
        printf("\nERROR\n");
        printf("%s contains invalid data (Course 1 Grade).\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }

    if (fgets(lineBuffer, sizeof(lineBuffer), inFile) == NULL) {
        printf("\nERROR\n");
        printf("%s is missing or incomplete.\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }
    if (sscanf(lineBuffer, "%d", &recoveredGrades[1]) != 1) {
        printf("\nERROR\n");
        printf("%s contains invalid data (Course 2 Grade).\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }

    if (fgets(lineBuffer, sizeof(lineBuffer), inFile) == NULL) {
        printf("\nERROR\n");
        printf("%s is missing or incomplete.\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }
    if (sscanf(lineBuffer, "%d", &recoveredGrades[2]) != 1) {
        printf("\nERROR\n");
        printf("%s contains invalid data (Course 3 Grade).\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }

    if (fgets(lineBuffer, sizeof(lineBuffer), inFile) == NULL) {
        printf("\nERROR\n");
        printf("%s is missing or incomplete.\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }
    if (sscanf(lineBuffer, "%d", &recoveredGrades[3]) != 1) {
        printf("\nERROR\n");
        printf("%s contains invalid data (Course 4 Grade).\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }

    if (fgets(lineBuffer, sizeof(lineBuffer), inFile) == NULL) {
        printf("\nERROR\n");
        printf("%s is missing or incomplete.\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }
    if (sscanf(lineBuffer, "%d", &recoveredGrades[4]) != 1) {
        printf("\nERROR\n");
        printf("%s contains invalid data (Course 5 Grade).\n", DATA_FILE_NAME);
        fclose(inFile);
        return 1;
    }

    fclose(inFile);
    printf("File successfully loaded.\n");

    /* Recompute average/highest/lowest from the RECOVERED grades - the file
     * never stores these three derived values, only the five raw grades, so
     * they must be worked out again the same way they were the first time.
     * This is intentional duplication rather than a shared helper, since
     * functions are still out of scope this week. */
    int recoveredSum = recoveredGrades[0] + recoveredGrades[1] + recoveredGrades[2] + recoveredGrades[3] + recoveredGrades[4];
    float recoveredAverage = (float) recoveredSum / NUM_COURSES;

    int recoveredHighest = recoveredGrades[0];
    if (recoveredGrades[1] > recoveredHighest) recoveredHighest = recoveredGrades[1];
    if (recoveredGrades[2] > recoveredHighest) recoveredHighest = recoveredGrades[2];
    if (recoveredGrades[3] > recoveredHighest) recoveredHighest = recoveredGrades[3];
    if (recoveredGrades[4] > recoveredHighest) recoveredHighest = recoveredGrades[4];

    int recoveredLowest = recoveredGrades[0];
    if (recoveredGrades[1] < recoveredLowest) recoveredLowest = recoveredGrades[1];
    if (recoveredGrades[2] < recoveredLowest) recoveredLowest = recoveredGrades[2];
    if (recoveredGrades[3] < recoveredLowest) recoveredLowest = recoveredGrades[3];
    if (recoveredGrades[4] < recoveredLowest) recoveredLowest = recoveredGrades[4];

    /* Question #6 - yes, the recovered report exactly matches the original
     * Student Summary's format and fields, so the employee can compare them
     * line by line. */
    printf("\nRecovered Student Record\n");
    printf("----------------------------------------\n\n");
    printf("%s\n", PROGRAM_TITLE);
    printf("Version %s\n", VERSION_NUMBER);
    printf("----------------------------------------\n\n");
    printf("Student ID : %d\n", recoveredID);
    printf("Student Name : %s\n", recoveredName);
    printf("Current GPA : %.2f\n", recoveredGPA);
    printf("Academic Standing : %s\n", recoveredStanding);
    printf("----------------------------------------\n");
    printf("Course Grades\n\n");
    printf("Course 1 : %d\n", recoveredGrades[0]);
    printf("Course 2 : %d\n", recoveredGrades[1]);
    printf("Course 3 : %d\n", recoveredGrades[2]);
    printf("Course 4 : %d\n", recoveredGrades[3]);
    printf("Course 5 : %d\n", recoveredGrades[4]);
    printf("\n----------------------------------------\n");
    printf("Average Grade : %.2f\n", recoveredAverage);
    printf("Highest Grade : %d\n", recoveredHighest);
    printf("Lowest Grade : %d\n", recoveredLowest);

    /* Requirement #7 - let the employee verify the recovered data actually
     * matches what was originally entered, instead of just eyeballing two
     * printouts side by side. strcmp is a standard library function (like
     * printf or fopen), not a user-defined one, so comparing strings this
     * way stays within this week's "no functions" constraint. */
    int recordsMatch = 1;
    if (recoveredID != studentID) recordsMatch = 0;
    if (strcmp(recoveredName, studentName) != 0) recordsMatch = 0;
    if (recoveredGPA != gpaRounded) recordsMatch = 0;
    if (strcmp(recoveredStanding, standingText) != 0) recordsMatch = 0;
    if (recoveredGrades[0] != courseGrades[0]) recordsMatch = 0;
    if (recoveredGrades[1] != courseGrades[1]) recordsMatch = 0;
    if (recoveredGrades[2] != courseGrades[2]) recordsMatch = 0;
    if (recoveredGrades[3] != courseGrades[3]) recordsMatch = 0;
    if (recoveredGrades[4] != courseGrades[4]) recordsMatch = 0;

    printf("\n----------------------------------------\n");
    if (recordsMatch) {
        printf("Verification: Recovered data matches the original data entered.\n");
    } else {
        printf("Verification: WARNING - recovered data does NOT match the original data entered.\n");
    }

    return 0;
}
