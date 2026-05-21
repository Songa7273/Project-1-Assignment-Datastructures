#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_NAME 64

typedef struct StudentRecord {
    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    double grade;
} StudentRecord;

typedef struct DuplicateRecord {
    StudentRecord record;
    struct DuplicateRecord *next;
} DuplicateRecord;

typedef struct BSTNode {
    StudentRecord record;
    DuplicateRecord *duplicates;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

static void trim_whitespace(char *text) {
    char *start = text;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    char *end = start + strlen(start);
    while (end > start && isspace((unsigned char)end[-1])) {
        end--;
    }

    size_t length = (size_t)(end - start);
    memmove(text, start, length);
    text[length] = '\0';
}

static BSTNode *create_bst_node(const StudentRecord *record) {
    BSTNode *node = malloc(sizeof(BSTNode));
    if (!node) {
        return NULL;
    }

    node->record = *record;
    node->duplicates = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void append_duplicate(BSTNode *node, const StudentRecord *record) {
    DuplicateRecord *duplicate = malloc(sizeof(DuplicateRecord));
    if (!duplicate) {
        return;
    }

    duplicate->record = *record;
    duplicate->next = NULL;

    if (!node->duplicates) {
        node->duplicates = duplicate;
        return;
    }

    DuplicateRecord *current = node->duplicates;
    while (current->next) {
        current = current->next;
    }
    current->next = duplicate;
}

static BSTNode *insert_bst(BSTNode *root, const StudentRecord *record) {
    if (!root) {
        return create_bst_node(record);
    }

    int cmp = strcmp(record->lastName, root->record.lastName);
    if (cmp < 0) {
        root->left = insert_bst(root->left, record);
    } else if (cmp > 0) {
        root->right = insert_bst(root->right, record);
    } else {
        append_duplicate(root, record);
    }

    return root;
}

static void free_bst(BSTNode *root) {
    if (!root) {
        return;
    }

    free_bst(root->left);
    free_bst(root->right);

    DuplicateRecord *duplicate = root->duplicates;
    while (duplicate) {
        DuplicateRecord *next = duplicate->next;
        free(duplicate);
        duplicate = next;
    }

    free(root);
}

static int parse_student_line(const char *line, StudentRecord *record) {
    char buffer[MAX_LINE];
    char *first;
    char *last;
    char *grade_text;
    char *endptr;

    strncpy(buffer, line, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    first = strtok(buffer, "|");
    last = strtok(NULL, "|");
    grade_text = strtok(NULL, "|");

    if (!first || !last || !grade_text || strtok(NULL, "|") != NULL) {
        return 0;
    }

    trim_whitespace(first);
    trim_whitespace(last);
    trim_whitespace(grade_text);

    if (first[0] == '\0' || last[0] == '\0' || grade_text[0] == '\0') {
        return 0;
    }

    double grade = strtod(grade_text, &endptr);
    while (*endptr && isspace((unsigned char)*endptr)) {
        endptr++;
    }
    if (*endptr != '\0') {
        return 0;
    }

    strncpy(record->firstName, first, MAX_NAME - 1);
    record->firstName[MAX_NAME - 1] = '\0';
    strncpy(record->lastName, last, MAX_NAME - 1);
    record->lastName[MAX_NAME - 1] = '\0';
    record->grade = grade;
    return 1;
}

static BSTNode *load_records_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open input file");
        return NULL;
    }

    BSTNode *root = NULL;
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), file)) {
        StudentRecord record;
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') {
            continue;
        }

        if (!parse_student_line(line, &record)) {
            fprintf(stderr, "Skipping malformed record: %s", line);
            continue;
        }

        root = insert_bst(root, &record);
        if (!root) {
            fprintf(stderr, "Memory allocation failed while building the BST.\n");
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return root;
}

static BSTNode *search_bst(BSTNode *root, const char *lastName) {
    if (!root) {
        return NULL;
    }

    int cmp = strcmp(lastName, root->record.lastName);
    if (cmp == 0) {
        return root;
    }
    if (cmp < 0) {
        return search_bst(root->left, lastName);
    }
    return search_bst(root->right, lastName);
}

static void print_record(const StudentRecord *record) {
    printf("%s | %s | %.2f\n", record->firstName, record->lastName, record->grade);
}

static void print_matches(BSTNode *node) {
    if (!node) {
        printf("No student records found for that last name.\n");
        return;
    }

    print_record(&node->record);
    DuplicateRecord *duplicate = node->duplicates;
    while (duplicate) {
        print_record(&duplicate->record);
        duplicate = duplicate->next;
    }
}

int main(void) {
    const char *filename = "students.txt";
    BSTNode *root = load_records_from_file(filename);
    if (!root) {
        fprintf(stderr, "No valid records were loaded.\n");
        return 1;
    }

    char lastName[MAX_NAME];
    printf("Enter the last name to search: ");
    if (!fgets(lastName, sizeof(lastName), stdin)) {
        free_bst(root);
        return 1;
    }
    lastName[strcspn(lastName, "\n")] = '\0';
    trim_whitespace(lastName);

    if (lastName[0] == '\0') {
        printf("Invalid last name.\n");
        free_bst(root);
        return 1;
    }

    BSTNode *result = search_bst(root, lastName);
    if (!result) {
        printf("No record found for last name: %s\n", lastName);
    } else {
        print_matches(result);
    }

    free_bst(root);
    return 0;
}
