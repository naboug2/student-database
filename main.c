/*------------------------------------------- 
Student Database
System: VS Code on MacOS
Author: Nuha Abougoash
------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
	char* name;
	char* id;
	double gpa;
	int creditHours;
} Student;

typedef struct StudentNode{
	Student* pStudent;
	struct StudentNode* pNext;
} StudentNode;

typedef struct {
	StudentNode* pIDList;
	StudentNode* pHonorRollList;
	StudentNode* pAcademicProbationList;
	StudentNode* pFreshmanList;
	StudentNode* pSophomoreList;
	StudentNode* pJuniorList;
	StudentNode* pSeniorList;
} Database;

typedef int (*CompareFunc)(Student*, Student*);

Database* initDatabase();
void addStudent(Database* db, Student* student);
Student* createStudent(char* name, char* id, double gpa, int creditHours);
void readStudentsFromFile(Database* db, char* filename);
void deleteStudent(Database* db, char* id);
void freeDatabase(Database* db);
StudentNode* createStudentNode(Student* student);
StudentNode* sortedInsert(StudentNode* head, StudentNode* newNode, int (*compare)(Student*, Student*));
int compareByID(Student* a, Student* b);
int compareByGPA(Student* a, Student* b);
int compareByName(Student* a, Student* b);
StudentNode* findAndRemove(StudentNode** pHead, char* id);
void displayStudent(StudentNode* studentNode);
void displayMenuAndExecute(Database* db);
void displayHead(Database* db);
void displayHonorRoll(Database* db);
void displayAcademicProbation(Database* db);
void displayFreshmen(Database* db);
void displaySophomores(Database* db);
void displayJuniors(Database* db);
void displaySeniors(Database* db);
void displayStudentByID(Database* db);
void addStudentFromFile(Database* db, Student* student);
Student* createStudentFromInput();
void deleteIDList(StudentNode** studentList, const char* id);

#define MAX_ID_LENGTH 10
#define MAX_NAME_LENGTH 100

// initializes a new empty database and returns pointer to it
Database* initDatabase() {
    // allocate memory for new database structure
    Database* db = (Database*) malloc(sizeof(Database));

    // check if memory allocation succesful
    if (db == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    // initialize all list pointer to null
    db->pIDList = NULL;
    db->pHonorRollList = NULL;
    db->pAcademicProbationList = NULL;
    db->pFreshmanList = NULL;
    db->pSophomoreList = NULL;
    db->pJuniorList = NULL;
    db->pSeniorList = NULL;

    // return pointer to new database
    return db;
}

Student* createStudent(char* name, char* id, double gpa, int creditHours) {
    Student* newStudent = (Student*) malloc(sizeof(Student));
    if (newStudent == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    newStudent->name = strdup(name);
    newStudent->id = strdup(id);
    newStudent->gpa = gpa;
    newStudent->creditHours = creditHours;

    return newStudent;
}


// create new studentNode and returns pointer to it
StudentNode* createStudentNode(Student* student) {
    // allocate memory
    StudentNode* newNode = (StudentNode*) malloc(sizeof(StudentNode));

    // check if allocation was succesful
    if (newNode == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    // initialize data
    newNode->pStudent = student;
    newNode->pNext = NULL;

    return newNode;
}

void addStudent(Database* db, Student* student) {
    StudentNode* newNode = createStudentNode(student);

    // add student to ID list, sorted by ID
    db->pIDList = sortedInsert(db->pIDList, newNode, compareByID);

    // add student to honor roll list, if their GPA is 3.5 or higher
    if (student->gpa >= 3.5) {
        newNode = createStudentNode(student);
        db->pHonorRollList = sortedInsert(db->pHonorRollList, newNode, compareByGPA);
    }

    // add student to the academic probation list, if GPA is below 2.0
    if (student->gpa < 2.0) {
        newNode = createStudentNode(student);
        db->pAcademicProbationList = sortedInsert(db->pAcademicProbationList, newNode, compareByGPA);
    }

    // add student to the appropriate class list, sorted by name based on credit hours
    newNode = createStudentNode(student);
    if (student->creditHours < 30) {
        // printf("Debug: student->name address: %p, length: %zu\n", student->name, strlen(student->name));
        // printf("Debug: student->id address: %p, length: %zu\n", student->id, strlen(student->id));
        db->pFreshmanList = sortedInsert(db->pFreshmanList, newNode, compareByName);
    } 
    else if (student->creditHours < 60) {
        // printf("Debug: student->name address: %p, length: %zu\n", student->name, strlen(student->name));
        // printf("Debug: student->id address: %p, length: %zu\n", student->id, strlen(student->id));
        db->pSophomoreList = sortedInsert(db->pSophomoreList, newNode, compareByName);
    } 
    else if (student->creditHours < 90) {
        // printf("Debug: student->name address: %p, length: %zu\n", student->name, strlen(student->name));
        // printf("Debug: student->id address: %p, length: %zu\n", student->id, strlen(student->id));
        db->pJuniorList = sortedInsert(db->pJuniorList, newNode, compareByName);
    } 
    else {
        // printf("Debug: student->name address: %p, length: %zu\n", student->name, strlen(student->name));
        // printf("Debug: student->id address: %p, length: %zu\n", student->id, strlen(student->id));
        db->pSeniorList = sortedInsert(db->pSeniorList, newNode, compareByName);
    }
}

//typedef int (*CompareFunc)(Student*, Student*);

// insert a new node into sorted linked list and return the new head of list
StudentNode* sortedInsert(StudentNode* head, StudentNode* newNode, CompareFunc compare) {
    // if list emtpy 
    if (head == NULL || compare(newNode->pStudent, head->pStudent) < 0) {
        newNode->pNext = head;
        return newNode;
    }

    // loop through list to find correct position for new node
    StudentNode* currentNode = head;
    while (currentNode->pNext != NULL && compare(newNode->pStudent, currentNode->pNext->pStudent) >= 0) {
        currentNode = currentNode->pNext;
    }

    // insert new node in correct position
    newNode->pNext = currentNode->pNext;
    currentNode->pNext = newNode;

    return head; // return head of updated list
}

// compare two students by their ID and return a neg, pos, zero int
int compareByID(Student* s1, Student* s2) {
    return strcmp(s1->id, s2->id);
}

// compares two students by their GPA and returns an int
// note: order is revered here compared to the compareByID function
int compareByGPA(Student* s1, Student* s2) {
    if (s1->gpa > s2->gpa) {
        return 1;
    } 
    else if (s1->gpa < s2->gpa) {
        return -1;
    } 
    else {
        return 0;
    }
}

// compare two students by their name and returns an int
int compareByName(Student* s1, Student* s2) {
    return strcmp(s1->name, s2->name);
}

// reads student info from a file and adds the student to the database
void readStudentsFromFile(Database* db, char* filename) {
    // open file for reading
    FILE* file = fopen(filename, "r");

    // check if file was opened succesfully
    if (file == NULL) {
        printf("Error: Unable to open file %s.\n", filename);
        return;
    }

    char buffer[256];

    fgets(buffer, sizeof(buffer), file);

    // read each line of file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // allocat memory for new student
        Student* student = (Student*) malloc(sizeof(Student));

        // check if allocation succesful
        if (student == NULL) {
            printf("Error: Memory allocation failed.\n");
            fclose(file);
            return;
        }
        
        // parse info from each line
        char* token = strtok(buffer, ",");
        student->name = strdup(token);

        token = strtok(NULL, ",");
        student->id = strdup(token);

        token = strtok(NULL, ",");
        student->gpa = atof(token);

        token = strtok(NULL, ",");
        student->creditHours = atoi(token);

        addStudentFromFile(db, student);
    }

    fclose(file); // close the file
}

void deleteIDList(StudentNode** studentList, const char* id) {
    if (*studentList == NULL) {
        return;
    }

    if (strcmp((*studentList)->pStudent->id, id) == 0) {
        StudentNode* temp = *studentList;
        *studentList = (*studentList)->pNext;
        free(temp);
        return;
    }

    StudentNode* prev = *studentList;
    StudentNode* current = prev->pNext;
    while (current != NULL && strcmp(current->pStudent->id, id) != 0) {
        prev = current;
        current = current->pNext;
    }

    if (current != NULL) {
        prev->pNext = current->pNext;
        free(current);
    }
}


void deleteStudent(Database* db,  char* id) {
  StudentNode* prev = NULL;
  StudentNode* current = db->pIDList;
  bool found = false;
 
  while (current != NULL) {
    if (strcmp(current->pStudent->id, id) == 0) {
      found = true;
     
      deleteIDList(&(db->pHonorRollList), id);
      deleteIDList(&(db->pAcademicProbationList), id);
      deleteIDList(&(db->pFreshmanList), id);
      deleteIDList(&(db->pSophomoreList), id);
      deleteIDList(&(db->pJuniorList), id);
      deleteIDList(&(db->pSeniorList), id);
     
      if (prev == NULL) {
        db->pIDList = current->pNext;
      } 
      else {
        prev->pNext = current->pNext;
      }
     
      free(current->pStudent->name);
      free(current->pStudent->id);
      free(current->pStudent);
      free(current);
     
      break;
    }
    prev = current;
    current = current->pNext;
  }
 
  if (!found) {
    printf("Sorry, there is no student in the db with the id %s.\n", id);
  }
}

// free memory allocated for given list of students
void freeList(StudentNode* list) {
    // loop thru list and free memory for wach student & node
    while (list != NULL) {
        StudentNode* next = list->pNext;
        free(list->pStudent->name);
        free(list->pStudent->id);
        free(list->pStudent);
        free(list);
        list = next;
    }
}

// fress memory allocated for given database
void freeDatabase(Database* db) {
    // free each list of students in database
    freeList(db->pIDList);
    freeList(db->pHonorRollList);
    freeList(db->pAcademicProbationList);
    freeList(db->pFreshmanList);
    freeList(db->pSophomoreList);
    freeList(db->pJuniorList);
    freeList(db->pSeniorList);
    // free the memory allocated for database itself
    free(db);
}

// find student node in given list with specified id, remove from list, and return it
StudentNode* findAndRemove(StudentNode** pHead, char* id) {
    StudentNode* prev = NULL;
    StudentNode* current = *pHead;

    // loop thru list to find the student with given id
    while (current != NULL) {
        if (strcmp(current->pStudent->id, id) == 0) {
            // if found, update list to remove
            if (prev != NULL) {
                prev->pNext = current->pNext;
            } 
            else {
                *pHead = current->pNext;
            }
            current->pNext = NULL;
            return current;
        }

        prev = current;
        current = current->pNext;
    }

    return NULL;
}

void displayStudent(StudentNode* studentNode) {
    if (studentNode == NULL) {
        printf("There are no students matching that criteria.\n");
        return;
    }

    Student* student = studentNode->pStudent;

    if (student == NULL || student->name == NULL || student->id == NULL) {
        printf("Invalid student data.\n");
        return;
    }

    printf("%s: \n", student->name);
    printf("    ID - %s\n", student->id);
    printf("    GPA - %.2f\n", student->gpa);
    printf("    Credit Hours - %d\n", student->creditHours);
}

// create new student from user input
Student* createStudentFromInput() {
    // Read student name from user
    printf("Enter the name of the new student: ");
    char name[100];
    fgets(name, 100, stdin);
    fgets(name, 100, stdin);
    strtok(name, "\n");

    // Read student id from user
    printf("Enter the ID of the new student: ");
    char id[100];
    scanf("%s", id);

    // Read student gpa
    printf("Enter the GPA of the new student: ");
    double gpa;
    scanf("%lf", &gpa);

    // Read student credit hours
    printf("Enter the credit hours of the new student: ");
    int creditHours;
    scanf("%d", &creditHours);

    // Create a new student with the input values and return it
    return createStudent(name, id, gpa, creditHours);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void readMenu(Database* db) {
    int option;
    bool repeat;

    printf("Select one of the following: \n");
    printf("\t1) Display the head (first 10 rows) of the database\n");
    printf("\t2) Display students on the honor roll, in order of their GPA\n");
    printf("\t3) Display students on academic probation, in order of their GPA\n");
    printf("\t4) Display freshmen students, in order of their name\n");
    printf("\t5) Display sophomore students, in order of their name\n");
    printf("\t6) Display junior students, in order of their name\n");
    printf("\t7) Display senior students, in order of their name\n");
    printf("\t8) Display the information of a particular student\n");
    clearInputBuffer(); // clear the input buffer

    while (1) {

        printf("Your choice --> ");
        scanf("%d", &option);

        repeat = false;
        
        switch (option) {
            case 1:
                displayHead(db);
                break;
            case 2:
                displayHonorRoll(db);
                break;
            case 3:
                displayAcademicProbation(db);
                break;
            case 4:
                displayFreshmen(db);
                break;
            case 5:
                displaySophomores(db);
                break;
            case 6:
                displayJuniors(db);
                break;
            case 7:
                displaySeniors(db);
                break;
            case 8:
                displayStudentByID(db);
                break;
            default:
                printf("Sorry, that input was invalid. Please try again.\n");
                repeat = true;
                break;
            }
        if (!repeat) break;
        
    }
}


void displayMenuAndExecute(Database* db) {
    char choice;

    while (1) {
        printf("\nEnter: \tC to create a new student and add them to the database,\n");
        printf("\tR to read from the database,\n");
        printf("\tD to delete a student from the database, or\n");
        printf("\tX to exit the program.\n");
        printf("Your choice --> ");
        scanf(" %c", &choice);
        
        switch (choice) {
            case 'C': {
                Student* newStudent = createStudentFromInput();
                if (newStudent != NULL) {
                    addStudent(db, newStudent);
                    printf("Successfully added the following student to the database!\n");
                    displayStudent(newStudent);
                }
                break;
            }

            case 'R': {
                readMenu(db);
                break;
            }

            case 'D': {
                char id[MAX_ID_LENGTH + 1];
                printf("Enter the ID of the student to be removed: ");
                scanf("%s", id);
                deleteStudent(db, id);
                break;
            }

            case 'X':
                printf("\nThanks for playing!\n");
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid option. Try again.\n");
                break;
        }
    }
}

// displays first 10 students in database, sorted by id
void displayHead(Database* db) {
    StudentNode* current = db->pIDList;
    int count = 0;
    int displayed = 0;

    while (current != NULL && count < 10) {
        displayStudent(current->pStudent);
        displayed = 1;
        current = current->pNext;
        count++;
    }
    if (!displayed) {
        printf("There are no students matching that criteria.\n");
    }
}

// display all student on honor roll, sorted by gpa
void displayHonorRoll(Database* db) {
    StudentNode* current = db->pHonorRollList;
    int displayed = 0;

    while (current != NULL) {
        displayStudent(current->pStudent);
        displayed = 1;
        current = current->pNext;
    }

    if (!displayed) {
        printf("There are no students matching that criteria.\n");
    }
}

// diplays all freshmam student, sorted by name
void displayAcademicProbation(Database* db) {
    StudentNode* current = db->pAcademicProbationList;
    int displayed = 0;
    while (current != NULL) {
        displayStudent(current->pStudent);
        displayed = 1;
        current = current->pNext;
    }

    if (!displayed) {
        printf("There are no students matching that criteria.\n");
    }
}

// diplays all freshman students, sorted by name
void displayFreshmen(Database* db) {
    StudentNode* current = db->pFreshmanList;
    int displayed = 0;

    while (current != NULL) {
        Student* student = current->pStudent;

        if (student->creditHours >= 0 && student->creditHours < 30) {
            displayStudent(student);
            displayed = 1;
        }
        current = current->pNext;
    }
    if (!displayed) {
        printf("There are no students matching that criteria.\n");
    }
}

// diplays all sophmore students, sorted by name
void displaySophomores(Database* db) {
    StudentNode* current = db->pSophomoreList;
    int displayed = 0;

    while (current != NULL) {
		Student* student = current->pStudent;
        if (student->creditHours >= 30 && student->creditHours < 60) {
			displayStudent(student);
            displayed = 1;
		}
        current = current->pNext;
    }
    if (!displayed) {
        printf("There are no students matching that criteria.\n");
    }
}

void displayJuniors(Database* db) {
    StudentNode* current = db->pJuniorList;
    int displayed = 0;

    while (current != NULL) {
        Student* student = current->pStudent;
        if (student != NULL && student->creditHours >= 60 && student->creditHours < 90) {
        //     printf("Debug: student->name address: %p, length: %zu\n", student->name, strlen(student->name));
        //     printf("Debug: student->id address: %p, length: %zu\n", student->id, strlen(student->id));
            displayStudent(student);
            displayed = 1;
        }
        current = current->pNext;
    }
    if (!displayed) {
        printf("There are no students matching that criteria.\n");
    }
}

// diplays all senior students, sorted by name
void displaySeniors(Database* db) {
    StudentNode* current = db->pSeniorList;
    int displayed = 0;

    while (current != NULL) {
        Student* student = current->pStudent;
		if (student->creditHours >= 90) {
			displayStudent(student);
            displayed = 1;
		}
        current = current->pNext;
    }
    if (!displayed) {
        printf("There are no students matching that criteria.\n");
    }
}

// display the student info with a given ID
void displayStudentByID(Database* db) {
    char id[MAX_ID_LENGTH + 1];
    printf("Enter the id of the student to find: ");
    scanf("%s", id);
    clearInputBuffer(); // clear the input buffer

    StudentNode* current = db->pIDList;

    while (current != NULL) {
        if (strcmp(current->pStudent->id, id) == 0) {
            displayStudent(current->pStudent);
            return;
        }
        current = current->pNext;
    }

    printf("Sorry, there is no student in the database with the ID %s.\n", id);
}


bool isEmptyStudent(Student* student) {
    return strlen(student->name) == 0 && strlen(student->id) == 0 && student->gpa == 0.0 && student->creditHours == 0;
}

// adds a student to the database when reading from a file
void addStudentFromFile(Database* db, Student* student) {
    if (!isEmptyStudent(student)) {
        addStudent(db, student);
    }
}

/// ------------------ MAIN ------------------ ///
int main() {
    printf("CS 211, Spring 2023\n");
    printf("Program 4: Database of Students\n\n");

	Database* db = initDatabase();
    char initial_choice;

    printf("Enter E to start with an empty database, \n");
    printf("or F to start with a database that has information on students from a file.\n");
    while (true) {
		printf("Your choice --> ");
		scanf(" %c", &initial_choice);

		if (initial_choice == 'F') {
			char filename[100];
			printf("Enter the name of the file you would like to use: ");
			scanf("%s", filename);
			readStudentsFromFile(db, filename);
			displayMenuAndExecute(db);
		}
		else if (initial_choice == 'E') {
			displayMenuAndExecute(db);
			break;
		}
		else {
			printf("Sorry, that input was invalid. Please try again.\n");
			continue;
		}
	}
    // free the allocated memory
    freeDatabase(db);

    return 0;
}


// printf("There are no students matching that criteria.\n\n");
