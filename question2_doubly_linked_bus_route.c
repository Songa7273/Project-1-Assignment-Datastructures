#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
static void delay_seconds(unsigned int seconds) {
    Sleep(seconds * 1000U);
}
#else
#include <unistd.h>
static void delay_seconds(unsigned int seconds) {
    sleep(seconds);
}
#endif

#define MAX_NAME 128

typedef struct BusStop {
    char name[MAX_NAME];
    int stopNumber;
    struct BusStop *prev;
    struct BusStop *next;
} BusStop;

typedef struct {
    BusStop *head;
    BusStop *tail;
} Route;

static void trim_newline(char *text) {
    size_t len = strlen(text);
    if (len > 0 && text[len - 1] == '\n') {
        text[len - 1] = '\0';
    }
}

static void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static int read_int(const char *prompt, int *value) {
    char buffer[64];
    char *endptr;

    for (;;) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return 0;
        }

        *value = (int)strtol(buffer, &endptr, 10);
        if (endptr != buffer) {
            while (*endptr == ' ' || *endptr == '\t' || *endptr == '\n') {
                endptr++;
            }
            if (*endptr == '\0') {
                return 1;
            }
        }

        printf("Invalid number. Try again.\n");
    }
}

static void read_text(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin)) {
        trim_newline(buffer);
    } else {
        buffer[0] = '\0';
    }
}

static BusStop *create_bus_stop(const char *name, int stopNumber) {
    BusStop *stop = malloc(sizeof(BusStop));
    if (!stop) {
        return NULL;
    }

    strncpy(stop->name, name, MAX_NAME - 1);
    stop->name[MAX_NAME - 1] = '\0';
    stop->stopNumber = stopNumber;
    stop->prev = NULL;
    stop->next = NULL;
    return stop;
}

static void append_stop(Route *route, const char *name, int stopNumber) {
    BusStop *stop = create_bus_stop(name, stopNumber);
    if (!stop) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    if (!route->head) {
        route->head = route->tail = stop;
        return;
    }

    stop->prev = route->tail;
    route->tail->next = stop;
    route->tail = stop;
}

static void free_route(Route *route) {
    BusStop *current = route->head;
    while (current) {
        BusStop *next = current->next;
        free(current);
        current = next;
    }
    route->head = NULL;
    route->tail = NULL;
}

static void print_route(const Route *route) {
    const BusStop *current = route->head;
    if (!current) {
        printf("The route is empty.\n");
        return;
    }

    while (current) {
        printf("Stop %d: %s\n", current->stopNumber, current->name);
        current = current->next;
    }
}

static void traverse_forward(const Route *route) {
    const BusStop *current = route->head;
    if (!current) {
        printf("The route is empty.\n");
        return;
    }

    printf("Starting forward traversal from home to campus.\n");
    while (current) {
        printf("Stop %d: %s\n", current->stopNumber, current->name);
        if (current->next) {
            delay_seconds(3);
        }
        current = current->next;
    }
    printf("Reached the last bus stop.\n");
}

static void traverse_backward(const Route *route) {
    const BusStop *current = route->tail;
    if (!current) {
        printf("The route is empty.\n");
        return;
    }

    printf("Starting backward traversal from campus to home.\n");
    while (current) {
        printf("Stop %d: %s\n", current->stopNumber, current->name);
        if (current->prev) {
            delay_seconds(3);
        }
        current = current->prev;
    }
    printf("Reached the first bus stop.\n");
}

int main(void) {
    Route route = {NULL, NULL};
    int initialCount;

    if (!read_int("Enter the number of initial bus stops: ", &initialCount) || initialCount < 0) {
        fprintf(stderr, "Invalid number of bus stops.\n");
        return 1;
    }

    for (int i = 0; i < initialCount; i++) {
        char name[MAX_NAME];
        int stopNumber;

        printf("Bus stop %d\n", i + 1);
        read_text("  Stop name: ", name, sizeof(name));
        if (!read_int("  Stop number: ", &stopNumber)) {
            fprintf(stderr, "Invalid stop number.\n");
            free_route(&route);
            return 1;
        }

        append_stop(&route, name, stopNumber);
    }

    for (;;) {
        int choice;
        printf("\nMenu:\n");
        printf("1. Traverse forward (home -> campus)\n");
        printf("2. Traverse backward (campus -> home)\n");
        printf("3. Add a new bus stop at the end\n");
        printf("4. Show route\n");
        printf("5. Exit\n");

        if (!read_int("Select an option: ", &choice)) {
            fprintf(stderr, "Invalid menu selection.\n");
            clear_input_buffer();
            continue;
        }

        if (choice == 1) {
            traverse_forward(&route);
        } else if (choice == 2) {
            traverse_backward(&route);
        } else if (choice == 3) {
            char name[MAX_NAME];
            int stopNumber;
            read_text("Enter new stop name: ", name, sizeof(name));
            if (!read_int("Enter new stop number: ", &stopNumber)) {
                fprintf(stderr, "Invalid stop number.\n");
                continue;
            }
            append_stop(&route, name, stopNumber);
            printf("Bus stop added at the end of the route.\n");
        } else if (choice == 4) {
            print_route(&route);
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid option.\n");
        }
    }

    free_route(&route);
    return 0;
}
