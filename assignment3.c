#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// A01372608

typedef struct person{
    char *fname;
    char *lname;
    int id;
    double salary;
} Person;

typedef struct listNode{
    struct person *tPerson;
    struct listNode *next;
} ListNode;

// Function headers
void printErrorExit();
void innitList(ListNode *head, int *option);
void basicPrint(ListNode *head);
void insertStudent(ListNode *head, Person *newPerson);
void freeList(ListNode *head);
void exportList(int *option, ListNode *head);

int checkName(char *name);
int checkId(int idNum);
int checkSalary(float salary);

FILE *in;
FILE *out;

/**
 * main:            Drives the program.
 *                  Assumes two operands are required.
 * param argc:      Argument count 
 * param *argv[]:   Arguments themselves 
*/
int main(int argc, char *argv[]){
    
    // Lab setup
    if (argc != 3)
        return 1;

    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");

    // Important variables
    int option = 0, *optionP;

    ListNode *head = malloc(sizeof(struct listNode));   // Actual list head
    head->tPerson = NULL;                              // make NULL
    head->next = NULL;                                  // make NULL

    optionP = &option;

    // Bad file open
    if (in == NULL || out == NULL)
        return 1;


    innitList(head, optionP);
    basicPrint(head);               // basic test print function
    // exportList(optionP, head);
    
    freeList(head);

    return 0;
}

/**
 * printErrorExit:      Prints 'exit' to out file and closes files. Terminates program.
*/
void printErrorExit() {
    if (out != NULL) {
        if (in != NULL) fclose(in);
        fprintf(out, "error\n");
        fclose(out);
    } else {
        printf("printErrorExit called incorrectly.\n");
    }
    exit(1);
}

/**
 * inserStudentBasic:       Basic insert student.
 * 
*/
void insertStudentBasic(ListNode *head, Person *newPerson) {
    ListNode *newNode = malloc(sizeof(struct listNode));
    newNode->tPerson = newPerson;
    newNode->next = NULL;

    if (head->next == NULL) {
        head->next = newNode;
        return;
    }

    ListNode *itr = head;
    while (itr->next != NULL) {
        itr = itr->next;
    }
    itr->next = newNode;
    return;
}

/**
 * innitList:               reads the file input, creates student structures and inserts them into the list.
 * param *studentCount:     the number of numbers
 * param *option:           the option that the program will do
*/
void innitList(ListNode *head, int *option) {
    char check = 0; // blank slate
    int ncount = 0; // student count

    // Determine the number of students
    while ((check = fgetc(in)) != EOF) {
        if (check == '\n') {
            ncount++;
        }
    }
    ncount-=1;                                       // ensure correct number of students (subtract option and E).

    char buff[100];       
    fseek(in, 2, SEEK_SET);                          // set buffer to the beginning of student list.
    int track = 0;

    while (fgets(buff, 100, in) != NULL && track < ncount) {
        char *fNameT = malloc(15 * sizeof(char));
        char *lNameT = malloc(15 * sizeof(char));
        int idT = 0;
        float salaryT = 0;

        if (fNameT == NULL || lNameT == NULL)                  // Variable malloc success check
            printErrorExit();

        int stat = sscanf(buff, "%d,%[^ ] %[^,],%f\n", 
            &idT, (char*)fNameT, (char*)lNameT, &salaryT);    // Keep track of success

        if (stat != 4)
            printErrorExit();                                                   // There must be 5 things in each line

        if (1) {

            Person *newPerson = malloc(sizeof(struct person));

            if (newPerson == NULL)
                printErrorExit();

            newPerson->lname = lNameT;
            newPerson->fname = fNameT;
            newPerson->id = idT;
            newPerson->salary = salaryT;
            insertStudentBasic(head, newPerson);

        } else {
            printErrorExit();
        }

        track++;
    }
}

/**
 * freeList:                Frees memory used by the linkedlist.
 * param *head:             the start of the list   
*/
void freeList(ListNode *head) {
    ListNode *itr = head->next;

    while (itr != NULL) {
        ListNode *temp = itr->next;
        free(itr->tPerson);
        free(itr);
        itr = temp;
    }
}

/**
 * basicPrint:              basic print function, prints all student information. For testing purposes.
 * param *head:             the start of the list
*/
void basicPrint(ListNode *head) {
    ListNode *itr = head->next;

    while(itr != NULL) {
        Person *cPerson = itr->tPerson;
        printf("ID: %d %s %s Salary: %f\n", 
            cPerson->id, cPerson->fname, cPerson->lname, 
            cPerson->salary);

        itr = itr->next;
    }
}