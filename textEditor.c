#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

void edit_line(char* buffer, int current_line) {
    // Locate the start of the target line
    char* line_start = buffer;
    for (int i = 0; i < current_line; i++) {
        line_start = strchr(line_start, '\n');
        if (!line_start) {
            fprintf(stderr, "Error: Line %d does not exist.\n", current_line);
            return;
        }
        line_start++; // Move to the next line
    }

    // Find the end of the line
    char* end_line = strchr(line_start, '\n');
    if (!end_line) {
        end_line = line_start + strlen(line_start); // Last line might not have '\n'
    }

    // Save the part of the buffer after the target line
    char saved[BUFFER_SIZE] = {0};
    strncpy(saved, end_line, BUFFER_SIZE - 1);

    // Get user input for the new line
    printf("Enter new line content: ");
    if (fgets(line_start, BUFFER_SIZE - (line_start - buffer), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read input.\n");
        return;
    }

    // Remove the newline character added by fgets
    line_start[strcspn(line_start, "\n")] = '\0';

    // Append the saved content back
    strncat(line_start, "\n", BUFFER_SIZE - strlen(line_start) - 1);
    strncat(line_start, saved, BUFFER_SIZE - strlen(line_start) - 1);
}

int main(int argc, char** argv){
 if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror("Error opening file");
        return 1;
    }

    char buffer[BUFFER_SIZE] = {0};
    fread(buffer, 1, BUFFER_SIZE - 1, f);
    fclose(f);

    printf("Content:\n%s\n", buffer);

    int current_line = 0;
    printf("Enter line number to edit (starting from 0): ");
    if (scanf("%d", &current_line) != 1 || current_line < 0) {
        fprintf(stderr, "Error: Invalid line number.\n");
        return 1;
    }

    // Clear stdin buffer after scanf
    while (getchar() != '\n');

    edit_line(buffer, current_line);

    f = fopen(argv[1], "w");
    if (!f) {
        perror("Error opening file for writing");
        return 1;
    }

    fwrite(buffer, strlen(buffer), 1, f);
    fclose(f);

    printf("File updated successfully.\n");
    return 0;
}