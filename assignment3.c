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
void printErrorExitA(Person **arr, int aSize);
void innitList(ListNode *head);
void basicPrint(ListNode *head);
void basicPrintA(Person **arr, int aSize);
void insertPerson(ListNode *head, Person *newPerson);
void sortArray(Person **arr, int aSize);
void freeList(ListNode *head);
void freeArray(Person **arr, int aSize);
void exportList(ListNode *head);
void exportArray(Person **arr, int aSize);

Person** innitListN(int *listSizeP);

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
    int listSize = 0, *listSizeP;
    listSizeP = &listSize;
    ListNode *head = malloc(sizeof(struct listNode));   // Actual list head
    head->tPerson = NULL;                               // make NULL
    head->next = NULL;                                  // make NULL

    // Bad file open
    if (in == NULL || out == NULL)
        return 1;

    Person **arr = innitListN(listSizeP);
    basicPrintA(arr, *listSizeP);
    sortArray(arr, *listSizeP);
    exportArray(arr, *listSizeP);
    freeArray(arr, *listSizeP);

    return 0;
}

/**
 * printErrorExit:      Prints 'exit' to out file and closes files. Terminates program.
*/
/* void printErrorExit(ListNode* head) {
    if (out != NULL) {
        if (in != NULL) fclose(in);
        fprintf(out, "error\n");
        fclose(out);
    } else {
        printf("printErrorExit called incorrectly.\n");
    }
    freeList(head);
    exit(1);
} */ // --> LinkedList version of the code LEGACY, NOT USED.

/**
 * printErrorExit:      Prints 'exit' to out file and closes files. Terminates program.
*/
void printErrorExitA(Person **arr, int aSize) {
    if (out != NULL) {
        if (in != NULL) fclose(in);
        fprintf(out, "error\n");
        fclose(out);
    } else {
        printf("printErrorExit called incorrectly.\n");
    }
    freeArray(arr, aSize);
    exit(1);
}

/**
 * checkId:             Checks if a grade is in the valid range.
 * param id:            the grade that is going to be checked
*/
/* int checkId(ListNode *head, int id) {

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
} */ // --> LinkedList version of the code LEGACY, NOT USED.

/**
 * checkId:             Checks if a grade is in the valid range.
 * param id:            the grade that is going to be checked
*/
int checkIdN(Person **arr, int aSize, int id) {

    if (arr[0] != NULL) {
        // Check for duplicates
        for(int i = 0; i < aSize; i++) {
            if (arr[i]->id == id)
                return 0;
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
/* void insertPersonBasic(ListNode *head, Person *newPerson) {
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
} */ // --> LinkedList version of the code LEGACY, NOT USED.

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
 * sortArray:               Sorts an array of people based of criteria.
 * param **arr:             Person array
 * param aSize:             Size of the person array
*/
void sortArray(Person **arr, int aSize) {
    for (int i = 0; i < aSize; i++) {
        Person *smallest = arr[i];
        int j = i;
        int s = 0;
        for (; j < aSize; j++) {
            if (comparePerson(smallest, arr[j]) >= 0) {
                smallest = arr[j];
                s = j;
            }
        }
        Person *temp = arr[s];
        arr[s] = arr[i];
        arr[i] = temp;
    }
}

/**
 * inserStudent:            Basic person into the list.
*/
/* void insertPerson(ListNode *head, Person *newPerson) {
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
} */ // --> LinkedList version of the code LEGACY, NOT USED.

/**
 * innitList:               reads the file input, creates student structures and inserts them into the list.
 * param *studentCount:     the number of numbers
 * param *option:           the option that the program will do
*/
/* void innitList(ListNode *head) {
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
            break;
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
} */ // --> LinkedList version of the code LEGACY, NOT USED.

/**
 * innitListN:              reads the file input, creates student structures and puts them into an array.
 *                          Returns the array.
 * param *listSizeP:        the pointer to the variable which will tell the array size
*/
Person** innitListN(int *listSizeP) {
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
    *listSizeP = ncount;
    Person **arr = malloc(sizeof(Person*) * ncount);

    if (ncount == 0 || arr == NULL)
        printErrorExitA(arr, *listSizeP);

    char buff[100];       
    fseek(in, 0, SEEK_SET);                          // set buffer to the beginning of student list.
    int track = 0;

    while (fgets(buff, 100, in) != NULL && track < pcount) {
        char *fNameT = malloc(15 * sizeof(char));
        char *lNameT = malloc(15 * sizeof(char));
        int idT = 0;
        float salaryT = 0;

        if (fNameT == NULL || lNameT == NULL)                   // Variable malloc success check
            printErrorExitA(arr, *listSizeP);
        int stat = sscanf(buff, "%d,%[^ ] %[^,],%f\n", 
            &idT, (char*)fNameT, (char*)lNameT, &salaryT);      // Keep track of success

        // Input ends with E, we are done
        if (track == pcount - 1 && strcmp("E\n", buff) == 0) {
            eF = 1;
            break;
        }

        // Error with data in the file
        // There must be 4 things in each line
        if (stat != 4 && eF != 1) {
            printErrorExitA(arr, *listSizeP);
            
        }

        if (checkIdN(arr, *listSizeP, idT) && checkName(lNameT) 
            && checkName(fNameT) && checkSalary(salaryT)) {
            
            // Get memory for the new person
            Person *newPerson = malloc(sizeof(struct person));
            if (newPerson == NULL) {
                printErrorExitA(arr, *listSizeP);
            }

            // Assign the person parts
            newPerson->lname = lNameT;
            newPerson->fname = fNameT;
            newPerson->id = idT;
            newPerson->salary = salaryT;
            arr[track] = newPerson;

        } else {
            printErrorExitA(arr, *listSizeP);
        }

        track++;
    }

    // If the end of the file indicator 'E' was not detected, kill the program
    if (eF == 0)
        printErrorExitA(arr, *listSizeP);
    return arr;
}

/**
 * freeList:                Frees memory used by the linkedlist.
 * param *head:             the start of the list   
*/
/* void freeList(ListNode *head) {
    ListNode *itr = head->next;

    while (itr != NULL) {
        ListNode *temp = itr->next;
        free(itr->tPerson);
        free(itr);
        itr = temp;
    }
} */ // --> LinkedList version of the code LEGACY, NOT USED.

/**
 * freeArray:               Frees memory used by array.
 * param **arr:             The array
 * param aSize:             The size of the array.
*/
void freeArray(Person **arr, int aSize) {
    for (int i = 0; i < aSize; i++) {
        if (arr[i] != NULL) {
            free(arr[i]);
        }
    }
    free(arr);
}

/**
 * basicPrint:              basic print function, prints all student information. For testing purposes.
 * param *head:             the start of the list
*/
/* void basicPrint(ListNode *head) {
    ListNode *itr = head->next;

    while(itr != NULL) {
        Person *cPerson = itr->tPerson;
        printf("ID: %d %s %s Salary: %f\n", 
            cPerson->id, cPerson->fname, cPerson->lname, 
            cPerson->salary);

        itr = itr->next;
    }
} */ // --> LinkedList version of the code LEGACY, NOT USED.

/**
 * basicPrint:              basic print function for the array version of the code.
 * param **arr:             pointer which is the array
*/
void basicPrintA(Person **arr, int aSize) {
    for (int i = 0; i < aSize; i++) {
        if (arr[i] != NULL) {
            Person *cS = arr[i];
            printf("%d,%s %s,%.2f\n", 
                cS->id, cS->fname, cS->lname, cS->salary);
        }
    }
}

/**
 * exportList:              Puts the list into the output file.
 * param *head:             the start of the list we are going to export
*/
/* void exportList(ListNode *head) {
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
} */ // --> LinkedList version of the code LEGACY, NOT USED.

/**
 * exportArray:             Puts the array contents into the output file.
 * param **arr:             Array of person pointers who's information we want to put into the file.
*/
void exportArray(Person **arr, int aSize) {
    if (out != NULL) {
        for (int i = 0; i < aSize; i++) {
            Person *cS = (Person *) arr[i];
            fprintf(out, "%d,%s %s,%.2f\n", 
                cS->id, cS->fname, cS->lname, cS->salary);
        }

    } else {
        printErrorExitA(arr, aSize);
    }
}