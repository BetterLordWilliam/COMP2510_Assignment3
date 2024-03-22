#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// A01372608, A01343016

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
void printErrorExit(ListNode *head);
void innitList(ListNode *head, int *option);
void basicPrint(ListNode *head);
void insertPerson(ListNode *head, Person *newPerson);
void freeList(ListNode *head);
void exportList(ListNode *head);

int checkName(char *name);
int checkId(ListNode *head, int idNum);
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
    head->tPerson = NULL;                               // make NULL
    head->next = NULL;                                  // make NULL

    optionP = &option;

    // Bad file open
    if (in == NULL || out == NULL)
        return 1;


    innitList(head, optionP);
    // basicPrint(head);               // basic test print function
    exportList(head);
    
    freeList(head);

    return 0;
}

/**
 * printErrorExit:      Prints 'exit' to out file and closes files. Terminates program.
*/
void printErrorExit(ListNode* head) {
    if (out != NULL) {
        if (in != NULL) fclose(in);
        fprintf(out, "error\n");
        fclose(out);
    } else {
        printf("printErrorExit called incorrectly.\n");
    }
    freeList(head);
    exit(1);
}

/**
 * checkId:             Checks if a grade is in the valid range.
 * param id:            the grade that is going to be checked
*/
int checkId(ListNode *head, int id) {

    if (head->next != NULL) {
        ListNode *itr = head->next;

        // Check for duplicates
        while (itr != NULL) {
            if (itr->tPerson->id == id)
                return 0;
            itr = itr->next;
        }
    }

    // Check for negative
    if (!(id >= 0))
        return 0;

    return 1;
}

/**
 * checkSalary:         Checks that a salary is numeric and greater than 0
 * param salary:        salary we are checking
*/
int checkSalary(float salary) {
    if (!(salary >= 0))
        return 0;

    return 1;
}

/**
 * checkName:           Checks if a name contains only alphabetical characters and dashes.
 * param *name:         name we are checking
*/
int checkName(char *name) {
    if(name == NULL)
        return 0;
    
    int pos = 0;

    while (*(name + pos) != '\0') {
        // character is alphabetical OR a dash
        if (!((*(name + pos) >= 'a' && *(name + pos) <= 'z') 
            || (*(name + pos) >= 'A' && *(name + pos) <= 'Z')
            || *(name + pos) == '-'))
            return 0;    

        pos++;
    }

    return 1;
}

/**
 * inserPersonBasic:       Insert person into the list.
 * 
 * UNUSED
*/
void insertPersonBasic(ListNode *head, Person *newPerson) {
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
 * comparePerson:           compares two persons to see what order they should fall in.
 * param *a:                first student
 * param *b:                second student
*/
int comparePerson(Person *a, Person *b) {
    int firstNameComparison = strcmp(a->fname, b->fname);
    if (firstNameComparison != 0) return firstNameComparison;

    int lastNameComparison = strcmp(a->lname, b->lname);
    if (lastNameComparison != 0) return lastNameComparison;

    return a->salary - b->salary;
}

/**
 * inserStudent:            Basic person into the list.
*/
void insertPerson(ListNode *head, Person *newPerson) {
    ListNode* newNode = malloc(sizeof(struct listNode));
    
    if (newNode == NULL)
        printErrorExit(head);

    newNode->tPerson = newPerson;
    newNode->next = NULL;

    if (head->next == NULL || comparePerson(newPerson, head->next->tPerson) < 0) {
        newNode->next = head->next;
        head->next = newNode;
        // The list is empty, put the person at the start
    } else {
        ListNode* current = head;
        while (current->next != NULL && comparePerson(newPerson, current->next->tPerson) > 0) {
            current = current->next;
        } // Go to the end of the list so long as the preson comes after the current person
        newNode->next = current->next;
        current->next = newNode;
    }
}

/**
 * innitList:               reads the file input, creates student structures and inserts them into the list.
 * param *studentCount:     the number of numbers
 * param *option:           the option that the program will do
*/
void innitList(ListNode *head, int *option) {
    char check = 0; // blank slate
    int ncount = 0; // student count
    int pcount = 0;
    int eF = 0;

    // Determine the number of students
    while ((check = fgetc(in)) != EOF) {
        if (check == '\n') {
            ncount++;
            pcount++;
        }
    }           
    ncount -= 1;                                     // ensure correct number of persons (subtract option and E).
    if (ncount == 0)
        printErrorExit(head);

    char buff[100];       
    fseek(in, 0, SEEK_SET);                          // set buffer to the beginning of student list.
    int track = 0;

    while (fgets(buff, 100, in) != NULL && track < pcount) {
        char *fNameT = malloc(15 * sizeof(char));
        char *lNameT = malloc(15 * sizeof(char));
        int idT = 0;
        float salaryT = 0;

        if (fNameT == NULL || lNameT == NULL)                   // Variable malloc success check
            printErrorExit(head);
        int stat = sscanf(buff, "%d,%[^ ] %[^,],%f\n", 
            &idT, (char*)fNameT, (char*)lNameT, &salaryT);      // Keep track of success

        // Input ends with E, we are done
        if (track == pcount - 1 && strcmp("E\n", buff) == 0) {
            eF = 1;
        }
        // Error with data in the file
        // There must be 4 things in each line
        if (stat != 4 && eF != 1) {
            printErrorExit(head);  
        }

        if (checkId(head, idT) && checkName(lNameT) 
            && checkName(fNameT) && checkSalary(salaryT)) {
            
            // Get memory for the new person
            Person *newPerson = malloc(sizeof(struct person));
            if (newPerson == NULL)
                printErrorExit(head);

            // Assign the person parts
            newPerson->lname = lNameT;
            newPerson->fname = fNameT;
            newPerson->id = idT;
            newPerson->salary = salaryT;
            insertPerson(head, newPerson);

        } else {
            printErrorExit(head);
        }

        track++;
    }

    // If the end of the file indicator 'E' was not detected, kill the program
    if (eF == 0)
        printErrorExit(head);
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

/**
 * exportList:              Puts the list into the output file.
 * param *head:             the start of the list we are going to export
*/
void exportList(ListNode *head) {
    if (out != NULL) {
        ListNode *itr = head->next;
        if (itr == NULL)
            return;

        while (itr != NULL) {
            Person *cS = itr->tPerson;
            fprintf(out, "%d,%s %s,%.2f\n", 
                cS->id, cS->fname, cS->lname, cS->salary);
            itr = itr->next;
        }

    } else {
        printErrorExit(head);
    }
}