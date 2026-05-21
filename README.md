# Project 1 - Data Structures Assignment

This repository contains four small C programs for the assignment questions.

## Files

- `question1_quicksort_students.c` - reads student records from `students.txt`, sorts them with Quick Sort, and writes `sorted_students.txt`.
- `question2_doubly_linked_bus_route.c` - models a bus route with a doubly linked list.
- `question3_binary_tree_relations.c` - builds a binary tree from 68 integers and prints tree relationships.
- `question4_bst_student_search.c` - builds a BST from `students.txt` and searches by last name.
- `analysis_and_explanations.md` - short written analysis for the assignment.
- `students.txt` - sample input file used by Question 1 and Question 4.

## How to run

Use any C compiler such as `gcc` or `clang`.

```bash
gcc question1_quicksort_students.c -o q1
gcc question2_doubly_linked_bus_route.c -o q2
gcc question3_binary_tree_relations.c -o q3
gcc question4_bst_student_search.c -o q4
```

Then run the program you want:

```bash
./q1
./q2
./q3
./q4
```

On Windows, you can run the `.exe` files instead.

## Notes

- Question 1 and Question 4 both read from `students.txt`.
- Question 3 asks for 68 integers from the user.
- Question 2 uses a 3-second delay while traversing the route.
