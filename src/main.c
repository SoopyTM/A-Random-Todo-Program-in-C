#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Get user input for the menu choice and validate it.
// It ensures the input is an integer between 1 and 4.
// If the input is invalid, it prompts the user to try again.
// This function returns the valid choice as an integer.
int getUserChoice() {
    char input[32];
    int choice;
    while (1) {
        printf("Enter your choice (1-5): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error. Try again.\n");
            continue;
        }
        // Remove newline if present
        char *newline = strchr(input, '\n');
        if (newline) *newline = '\0';

        // Validate integer input
        char extra;
        if (sscanf(input, "%d %c", &choice, &extra) == 1) {
            if (choice >= 1 && choice <= 5) {
                return choice;
            } else {
                printf("Please enter a number between 1 and 4.\n");
            }
        } else {
            printf("Invalid input. Please enter an integer.\n");
        }
    }
}

// Function to read tasks from the file and print them to the console.
// It reads each line, extracts the ID and task description, and prints them in a formatted way.
// If the file cannot be opened, it prints an error message.
int readFile(){
        FILE *file = fopen("tasks.json", "r");
        if (file == NULL) {
                perror("Error opening file");
                return 1;
        }

        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
                int id;
                char task[256];
                if (sscanf(buffer, " { \"ID\": %d, \"Task\": \"%255[^\"]\" }", &id, task) == 2) {
                        printf("%d: %s\n", id, task);
                }
        }

        fclose(file);
        return 0;
}

// Function to add a new task to the file.
// It reads the last ID from the file, increments it, and appends a new task in JSON format.
// If the file cannot be opened, it prints an error message.
int addTask(){
        FILE *file = fopen("tasks.json", "r");
        int lastID = -1;
        char line[512];

        // Find the last ID in the file
        if (file != NULL) {
                while (fgets(line, sizeof(line), file) != NULL) {
                        int id;
                        if (sscanf(line, " { \"ID\": %d,", &id) == 1) {
                                lastID = id;
                        }
                }
                fclose(file);
        }

        int newID = lastID + 1;

        file = fopen("tasks.json", "a");
        if (file == NULL) {
                perror("Error opening file");
                return 1;
        }

        char task[256];
        printf("Enter the task to add: ");
        if (fgets(task, sizeof(task), stdin) == NULL) {
                printf("Input error. Task not added.\n");
                fclose(file);
                return 1;
        }

        // Remove newline if present
        char *newline = strchr(task, '\n');
        if (newline) *newline = '\0';

        // Write the task in JSON format
        fprintf(file, " { \"ID\": %d, \"Task\": \"%s\" }\n", newID, task);
        fclose(file);
        printf("Task added successfully.\n");
        return 0;
}

// Function to delete a task from the file.
// It prompts the user for the ID of the task to delete, searches for it in the file,
// and removes it if found. If the task is not found, it prints an appropriate message.
int deleteTask() {
        int id;
        printf("Enter the ID of the task to delete: ");
        if (scanf("%d", &id) != 1) {
                printf("Invalid input. Please enter a valid ID.\n");
                return 1;
        }
        // Clear input buffer to avoid affecting getUserChoice
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);

        FILE *file = fopen("tasks.json", "r");
        if (file == NULL) {
                perror("Error opening file");
                return 1;
        }

        FILE *tempFile = fopen("temp.json", "w");
        if (tempFile == NULL) {
                perror("Error creating temporary file");
                fclose(file);
                return 1;
        }

        char line[512];
        int found = 0;

        while (fgets(line, sizeof(line), file) != NULL) {
                int currentID;
                if (sscanf(line, " { \"ID\": %d,", &currentID) == 1) {
                        if (currentID != id) {
                                fputs(line, tempFile);
                        } else {
                                found = 1;
                                printf("Task with ID %d deleted successfully.\n", id);
                        }
                } else {
                        fputs(line, tempFile); // Write lines that do not match the ID
                }
        }

        fclose(file);
        fclose(tempFile);

        if (!found) {
                printf("No task found with ID %d.\n", id);
                remove("temp.json");
                return 1;
        }

        // Replace the original file with the temporary file
        remove("tasks.json");
        rename("temp.json", "tasks.json");

        return 0;
}

int createTasksFile(){
        FILE *file = fopen("tasks.json", "w");
        if (file == NULL) {
                perror("Error opening file");
                return 1;
        }

	printf("File Created\n");

	return 0;

}

// Main function to run the Todo List application.
// It initializes the tasks file, displays the menu, and handles user choices for reading, adding, and deleting tasks.
int main() {
//        FILE *file = fopen("tasks.json", "w");
//        if (file == NULL) {
//                perror("Error opening file");
//                return 1;
//        }
//        fclose(file);

        while (1) {
                printf("What would you like to do?\n");
                printf("1. Read tasks\n");
                printf("2. Add task\n");
                printf("3. Delete task\n");
                printf("4. Create/wipe tasks file\n");
                printf("5. Exit\n");

                int choice = getUserChoice();

                switch (choice) {
                        case 1:
                                printf("Reading tasks...\n");
                                printf("--------------------\n");
                                // Call the function to read tasks from the file, and prints them to screen.
                                readFile();
                                printf("--------------------\n");
                                break;
                        
                        case 2:
                                // Call the function to add a task to the file.
                                addTask();
                                break;
                        
                        case 3:
                                // Call the function to delete a task from the file.
                                deleteTask();
                                break;

			case 4:
				printf("Creating File\n");
				createTasksFile();
				break;

                        case 5:
                                printf("Exiting the program.\n");
                                // Exit's the program with status code 0.
                                exit(0);
                                break;
                }
        }
        return 0;
}
