Description:

In this program, I implemented a  simplified database system to store information about students. 
The implementation make use of linked structures. The data about the students will be as follows:

● Name - the first and last name of the student
● ID - a unique identifier for each student, similar to your UIN
● GPA - the student’s overall GPA
● Credit Hours - the number of credit hours the student has completed


Running the Program:

To begin, the user may choose to start with an empty database or with information read in from a file.
If they choose to have information from a file, your program should prompt them for the filename. 
Invalid input for E or F will prompt the user to continue entering input until it is valid.

From there, the main menu is displayed. In a database, there are four main operations involved in its 
maintenance, abbreviated as CRUD: create, read, update, delete. 
If the user selects C to Create, ask them for the information for the new student. Add them to the 
database, updating the linked lists appropriately. A message is displayed, showing the user that the 
student was successfully added.
    
In the case that the user selects R to read, a subsequent menu is displayed.
As shown above, if the user types something other than 1-8, they are asked to try again.
● Menu option 1 displays a sample of the database by printing the information of the first 10 students. 
  They are ordered by their ID, so the first 10 students would be the ones whose ID comes first 
  alphanumerically.
● Menu options 2 and 3 display the students on the honor roll and on academic probation, respectively.
  The students are listed in ascending order by their GPA.
● Menu options 3-7 display the students in each class. The students are listed in ascending order by 
  their name.
● Menu option 8 allows the user to search for a particular student by ID. The program will ask them to 
  enter the ID of the student they would like to find.
 
The user may also select D from the main menu, which prompts them to enter the ID of the student they 
would like to have removed from the database. If the student is found, the linked lists should be 
updated appropriately and all associated memory should be freed.

If the input for the main menu is invalid, i.e. not C, R, D, or X, the user will be prompted to try again.