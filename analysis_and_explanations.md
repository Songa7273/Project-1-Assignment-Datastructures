# Assignment Analysis

## Question 1: Quick Sort for Student Records

The program reads the students from `students.txt`, stores them in an array, sorts them using Quick Sort, and writes the result to `sorted_students.txt`.

The order is ascending by grade. If two students have the same grade, they are ordered by first name.

Let $T(n)$ be the time for $n$ records.

- Best case: $$T(n) = 2T(n/2) + cn = O(n \log n)$$
- Average case: $$T(n) = 2T(n/2) + cn = O(n \log n)$$
- Worst case: $$T(n) = T(n-1) + cn = O(n^2)$$

Quick Sort is better than Insertion Sort here because the file may contain many records. Insertion Sort becomes slow for large input, but Quick Sort is usually much faster on bigger datasets.

## Question 2: Doubly Linked List Bus Route

The program stores each bus stop in a doubly linked list, so it can move forward and backward through the route. The user can also add a new stop at the end.

The 3-second delay is done with `Sleep(3000)` on Windows or `sleep(3)` on other systems.

Adding a new stop at the end takes $O(1)$ time because the program keeps a tail pointer. It does not need to walk through the whole list.

## Question 3: Binary Tree from 68 Integers

The tree is stored as a complete binary tree in an array. For an index $i$:

- left child is $2i + 1$
- right child is $2i + 2$

This makes the code simple and easy to follow.

The program prints the root node, all leaf nodes, the parent, siblings, and grandchildren of a selected value. If the value is not found, it prints a message instead of crashing.

Using this level-order style, inserting a new node takes $O(n)$ time because the program may need to scan for the next open position.

## Question 4: BST for Student Search

The program reads student records from the file and builds a BST using the last name as the key. If two students have the same last name, the extra records are stored in a small duplicate list.

Searching in a BST is:

- Best case: $O(1)$
- Average case: $O(\log n)$
- Worst case: $O(n)$

A BST is better than a normal binary tree for searching because the values are ordered. Linear search through the file is always $O(n)$, so for repeated lookups the BST is the better choice.
