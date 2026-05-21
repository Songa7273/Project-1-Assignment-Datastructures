#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16
#define MAX_LINE 256
#define MAX_NAME 64
#define MAX_ID 32

typedef struct {
    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    char studentId[MAX_ID];
    int grade;
} Student;

static int compare_students(const Student *a, const Student *b) {
    if (a->grade != b->grade) {
        return a->grade - b->grade;
    }

    int cmp = strcmp(a->firstName, b->firstName);
    if (cmp != 0) {
        return cmp;
    }

    cmp = strcmp(a->lastName, b->lastName);
    if (cmp != 0) {
        return cmp;
    }

    return strcmp(a->studentId, b->studentId);
}

static void swap_students(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

static int partition(Student students[], int low, int high) {
    Student pivot = students[(low + high) / 2];
    int i = low;
    int j = high;

    while (i <= j) {
        while (compare_students(&students[i], &pivot) < 0) {
            i++;
        }
        while (compare_students(&students[j], &pivot) > 0) {
            j--;
        }
        if (i <= j) {
            swap_students(&students[i], &students[j]);
            i++;
            j--;
        }
    }

    return i;
}

static void quick_sort(Student students[], int low, int high) {
    if (low >= high) {
        return;
    }

    int index = partition(students, low, high);
    if (low < index - 1) {
        quick_sort(students, low, index - 1);
    }
    if (index < high) {
        quick_sort(students, index, high);
    }
}

static int parse_student_line(const char *line, Student *student) {
    char first[MAX_NAME];
    char last[MAX_NAME];
    char id[MAX_ID];
    int grade;

    if (sscanf(line, "%63s %63s %31s %d", first, last, id, &grade) != 4) {
        return 0;
    }

    if (grade < 0 || grade > 100) {
        return 0;
    }

    strncpy(student->firstName, first, MAX_NAME - 1);
    student->firstName[MAX_NAME - 1] = '\0';
    strncpy(student->lastName, last, MAX_NAME - 1);
    student->lastName[MAX_NAME - 1] = '\0';
    strncpy(student->studentId, id, MAX_ID - 1);
    student->studentId[MAX_ID - 1] = '\0';
    student->grade = grade;
    return 1;
}

static size_t read_students(const char *filename, Student **students_out) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open input file");
        return 0;
    }

    size_t capacity = INITIAL_CAPACITY;
    size_t count = 0;
    Student *students = malloc(capacity * sizeof(Student));
    if (!students) {
        fclose(file);
        fprintf(stderr, "Memory allocation failed.\n");
        return 0;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        Student student;
        if (!parse_student_line(line, &student)) {
            if (strlen(line) > 1) {
                fprintf(stderr, "Skipping malformed record: %s", line);
            }
            continue;
        }

        if (count == capacity) {
            capacity *= 2;
            Student *tmp = realloc(students, capacity * sizeof(Student));
            if (!tmp) {
                free(students);
                fclose(file);
                fprintf(stderr, "Memory reallocation failed.\n");
                return 0;
            }
            students = tmp;
        }

        students[count++] = student;
    }

    fclose(file);
    *students_out = students;
    return count;
}

static int write_students(const char *filename, const Student students[], size_t count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Unable to open output file");
        return 0;
    }

    for (size_t i = 0; i < count; i++) {
        fprintf(file, "%s %s %s %d\n",
                students[i].firstName,
                students[i].lastName,
                students[i].studentId,
                students[i].grade);
    }

    fclose(file);
    return 1;
}

int main(void) {
    const char *input_file = "students.txt";
    const char *output_file = "sorted_students.txt";

    Student *students = NULL;
    size_t count = read_students(input_file, &students);
    if (count == 0) {
        fprintf(stderr, "No valid student records were loaded.\n");
        free(students);
        return 1;
    }

    quick_sort(students, 0, (int)count - 1);

    if (!write_students(output_file, students, count)) {
        free(students);
        return 1;
    }

    printf("Loaded %zu records from %s and wrote sorted output to %s.\n",
           count, input_file, output_file);

    free(students);
    return 0;
}
