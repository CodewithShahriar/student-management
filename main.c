#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PASSWORD "faculty123"

// Structure for student information
typedef struct Student {
    int studentID;
    char firstName[50];
    char lastName[50];
    char batch[20];
    char section[5];
    char grade[5];
} Student;

// Structure for midterm marks
typedef struct Marks {
    int studentID;
    float midMarks;
} Marks;

// Function prototypes
void insertStudent();
void searchStudent();
void printAllStudents();
void insertMidMarks();
void studentMarksDatabase();
int verifyPassword();
void userMenu();
void viewerMenu();

// Function to insert a new student and save to file
void insertStudent() {
    Student student;
    FILE *file = fopen("students_data.dat", "ab");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d", &student.studentID);
    printf("Enter First Name: ");
    scanf("%s", student.firstName);
    printf("Enter Last Name: ");
    scanf("%s", student.lastName);
    printf("Enter Batch: ");
    scanf("%s", student.batch);
    printf("Enter Section: ");
    scanf("%s", student.section);
    printf("Enter Grade: ");
    scanf("%s", student.grade);

    fwrite(&student, sizeof(Student), 1, file);
    fclose(file);
    printf("Student record inserted successfully.\n");
}

// Function to search for a student by ID from file
void searchStudent() {
    int studentID;
    Student student;
    FILE *file = fopen("students_data.dat", "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Student ID to search: ");
    scanf("%d", &studentID);

    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.studentID == studentID) {
            printf("ID: %d, Name: %s %s, Batch: %s, Section: %s, Grade: %s\n",
                   student.studentID, student.firstName, student.lastName, student.batch, student.section, student.grade);
            fclose(file);
            return;
        }
    }
    printf("Student ID %d not found.\n", studentID);
    fclose(file);
}

// Function to print all students from file
void printAllStudents() {
    Student student;
    FILE *file = fopen("students_data.dat", "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("All Students:\n");
    while (fread(&student, sizeof(Student), 1, file)) {
        printf("ID: %d, Name: %s %s, Batch: %s, Section: %s, Grade: %s\n",
               student.studentID, student.firstName, student.lastName, student.batch, student.section, student.grade);
    }
    fclose(file);
}

// Function to insert midterm marks for a student and save to file
void insertMidMarks() {
    Marks marks;
    FILE *file = fopen("mark_database.dat", "ab");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d", &marks.studentID);
    printf("Enter Mid Marks: ");
    scanf("%f", &marks.midMarks);

    fwrite(&marks, sizeof(Marks), 1, file);
    fclose(file);
    printf("Mid marks inserted successfully.\n");
}

// Function to display student marks database from file
void studentMarksDatabase() {
    Marks marks;
    FILE *file = fopen("mark_database.dat", "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Student Marks Database:\n");
    while (fread(&marks, sizeof(Marks), 1, file)) {
        printf("ID: %d, Mid Marks: %.2f\n", marks.studentID, marks.midMarks);
    }
    fclose(file);
}

// Password verification function
int verifyPassword() {
    char password[50];
    printf("Enter password: ");
    scanf("%s", password);
    return strcmp(password, PASSWORD) == 0;
}

// Main menu function for User section
void userMenu() {
    int choice;
    do {
        printf("\nUser Menu:\n");
        printf("1. Insert new student\n");
        printf("2. Search for a student\n");
        printf("3. Print all students\n");
        printf("4. Quit\n");
        printf("5. Insert Mid Marks\n");
        printf("6. Student Marks Database\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                insertStudent();
                break;
            case 2:
                searchStudent();
                break;
            case 3:
                printAllStudents();
                break;
            case 4:
                printf("Exiting User Menu.\n");
                return;
            case 5:
                insertMidMarks();
                break;
            case 6:
                studentMarksDatabase();
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 4);
}

// Main menu function for Viewer section
void viewerMenu() {
    int choice;
    do {
        printf("\nViewer Menu:\n");
        printf("1. Search for a student\n");
        printf("2. Student Marks Database\n");
        printf("3. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                searchStudent();
                break;
            case 2:
                studentMarksDatabase();
                break;
            case 3:
                printf("Exiting Viewer Menu.\n");
                return;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 3);
}

// Main function
int main() {
    int mainChoice;

    while (1) {
        printf("\nMain Menu:\n");
        printf("1. User\n");
        printf("2. Viewer\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &mainChoice);

        if (mainChoice == 1) {
            if (verifyPassword()) {
                userMenu();
            } else {
                printf("Wrong password. Try again.\n");
            }
        } else if (mainChoice == 2) {
            viewerMenu();
        } else if (mainChoice == 3) {
            printf("Exiting program.\n");
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
