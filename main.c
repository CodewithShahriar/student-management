
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
    int batch;
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
         // Center "Metropolitan University" at the top of the menu
        printf("\n%*s\n", 25 + strlen("Metropolitan University") / 2, "Metropolitan University");
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
                    printf("ID can't be a negative number\n");
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

                    printf("Give student's batch: ");
                    scanf("%d", &student.batch);
                    getchar();

                    printf("Give student's grade(out of 4): ");
                    scanf("%g", &student.grade);
                    getchar();
                    while(student.grade<0 || student.grade>20){
                        printf("Give student's grade(out of 4): ");
                        scanf("%g", &student.grade);
                        getchar();
                    }

                    printf("Give student section : ");
                    scanf("%c", &student.section);
                    getchar();
                    while(!isalpha(student.section)){
                        printf("Please enter a valid section: ");
                        scanf("%c", &student.section);
                        getchar();
                    }


                    printf("\nsize = %d\n", size);
                    indexRec.recNo = size;
                    RecBSTInsert(&Root, indexRec);
                    fprintf(fp,"%d, %s, %s, %c, %d, %g%c", student.id, student.firstname, student.lastname,
                            student.section, student.batch, student.grade, '\n');

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
            printf("Print students data by grade: \n");
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
    printf("4. Print students data by grade\n");
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

void PrintStudent(int RecNum){
    FILE *infile;
    int nscan, lines;
    char termch;
    StudentT student;

    lines = 0;

    infile = fopen("students_data.dat", "r");
    if (infile == NULL)
        printf("Can't open students_data.dat\n");
    else{
        while(lines <= RecNum){
            nscan = fscanf(infile,"%d, %20[^,], %20[^,], %c, %d, %g%c", &student.id, student.firstname, student.lastname, &student.section, &student.batch, &student.grade, &termch);
            if (nscan == EOF) break;
            if (nscan != 7 || termch != '\n'){
                printf("Improper file format\n");
                break;
            }
            else
                lines++;
        }
        if(lines != RecNum)
            printf("%d, %s, %s, %c, %d, %0.2g\n", student.id, student.firstname, student.lastname, student.section, student.batch, student.grade);
    }
    fclose(infile);
}

void printStudentsWithGrade(){
    FILE *infile;
    int nscan;
    char termch;
    StudentT student;
    float theGrade;

    printf("Give the grade: ");
    scanf("%g", &theGrade);
    while(theGrade<0){
        printf("Grade can't be a negative number\n");
        printf("Give the grade: ");
        scanf("%g", &theGrade);
    }

    infile = fopen("students_data.dat", "r");
    if (infile == NULL)
        printf("Can't open students_data.dat\n");
    else{
        while(TRUE){
            nscan = fscanf(infile,"%d, %20[^,], %20[^,], %c, %d, %g%c", &student.id, student.firstname, student.lastname, &student.section, &student.batch, &student.grade, &termch);
            if (nscan == EOF) break;
            if (nscan != 7 || termch != '\n'){
                printf("Improper file format\n");
                break;
            }
            else
                if(student.grade >= theGrade)
                    printf("%d, %s, %s, %c, %d, %g\n", student.id, student.firstname, student.lastname, student.section, student.batch, student.grade);
        }
    }
    fclose(infile);
}
