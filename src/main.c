#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "student.h"

int main(int argc, char *argv[]) // η main ξεκινά την εκτέλεση του προγράμματος
{
    int choice, studentID, check;     // μεταβλητή επιλογής, ID φοιτητή και προσωρινή μεταβλητή ελέγχου
    student firstYear = {"", 0};      // δημιούργησε έναν student με προεπιλεγμένες πληροφορίες
    student *stdPtr = &firstYear;     // δημιούργησε και αρχικοποίησε δείκτη σε student
    list studentList = load(argv[1]); // φόρτωσε την λίστα φοιτητών

    // λίστα για αρχικοποίηση του studentList
    struct listR startupList = {NULL, NULL};

    printMenu();
    choice = enterChoice(argv[1], studentList);

    while (1) // το πρόγραμμα τερματίζει μόνο αν ο χρήστης εισάγει EOF
    {
        // εκτέλεσε την κατάλληλη λειτουργία ανάλογα με την επιλογή του χρήστη
        switch (choice)
        {
        case 1:
            // δημιούργησε αρχείο κειμένου
            textFile(argv[1], studentList);
            break;

        case 2:
            studentID = enterID(argv[1], studentList);
            stdPtr = findStudent(studentID, studentList);

            // εάν ο φοιτητής δεν βρέθηκε εμφάνισε μήνυμα
            if (stdPtr == NULL)
            {
                printf(">>> Student with ID: %0*d doesn't exist.\n\n", FIELD_WIDTH, studentID);
            }
            else // διαφορετικά
            {
                printf("\n%s", "____________\nSTUDENT INFO\n____________\n");
                print(*stdPtr); // εκτύπωσε τα στοιχεία του φοιτητή
            }
            break;

        case 3:
            // εκτύπωσε την λίστα φοιτητών
            printstudents(studentList);
            break;

        case 4:
            printRules(0);       // εκτύπωσε όλους τους κανόνες
            stdPtr = &firstYear; // ο stdPtr δείχνει στον firstYear

            stdPtr->id = enterID(argv[1], studentList);
            enterName(stdPtr->name, argv[1], studentList);

            if (studentList == NULL)        // εάν η λίστα είναι άδεια
                studentList = &startupList; // αρχικοποίησε την λίστα

            // επιτυχής προσθήκη νέου φοιτητή
            if ((check = addStudent(*stdPtr, studentList)) == 1)
            {
                puts(">>> Student successfully added to the list.\n");
            }
            else if (check == 0) // ανεπιτυχής προσθήκη νέου φοιτητή, μη διαθέσιμος χώρος
            {
                printf(">>> Student \"%s\" with ID: %0*d not inserted. No memory available.\n\n",
                       stdPtr->name, FIELD_WIDTH, stdPtr->id);
            }
            else if (check == -1) // ανεπιτυχής προσθήκη νέου φοιτητή, μη έγκυρο ID
                ;

            break;

        case 5:
            studentID = enterID(argv[1], studentList);
            stdPtr = findStudent(studentID, studentList);

            // εάν ο φοιτητής δεν βρέθηκε εμφάνισε μήνυμα
            if (stdPtr == NULL)
                printf(">>> Student with ID: %0*d doesn't exist.\n\n", FIELD_WIDTH, studentID);
            else // διαφορετικά αφαίρεσε τον κόμβο που περιέχει τον φοιτητή
            {
                if (deleteStudent(*stdPtr, studentList) == -1) // εάν διαγράφηκε ο μοναδικός κόμβος
                    studentList = NULL;                        // άδειασε την λίστα
                puts(">>> Student successfully deleted.\n");
            }
            break;

        case 6:
            studentID = enterID(argv[1], studentList);
            stdPtr = findStudent(studentID, studentList);

            // εάν ο φοιτητής δεν βρέθηκε ενημέρωσε τον χρήστη
            if (stdPtr == NULL)
                printf(">>> Student with ID: %0*d doesn't exist.\n\n", FIELD_WIDTH, studentID);
            else // διαφορετικά
                printf("%s\n\n", ">>> Student was found.");
            break;

        case 7:
            printf("\n%s", "# Search for student to update.");
            studentID = enterID(argv[1], studentList);
            stdPtr = findStudent(studentID, studentList);

            // εάν ο φοιτητής δεν βρέθηκε εμφάνισε μήνυμα
            if (stdPtr == NULL)
                printf(">>> Student with ID: %0*d doesn't exist.\n\n", FIELD_WIDTH, studentID);
            else // διαφορετικά
            {
                // επιτυχής ανανέωση φοιτητή
                if ((check = updateStudent(*stdPtr, studentList)) == 1)
                {
                    printf("\n%s", "NOTICE\n~~~~~~\n");
                    puts(">>> Student successfully updated.\n");
                }
                else if (check == -1) // ο χρήστης εισήγαγε EOF
                {
                    save(argv[1], studentList);      // αποθήκευσε την λίστα
                    puts(">>> Program terminated."); // εμφάνισε μήνυμα
                    return EXIT_SUCCESS;             // τερμάτισε το πρόγραμμα
                }
            }
            break;
        }

        printf("%s", "$ ");
        choice = enterChoice(argv[1], studentList);
    }
}