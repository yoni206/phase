#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

// Maximum line length buffer (1MB). Increase if dealing with extremely long individual lines.
const unsigned long MAX_LINE_BUFFER = 16*1048576 ;

// Structure to hold L:K pairs
typedef struct {
    int spaces; // L
    int count;  // K
} StatPair;

// Global variables for dynamic statistics array
StatPair *stats = NULL;
int stats_count = 0;
int stats_capacity = 0;

// Function to add or update statistics
void add_stat(int spaces) {
    // 1. Search if this space count (L) already exists
    for (int i = 0; i < stats_count; i++) {
        if (stats[i].spaces == spaces) {
            stats[i].count++;
            return;
        }
    }

    // 2. If not found, add new entry
    if (stats_count >= stats_capacity) {
        // Resize array
        int new_capacity = (stats_capacity == 0) ? 4 : stats_capacity * 2;
        StatPair *new_stats = realloc(stats, new_capacity * sizeof(StatPair));
        if (!new_stats) {
            fprintf(stderr, "Error: Memory allocation failed.\n");
            exit(1);
        }
        stats = new_stats;
        stats_capacity = new_capacity;
    }

    stats[stats_count].spaces = spaces;
    stats[stats_count].count = 1;
    stats_count++;
}

// Comparison function for qsort (sort by spaces/L ascending)
int compare_stats(const void *a, const void *b) {
    StatPair *statA = (StatPair *)a;
    StatPair *statB = (StatPair *)b;
    return (statA->spaces - statB->spaces);
}

// Helper to exit with message
void error_exit(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    FILE *fp = stdin;
    char *filename = NULL;

    // 1. Determine Input Source
    if (argc > 1) {
        filename = argv[1];
        fp = fopen(filename, "r");
        if (!fp) {
            fprintf(stderr, "Error: File '%s' not found.\n", filename);
            return 1;
        }
    }

    char *buffer = NULL;
    int V = 0, C = 0;
    int header_found = 0;
    int clauses_processed = 0;

    if (NULL==(buffer = (char *)malloc(MAX_LINE_BUFFER))) {
        error_exit("Error: Memory allocation failed.");
    }

    // 2. Parse Header
    if (fgets(buffer, MAX_LINE_BUFFER, fp)) {
        // Strip newline
        buffer[strcspn(buffer, "\n")] = 0;

        // Tokenize
        char *token = strtok(buffer, " \t\r");
        if (!token) error_exit("Error: File is empty.");

        // Check 'p'
        if (strcmp(token, "p") != 0) error_exit("Error: Invalid header format. Expected 'p cnf V C'.");

        // Check 'cnf'
        token = strtok(NULL, " \t\r");
        if (!token || strcmp(token, "cnf") != 0) error_exit("Error: Invalid header format. Expected 'p cnf V C'.");

        // Check V
        token = strtok(NULL, " \t\r");
        if (!token) error_exit("Error: Invalid header format. Missing V.");
        char *endptr;
        long val_v = strtol(token, &endptr, 10);
        if (*endptr != '\0') error_exit("Error: V in header must be an integer.");
        V = (int)val_v;

        // Check C
        token = strtok(NULL, " \t\r");
        if (!token) error_exit("Error: Invalid header format. Missing C.");
        long val_c = strtol(token, &endptr, 10);
        if (*endptr != '\0') error_exit("Error: C in header must be an integer.");
        C = (int)val_c;

        header_found = 1;
    } else {
        error_exit("Error: File is empty.");
    }

    // 3. Process Clause Lines
    // Loop continues until EOF or error
    // line_num starts at 2 because header was line 1
    int line_num = 2;
    while (fgets(buffer, MAX_LINE_BUFFER, fp)) {
        clauses_processed++;

        // Validate Line Count Limit
        if (clauses_processed > C) {
            fprintf(stderr, "Error: Line count mismatch. Header specifies %d clauses, but file has extra lines (at least %d).\n", C, clauses_processed);
            if (filename) fclose(fp);
            return 1;
        }

        // Clean up newline
        buffer[strcspn(buffer, "\n")] = 0;

        // Check for empty lines (just whitespace)
        // The strtok loop handles this, but we need to check if we got ANY tokens
        char *token = strtok(buffer, " \t\r");
        if (!token) {
            fprintf(stderr, "Error: Line %d is empty or missing data.\n", line_num);
            if (filename) fclose(fp);
            return 1;
        }

        int token_count = 0;
        long last_val = -1;

        while (token) {
            token_count++;

            // Parse Integer
            char *endptr;
            long val = strtol(token, &endptr, 10);

            if (*endptr != '\0') {
                fprintf(stderr, "Error: Line %d contains non-integer value '%s'.\n", line_num, token);
                if (filename) fclose(fp);
                return 1;
            }

            // Check Bounds [-V, +V]
            if (val > V || val < -V) {
                fprintf(stderr, "Error: Line %d contains number %ld out of bounds [-%d, %d].\n", line_num, val, V, V);
                if (filename) fclose(fp);
                return 1;
            }

            last_val = val;
            token = strtok(NULL, " \t\r");
        }

        // Check if line ends with 0
        if (last_val != 0) {
            fprintf(stderr, "Error: Line %d does not end with 0. Read %d tokens\n", line_num, token_count);
            if (filename) fclose(fp);
            return 1;
        }

        // Calculate spaces
        // Spaces = Number of tokens - 1
        int num_spaces = token_count - 1;
        
        add_stat(num_spaces);

        line_num++;
    }

    // 4. Final Line Count Validation
    if (clauses_processed < C) {
        fprintf(stderr, "Error: Line count mismatch. Header specifies %d clauses, but file only has %d clause lines.\n", C, clauses_processed);
        if (filename) fclose(fp);
        return 1;
    }

    // 5. Generate Output
    // Sort statistics by L (spaces)
    qsort(stats, stats_count, sizeof(StatPair), compare_stats);

    // Print V C
    printf("%d %d", V, C);

    // Print pairs L:K
    for (int i = 0; i < stats_count; i++) {
        printf(" %d:%d", stats[i].spaces, stats[i].count);
    }
    printf("\n");

    // Cleanup
    if (stats) free(stats);
    if (filename) fclose(fp);

    return 0;
}
