
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    int id;
    int recNo;
}BinTreeElementType;

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
    int year;
    float grade;
}StudentT;

void CreateBST(BinTreePointer *Root);
boolean BSTEmpty(BinTreePointer Root);
void RecBSTInsert(BinTreePointer *Root, BinTreeElementType Item);
void RecBSTSearch(BinTreePointer Root, BinTreeElementType KeyValue, boolean *Found, BinTreePointer *LocPtr);
void RecBSTInorder(BinTreePointer Root);
void menu(int *choice);
int BuildBST(BinTreePointer *Root);
void PrintStudent(int RecNum);
void printStudentsWithGrade();

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
            fp = fopen("students_data.dat","a");
            if (fp == NULL){
                printf("Can't open students_data.dat\n");
                exit(1);
            }
            else{
                printf("Give student's ID: ");
                scanf("%d", &student.id);
                getchar();
                while(student.id<0){
                    printf("id can't be a negative number\n");
                    printf("Give student's ID: ");
                    scanf("%d", &student.id);
                    getchar();
                }
                indexRec.id = student.id;
                RecBSTSearch(Root, indexRec, &found, &LocPtr);
                if(found == FALSE){
                    printf("Give student firstname: ");
                    fgets(student.firstname,20,stdin);
                    student.firstname[strlen(student.firstname)-1]='\0';

                    printf("Give student lastname: ");
                    fgets(student.lastname,20,stdin);
                    student.lastname[strlen(student.lastname)-1]='\0';

                    printf("Give student's registration year: ");
                    scanf("%d", &student.year);
                    getchar();

                    printf("Give student's grade(0-20): ");
                    scanf("%g", &student.grade);
                    getchar();
                    while(student.grade<0 || student.grade>20){
                        printf("Give student's grade(0-20): ");
                        scanf("%g", &student.grade);
                        getchar();
                    }

                    printf("Give student section : ");
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
                    fprintf(fp,"%d, %s, %s, %c, %d, %g%c", student.id, student.firstname, student.lastname,
                            student.section, student.year, student.grade, '\n');

                    size++;
                    fclose(fp);
                }
                else
                    printf("A student with the same id is already in the tree");
            }
        }
        if(choice == 2){
            printf("Give student's id: ");
            scanf("%d", &student.id);
            indexRec.id = student.id;
            RecBSTSearch(Root, indexRec, &found, &LocPtr);
            if(found == TRUE){
                indexRec.recNo = LocPtr->Data.recNo;
                PrintStudent(indexRec.recNo);
            }
            else
                printf("There is no student with this id");
        }
        if(choice == 3){
            printf("Print all students (id, Number of record) ascending order(id): \n");
            RecBSTInorder(Root);
            printf("\n");
        }
        if(choice == 4){
            printf("Print students with a >= given grade: \n");
            printStudentsWithGrade();
        }
    }while(choice!=5);

    system("PAUSE");
}

void menu(int *choice){
    printf("\n                  MENU                  \n");
    printf("-------------------------------------------------\n");
    printf("1. Insert new student\n");
    printf("2. Search for a student\n");
    printf("3. Print all students (Traverse Inorder)\n");
    printf("4. Print students with a >= given grade\n");
    printf("5. Quit\n");
    printf("\nChoice: ");
    do{
        scanf("%d", choice);
        getchar();
    } while (*choice<1 && *choice>4);
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

void RecBSTInorder(BinTreePointer Root){
    if (Root!=NULL){
        RecBSTInorder(Root->LChild);
        printf("(%d, %d), ", Root->Data.id, Root->Data.recNo);
        RecBSTInorder(Root->RChild);
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
            nscan = fscanf(fp,"%d, %20[^,], %20[^,], %c, %d, %g%c", &student.id, student.firstname, student.lastname, &student.section, &student.year, &student.grade, &termch);
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

void PrintStudent(int RecNum){
    FILE *infile;
    int nscan, lines = 0;
    char termch;
    StudentT student;

    infile = fopen("students_data.dat", "r");
    if(infile == NULL)
        printf("Can't open students_data.dat\n");
    else{
        while(lines <= RecNum){
            nscan = fscanf(infile, "%d, %20[^,], %20[^,], %c, %d, %f%c", &student.id, student.firstname, student.lastname, &student.section, &student.year, &student.grade, &termch);
            if(nscan == EOF) break;
            if(nscan != 7 || termch != '\n'){
                printf("Improper file format\n");
                break;
            }
            else
                lines++;
        }
        if(lines == RecNum + 1)  // Print only if the correct record is reached
            printf("ID: %d, Name: %s %s, Section: %c, Registration Year: %d, Grade: %.2f\n", student.id, student.firstname, student.lastname, student.section, student.year, student.grade);
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
                           &student.section, &student.year, &student.grade, 
                           &termch);
            if (nscan == EOF) break;
            if (nscan != 7 || termch != '\n') {
                printf("Improper file format\n");
                break;
            }
            // Check for exact grade match
            if (student.grade == theGrade) {
                printf("ID: %d, Name: %s %s, Section: %c, Registration Year: %d, Grade: %.2f\n", 
                       student.id, student.firstname, student.lastname, 
                       student.section, student.year, student.grade);
                found = TRUE;
            }
        }
        if (!found) {
            printf("No students found with grade = %.2f\n", theGrade);
        }
    }
    fclose(infile);
}
