#include <stdio.h>
#include <stdlib.h>

#define NODE_COUNT 68

int main(void) {
    int *tree = malloc(sizeof(int) * NODE_COUNT);
    if (!tree) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d distinct integers to build the binary tree:\n", NODE_COUNT);
    for (int i = 0; i < NODE_COUNT; i++) {
        if (scanf("%d", &tree[i]) != 1) {
            fprintf(stderr, "Invalid input. Expected %d integers.\n", NODE_COUNT);
            free(tree);
            return 1;
        }
    }

    printf("Root node: %d\n", tree[0]);

    printf("Leaf nodes: ");
    for (int i = NODE_COUNT / 2; i < NODE_COUNT; i++) {
        printf("%d ", tree[i]);
    }
    printf("\n");

    int target;
    printf("Enter a node value to inspect: ");
    if (scanf("%d", &target) != 1) {
        fprintf(stderr, "Invalid node value.\n");
        free(tree);
        return 1;
    }

    int index = -1;
    for (int i = 0; i < NODE_COUNT; i++) {
        if (tree[i] == target) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Node %d was not found in the tree.\n", target);
        free(tree);
        return 0;
    }

    printf("Selected node: %d\n", tree[index]);

    printf("Parent: ");
    if (index == 0) {
        printf("None (selected node is the root)\n");
    } else {
        printf("%d\n", tree[(index - 1) / 2]);
    }

    printf("Siblings: ");
    if (index == 0) {
        printf("None\n");
    } else {
        int sibling = (index % 2 == 0) ? index - 1 : index + 1;
        if (sibling < NODE_COUNT) {
            printf("%d\n", tree[sibling]);
        } else {
            printf("None\n");
        }
    }

    printf("Grandchildren: ");
    int found = 0;
    int child1 = 2 * index + 1;
    int child2 = 2 * index + 2;
    int grandchildren[4] = {2 * child1 + 1, 2 * child1 + 2, 2 * child2 + 1, 2 * child2 + 2};
    for (int i = 0; i < 4; i++) {
        if (grandchildren[i] < NODE_COUNT) {
            printf("%d ", tree[grandchildren[i]]);
            found = 1;
        }
    }
    if (!found) {
        printf("None");
    }
    printf("\n");

    free(tree);
    return 0;
}
