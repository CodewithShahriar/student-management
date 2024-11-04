#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    int id;
    int recNo;
} BinTreeElementType;

typedef struct BinTreeNode *BinTreePointer;

struct BinTreeNode{
    BinTreeElementType Data;
    BinTreePointer LChild, RChild;
} ;


typedef enum{
    FALSE, TRUE
} boolean;

typedef struct{
    int id;
    char firstname[20];
    char lastname[20];
    char section;
    int batch;
    float grade;
} StudentT;

void CreateBST(BinTreePointer *Root);
boolean BSTEmpty(BinTreePointer Root);
void RecBSTInsert(BinTreePointer *Root, BinTreeElementType Item);
void RecBSTSearch(BinTreePointer Root, BinTreeElementType KeyValue, boolean *Found, BinTreePointer *LocPtr);
void RecBSTInorder(BinTreePointer Root);
void menu(int *choice);
int BuildBST(BinTreePointer *Root);
void PrintStudent(int RecNum);
void printStudentsWithGrade();
void InsertMidMarks();
void StudentMarksDatabase();

int main(){
    FILE *fp;
    boolean found;
    BinTreePointer Root, LocPtr;
    int choice, size;
    BinTreeElementType indexRec;
    StudentT student;

    size = BuildBST(&Root);
    do{
        menu(&choice);
        if(choice == 1){
            // Existing Insert Student option
            fp = fopen("students_data.dat","a");
            if (fp == NULL){
                printf("Can't open students_data.dat\n");
                exit(1);
            }
            else{
                printf("Give student's ID: ");
                scanf("%d", &student.id);
                getchar();
                while(student.id < 0){
                    printf("ID can't be a negative number\n");
                    printf("Give student's ID: ");
                    scanf("%d", &student.id);
                    getchar();
                }
                indexRec.id = student.id;
                RecBSTSearch(Root, indexRec, &found, &LocPtr);
                if(found == FALSE){
                    printf("Give student firstname: ");
                    fgets(student.firstname, 20, stdin);
                    student.firstname[strlen(student.firstname) - 1] = '\0';

                    printf("Give student lastname: ");
                    fgets(student.lastname, 20, stdin);
                    student.lastname[strlen(student.lastname) - 1] = '\0';

                    printf("Give student's batch: ");
                    scanf("%d", &student.batch);
                    getchar();

                    printf("Give student's grade (out of 4): ");
                    scanf("%f", &student.grade);
                    getchar();
                    while(student.grade < 0 || student.grade > 20){
                        printf("Give student's grade (out of 4): ");
                        scanf("%f", &student.grade);
                        getchar();
                    }

                    printf("Give student section: ");
                    scanf("%c", &student.section);
                    getchar();

                    // Validation to ensure section is between 'A' and 'Z'
                    while(student.section < 'A' || student.section > 'Z'){
                        printf("Section must be a letter between A and Z.\n");
                        printf("Give student section (A-Z): ");
                        scanf("%c", &student.section);
                        getchar();
                    }

                    printf("\nsize = %d\n", size);
                    indexRec.recNo = size;
                    RecBSTInsert(&Root, indexRec);
                    fprintf(fp, "%d, %s, %s, %c, %d, %.2f\n", student.id, student.firstname, student.lastname,
                            student.section, student.batch, student.grade);

                    size++;
                    fclose(fp);
                }
                else
                    printf("A student with the same ID is already in the tree.\n");
            }
        }
        else if(choice == 2){
            // Existing Search Student option
            printf("Give student's ID: ");
            scanf("%d", &student.id);
            indexRec.id = student.id;
            RecBSTSearch(Root, indexRec, &found, &LocPtr);
            if(found == TRUE){
                indexRec.recNo = LocPtr->Data.recNo;
                PrintStudent(indexRec.recNo);
            }
            else
                printf("There is no student with this ID.\n");
        }
        else if(choice == 3) {
            // Existing Print All Students option
            printf("Print all students (ID, Name, Section, Batch, Grade) in ascending order:\n");
            RecBSTInorder(Root);
            printf("\n");
        }
        else if(choice == 4){
            // Existing Print Students With Grade option
            printf("Print students with a >= given grade: \n");
            printStudentsWithGrade();
        }
        else if(choice == 6){
            // New Insert Mid Marks option
            InsertMidMarks();
        }
        else if(choice == 7){
            // New Student Marks Database option
            StudentMarksDatabase();
        }
    } while(choice != 5);

    system("PAUSE");
}

void menu(int *choice){
    printf("\n                  MENU                  \n");
    printf("-------------------------------------------------\n");
    printf("1. Insert new student\n");
    printf("2. Search for a student\n");
    printf("3. Print all students \n");
    printf("4. Print students with a >= given grade\n");
    printf("5. Quit\n");
    printf("6. Insert Mid Marks\n");
    printf("7. Student Marks Database\n");
    printf("\nChoice: ");
    do{
        scanf("%d", choice);
        getchar();
    } while (*choice < 1 || *choice > 7);
}

void CreateBST(BinTreePointer *Root){*Root = NULL;}

boolean BSTEmpty(BinTreePointer Root){return (Root==NULL);}

void RecBSTInsert(BinTreePointer *Root, BinTreeElementType Item){
    if(BSTEmpty(*Root)){
        (*Root) = (BinTreePointer)malloc(sizeof (struct BinTreeNode));
        (*Root) ->Data.id = Item.id;
        (*Root) ->Data.recNo = Item.recNo;
        (*Root) ->LChild = NULL;
        (*Root) ->RChild = NULL;
    }
    else
        if (Item.id < (*Root) ->Data.id)
            RecBSTInsert(&(*Root) ->LChild, Item);
        else if (Item.id > (*Root) ->Data.id)
            RecBSTInsert(&(*Root) ->RChild, Item);
        else
            printf("This id already exists in the tree\n", Item.id);
}

void RecBSTSearch(BinTreePointer Root, BinTreeElementType KeyValue, boolean *Found, BinTreePointer *LocPtr){
    boolean stop;

    if (BSTEmpty(Root))
        *Found=FALSE;
    else
        if (KeyValue.id < Root->Data.id)
            RecBSTSearch(Root->LChild, KeyValue, &(*Found), &(*LocPtr));
        else
            if (KeyValue.id > Root->Data.id)
                RecBSTSearch(Root->RChild, KeyValue, &(*Found), &(*LocPtr));
            else{
                *Found = TRUE;
                *LocPtr = Root;
            }
}

void RecBSTInorder(BinTreePointer Root) {
    if (Root != NULL) {
        RecBSTInorder(Root->LChild); // Traverse left subtree
        
        // Print student information in the specified format
        PrintStudent(Root->Data.recNo); // Fetches and prints student data based on recNo
        
        RecBSTInorder(Root->RChild); // Traverse right subtree
    }
}


int BuildBST(BinTreePointer *Root){
    FILE *fp;
    StudentT student;
    BinTreeElementType indexRec;
    int size, nscan;
    char termch;

    CreateBST(Root);

    size = 0;
    fp = fopen("students_data.dat","r");
    if (fp == NULL){
        printf("Can't open students_data.dat\n");
        exit(1);
    }
    else{
        while(TRUE){
            nscan = fscanf(fp,"%d, %20[^,], %20[^,], %c, %d, %g%c", &student.id, student.firstname, student.lastname, &student.section, &student.batch, &student.grade, &termch);
            if (nscan == EOF) break;
            if (nscan != 7 || termch != '\n'){
                printf("Improper file format\n");
                break;
            }
            else{
                indexRec.id = student.id;
                indexRec.recNo = size;

                RecBSTInsert((Root), indexRec);
                size++;
            }
        }
    }
    fclose(fp);

    return size;
}

void PrintStudent(int RecNum) {
    FILE *infile;
    int nscan, lines = 0;
    char termch;
    StudentT student;

    infile = fopen("students_data.dat", "r");
    if (infile == NULL) {
        printf("Can't open students_data.dat\n");
    } else {
        while (lines <= RecNum) {
            nscan = fscanf(infile, "%d, %20[^,], %20[^,], %c, %d, %f%c", 
                           &student.id, student.firstname, student.lastname, 
                           &student.section, &student.batch, &student.grade, 
                           &termch);
            if (nscan == EOF) break;
            if (nscan != 7 || termch != '\n') {
                printf("Improper file format\n");
                break;
            } else {
                lines++;
            }
        }
        if (lines == RecNum + 1) { // Correctly reached the record number
            printf("ID: %d, Name: %s %s, Section: %c, Batch: %d, Grade: %.2f\n", 
                   student.id, student.firstname, student.lastname, 
                   student.section, student.batch, student.grade);
        }
    }
    fclose(infile);
}

void printStudentsWithGrade() {
    FILE *infile;
    int nscan;
    char termch;
    StudentT student;
    float theGrade;
    boolean found = FALSE;

    printf("Give the specific grade: ");
    scanf("%f", &theGrade);
    while (theGrade < 0 || theGrade > 20) {
        printf("Grade must be between 0 and 20.\n");
        printf("Give the specific grade: ");
        scanf("%f", &theGrade);
    }

    infile = fopen("students_data.dat", "r");
    if (infile == NULL) {
        printf("Can't open students_data.dat\n");
    } else {
        printf("Students with grade = %.2f:\n", theGrade);
        while (TRUE) {
            nscan = fscanf(infile, "%d, %20[^,], %20[^,], %c, %d, %f%c", 
                           &student.id, student.firstname, student.lastname, 
                           &student.section, &student.batch, &student.grade, 
                           &termch);
            if (nscan == EOF) break;
            if (nscan != 7 || termch != '\n') {
                printf("Improper file format\n");
                break;
            }
            // Check for exact grade match
            if (student.grade == theGrade) {
                printf("ID: %d, Name: %s %s, Section: %c, Batch: %d, Grade: %.2f\n", 
                       student.id, student.firstname, student.lastname, 
                       student.section, student.batch, student.grade);
                found = TRUE;
            }
        }
        if (!found) {
            printf("No students found with grade = %.2f\n", theGrade);
        }
    }
    fclose(infile);
}


void InsertMidMarks() {
    FILE *fp = fopen("students_data.dat", "a");
    if (fp == NULL) {
        printf("Can't open students_data.dat\n");
        return;
    }

    int id;
    char fullName[40];
    int attendanceMarks, ctMarks, assignmentMarks, midMarks;

    printf("Enter student ID: ");
    while (scanf("%d", &id) != 1) {
        printf("Invalid input. Enter a valid numeric ID: ");
        getchar(); // Clear invalid input
    }

    getchar(); // Clear newline character
    printf("Enter student full name: ");
    fgets(fullName, sizeof(fullName), stdin);
    fullName[strcspn(fullName, "\n")] = '\0'; // Remove trailing newline

    printf("Enter Attendance Marks: ");
    while (scanf("%d", &attendanceMarks) != 1 || attendanceMarks < 0 || attendanceMarks > 99) {
        printf("Invalid input. Enter a number between 0 and 99: ");
        getchar();
    }

    printf("Enter CT Marks: ");
    while (scanf("%d", &ctMarks) != 1 || ctMarks < 0 || ctMarks > 99) {
        printf("Invalid input. Enter a number between 0 and 99: ");
        getchar();
    }

    printf("Enter Assignment Marks: ");
    while (scanf("%d", &assignmentMarks) != 1 || assignmentMarks < 0 || assignmentMarks > 99) {
        printf("Invalid input. Enter a number between 0 and 99: ");
        getchar();
    }

    printf("Enter Mid Marks: ");
    while (scanf("%d", &midMarks) != 1 || midMarks < 0 || midMarks > 99) {
        printf("Invalid input. Enter a number between 0 and 99: ");
        getchar();
    }

    fprintf(fp, "%d,%s,%d,%d,%d,%d\n", id, fullName, attendanceMarks, ctMarks, assignmentMarks, midMarks);
    fclose(fp);
}

void StudentMarksDatabase() {
    FILE *fp = fopen("students_data.dat", "r");
    if (fp == NULL) {
        printf("Can't open students_data.dat\n");
        return;
    }

    int id, searchId, attendanceMarks, ctMarks, assignmentMarks, midMarks;
    char fullName[40];
    boolean found = FALSE;

    printf("Enter student ID to search: ");
    scanf("%d", &searchId);

    // Ensure buffer is clean
    getchar();

    // Loop through each line in the file
    while (fscanf(fp, "%d,%39[^,],%d,%d,%d,%d\n", &id, fullName, &attendanceMarks, &ctMarks, &assignmentMarks, &midMarks) == 6) {
        if (id == searchId) {
            int total = attendanceMarks + ctMarks + assignmentMarks;
            printf("ID: %d, Name: %s, Attendance Marks: %d, CT Marks: %d, Assignment Marks: %d, Mid Marks: %d, Total: %d\n",
                   id, fullName, attendanceMarks, ctMarks, assignmentMarks, midMarks, total);
            found = TRUE;
            break;
        }
    }

    if (!found) {
        printf("No record found for ID %d\n", searchId);
    }

    fclose(fp);
}