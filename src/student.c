#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

// δημιούργησε αρχείο μορφοποιημένου κειμένου για προβολή
void textFile(char *filename, list l)
{
    FILE *readPtr;  // δείκτης αρχείου γραμμής εντολών
    FILE *writePtr; // δείκτης αρχείου students.txt
    int result,     // χρησιμοποιείται για να ελεγχθεί αν η fread διαβάζει byte
        entries,    // αριθμός εγγεγραμμένων φοιτητών
        i = 0;      // μετρητής

    printf("\n%s", "NOTICE\n~~~~~~\n"); // επικεφαλίδα
    entries = saveList(filename, l);    // αποθήκευσε την λίστα φοιτητών

    // η fopen ανοίγει το αρχείο γραμμής εντολών για ανάγνωση
    if ((readPtr = fopen(filename, "rb")) == NULL)
    {
        printf(">>> File \"%s\" could not be opened.\n", filename);
        exit(EXIT_FAILURE); // έξοδος αν το αρχείο δεν ανοίγει
    }

    // δημιούργησε έναν student με προεπιλεγμένες πληροφορίες
    student freshman = {"", 0};

    // η fopen ανοίγει το αρχείο students.txt για εγγραφή
    if ((writePtr = fopen("students.txt", "w")) == NULL)
    {
        puts(">>> Could not create file \"students.txt\".");
        exit(EXIT_FAILURE); // έξοδος αν το αρχείο δεν δημιουργείται
    }
    else
    {
        // απόπειρα ανάγνωσης του πρώτου φοιτητή ώστε να λειτουργήσει σωστά η feof
        result = fread(&freshman, sizeof(student), 1, readPtr);

        // εάν η λίστα φοιτητών είναι άδεια
        if (feof(readPtr))
        {
            fprintf(writePtr, "%s\n", "______________\nList is empty!\n______________");
            printf("%s\n", ">>> Text file \"students.txt\" successfully created.\n");
            fclose(readPtr);  // η fclose κλείνει το αρχείο γραμμής εντολών
            fclose(writePtr); // η fclose κλείνει το αρχείο students.txt
            return;
        }
        else
            rewind(readPtr); // επέστρεψε τον stdPtr στην αρχή του αρχείου

        // επικεφαλίδα
        fprintf(writePtr, "%s\n%s\n", "ID | Student Name", "~~~~~~~~~~~~~~~~~");

        // αντέγραψε όλες τις εγγραφές απ' το αρχείο τυχαίας προσπέλασης στο αρχείο κειμένου
        while (!feof(readPtr))
        {
            result = fread(&freshman, sizeof(student), 1, readPtr);

            // γράψε μία εγγραφή σε αρχείο κειμένου
            if (result == 1)
            {
                fprintf(writePtr, "%0*d | %12s\n", FIELD_WIDTH, freshman.id, freshman.name);
                i++; // αύξησε μετρητή
            }

            if (i < entries && result == 1)
            {
                fprintf(writePtr, "%s\n", "-----------------");
            }
            else if (i == entries && result == 1)
            {
                fprintf(writePtr, "%s\n", "~~~~~~~~~~~~~~~~~");
            }
        }

        // η fclose κλείνει το αρχείο students.txt
        fclose(writePtr);
        puts(">>> Text file \"students.txt\" successfully created.\n");
    }

    // η fclose κλείνει το αρχείο γραμμής εντολών
    fclose(readPtr);
}

// χειρισμός της εισόδου χρήστη για επιλογή μενού
int enterChoice(char *filename, list l)
{
    int i = 0;      // δείκτης πίνακα number
    char number[3]; // είσοδος χρήστη

    while (1) // ο βρόχος τερματίζει μόνο με έγκυρη είσοδο χρήστη
    {
        fgets(number, 3, stdin); // αποθήκευσε την επιλογή του χρήστη

        while (1) // ατέρμων βρόχος
        {
            if (feof(stdin)) // έαν ο χρήστης εισήγαγε EOF
            {
                printf("%s\n\n", "EOF");         // εμφάνισε EOF
                save(filename, l);               // αποθήκευσε την λίστα
                puts(">>> Program terminated."); // εμφάνισε μήνυμα
                exit(EXIT_SUCCESS);              // τερμάτισε το πρόγραμμα
            }
            else if (number[i] == '\n') // εάν ο χρήστης πάτησε Enter
            {
                if (i == 0) // αν το Enter είναι το πρώτο πλήκτρο που πατήθηκε
                {
                    printf("%s", "$ ");      // προτροπή είσοδου
                    fgets(number, 3, stdin); // ο χρήστης ξαναεπιλέγει
                }
                else // διαφορετικά
                {
                    number[i] = '\0'; // αντικατάστησε το newline με null
                    break;            // έξοδος από τον εσωτερικό βρόχο while
                }
            }
            else if (!isdigit(number[i])) // εάν ο χαρακτήρας που εισήχθη δεν είναι ψηφίο
            {
                // αν ο χρήστης έγραψε πέρα από το μέγεθος του πίνακα
                if (strlen(number) == 2 && number[1] != '\n')
                {
                    // καθάρισε το buffer από όλους τους χαρακτήρες
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF)
                        ;
                }

                printf("%s", "\nWrong input.\n$ "); // εμφάνισε μήνυμα λανθασμένης είσοδου
                fgets(number, 3, stdin);            // ο χρήστης ξαναεπιλέγει
                i = 0;                              // μηδένισε τον δείκτη πίνακα
            }
            else if (isdigit(number[i])) // εάν ο χαρακτήρας που εισήχθη είναι ψηφίο
                i++;                     // αύξησε τον δείκτη πίνακα
        }

        if ((i = strtol(number, NULL, 0)) < 1 || i > 7) // εάν η επιλογή χρήστη δεν είναι έγκυρη
        {
            printf("%s", "\nWrong input.\n$ "); // εμφάνισε μήνυμα λανθασμένης είσοδου
            i = 0;                              // μηδένισε τον δείκτη πίνακα
        }
        else
            break; // έξοδος από τον εξωτερικό βρόχο while
    }

    return strtol(number, NULL, 0); // επέστρεψε την είσοδο του χρήστη
}

// χειρισμός της εισόδου χρήστη για όνομα φοιτητή
int enterName(char name[], char *filename, list l)
{
    int i = 0;                     // δείκτης πίνακα name
    printf("%s", "Enter name: ");  // προτροπή εισόδου
    fgets(name, MAXSTRING, stdin); // αποθήκευσε το όνομα φοιτητή

    while (1) // ο βρόχος τερματίζει μόνο με έγκυρη είσοδο χρήστη
    {
        if (feof(stdin)) // έαν ο χρήστης εισήγαγε EOF
        {
            if (filename == NULL) // αν η enterName καλείται από την updateStudent
            {
                printf("%s\n\n", "EOF"); // εμφάνισε EOF
                return 0;                // επέστρεψε 0 για να υποδείξεις ότι εισήχθει EOF
            }
            else if (filename != NULL) // αν η enterName καλείται απ' οποιαδήποτε άλλη συνάρτηση
            {
                printf("%s\n\n", "EOF");         // εμφάνισε EOF
                save(filename, l);               // αποθήκευσε την λίστα
                puts(">>> Program terminated."); // εμφάνισε μήνυμα
                exit(EXIT_SUCCESS);              // τερμάτισε το πρόγραμμα
            }
        }
        else if (name[i] == '\n') // εάν ο χρήστης πάτησε Enter
        {
            if (i == 0) // αν το Enter είναι το πρώτο πλήκτρο που πατήθηκε
            {
                printf("%s", "Enter name: ");  // προτροπή εισόδου
                fgets(name, MAXSTRING, stdin); // ο χρήστης ξαναεπιλέγει όνομα
            }
            else // διαφορετικά
            {
                name[i] = '\0'; // αντικατάστησε το newline με null
                break;          // έξοδος από τον βρόχο while
            }
        }
        else if (!isalpha(name[i])) // εάν ο χαρακτήρας που εισήχθη δεν είναι γράμμα
        {
            // αν ο χρήστης έγραψε πέρα από το μέγεθος του πίνακα
            if (strlen(name) == MAXSTRING - 1 && name[MAXSTRING - 2] != '\n')
            {
                // καθάρισε το buffer από όλους τους χαρακτήρες
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
                printRules(3); // υπενθύμιση για τα όρια του ονόματος
            }
            else
                printRules(2); // υπενθύμιση για τους χαρακτήρες που επιτρέπονται

            printf("%s", "Enter name: ");  // προτροπή εισόδου
            fgets(name, MAXSTRING, stdin); // ο χρήστης ξαναεπιλέγει όνομα
            i = 0;                         // μηδένισε τον δείκτη πίνακα
        }
        else if (isalpha(name[i])) // εάν ο χαρακτήρας που εισήχθη είναι γράμμα
            i++;                   // αύξησε τον δείκτη πίνακα
    }
    return 1; // επέστρεψε 1 αν η συνάρτηση εκτελέστηκε με επιτυχία
}

// χειρισμός της εισόδου χρήστη για ID
int enterID(char *filename, list l)
{
    int i = 0;                             // δείκτης πίνακα number
    char number[FIELD_WIDTH + 2];          // είσοδος χρήστη
    printf("\n%s", "Enter ID: ");          // προτροπή εισόδου
    fgets(number, FIELD_WIDTH + 2, stdin); // αποθήκευσε το ID που εισάγει ο χρήστης

    while (1) // ο βρόχος τερματίζει μόνο με έγκυρη είσοδο χρήστη
    {
        if (feof(stdin)) // έαν ο χρήστης εισήγαγε EOF
        {
            if (filename == NULL) // αν η enterID καλείται από την updateStudent
            {
                printf("%s\n\n", "EOF"); // εμφάνισε EOF
                return -1;               // επέστρεψε -1 για να υποδείξεις ότι εισήχθει EOF
            }
            else if (filename != NULL) // αν η enterID καλείται απ' οποιαδήποτε άλλη συνάρτηση
            {
                printf("%s\n\n", "EOF");         // εμφάνισε EOF
                save(filename, l);               // αποθήκευσε την λίστα
                puts(">>> Program terminated."); // εμφάνισε μήνυμα
                exit(EXIT_SUCCESS);              // τερμάτισε το πρόγραμμα
            }
        }
        else if (number[i] == '\n') // εάν ο χρήστης πάτησε Enter
        {
            if (i == 0) // αν το Enter είναι το πρώτο πλήκτρο που πατήθηκε
            {
                printf("%s", "Enter ID: ");            // προτροπή εισόδου
                fgets(number, FIELD_WIDTH + 2, stdin); // ο χρήστης ξαναεπιλέγει ID
            }
            else // διαφορετικά
            {
                number[i] = '\0'; // αντικατάστησε το newline με null
                break;            // έξοδος από τον βρόχο while
            }
        }
        else if (!isdigit(number[i])) // εάν ο χαρακτήρας που εισήχθη δεν είναι ψηφίο
        {
            // αν ο χρήστης έγραψε πέρα από το μέγεθος του πίνακα
            if (strlen(number) == FIELD_WIDTH + 1 && number[FIELD_WIDTH] != '\n')
            {
                // καθάρισε το buffer από όλους τους χαρακτήρες
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
            }

            printRules(1);                         // υπενθύμιση για τα όρια του ID
            printf("%s", "Enter ID: ");            // προτροπή εισόδου
            fgets(number, FIELD_WIDTH + 2, stdin); // ο χρήστης ξαναεπιλέγει ID
            i = 0;                                 // μηδένισε τον δείκτη πίνακα
        }
        else if (isdigit(number[i])) // εάν ο χαρακτήρας που εισήχθη είναι ψηφίο
            i++;                     // αύξησε τον δείκτη πίνακα
    }
    return strtol(number, NULL, 0); // επέστρεψε το ID που εισήγαγε ο χρήστης
}

// εκτύπωσε έναν ή όλους τους κανόνες
void printRules(int rule)
{
    switch (rule)
    {
    case 0: // συνοπτική λίστα όλων των κανόνων
        printf("\n%s%s%d%s%d%s%s%d%s", "RULES\n~~~~~\n", "# ID must be an integer between ",
               MIN_ID, " and ", MAX_ID, ".\n", "# Student name must only contain letters.\n"
                                               "# Student name must not exceed ",
               MAXSTRING - 2, " letters.\n");
        break;

    case 1: // μην ξεπεράσεις τα επιτρεπτά όρια για το ID
        printf("\n%s%d%s%d.\n", "# ID must be an integer between ", MIN_ID, " and ", MAX_ID);
        break;

    case 2: // επιτρέπονται μόνο κεφαλαία ή πεζά γράμματα σε ένα όνομα
        puts("\n# Student name must only contain letters.");
        break;

    case 3: // μην ξεπεράσεις το επιτρεπτό όριο χαρακτήρων σε ένα όνομα
        printf("\n%s%d letters.\n", "# Student name must not exceed ", MAXSTRING - 2);
        break;
    }
}

void printMenu(void) // μενού επιλογών
{
    // εμφάνισε τις διαθέσιμες επιλογές
    printf("%44s\n%44s\n%44s\n%s\n%38s%28s\n%s%44s\n%s%34s\n\n%s\n$ ",
           "~~~~~~~~~~~~~~~~~~", "Enter your choice:", "~~~~~~~~~~~~~~~~~~",
           "1 - store a formatted text file of students | 4 - add a new student", "called \"students.txt\" for printing",
           "| 5 - delete a student", "2 - print a single student", "| 6 - search for a student",
           "3 - print a list of all students", "| 7 - update a student", "Enter EOF to end program.");
}

void print(student st) // εκτύπωσε τα στοιχεία ενός φοιτητή
{
    printf("Name: %s\nID  : %0*d\n\n", st.name, FIELD_WIDTH, st.id);
    return;
}

void printstudents(list l) // εκτύπωσε τα στοιχεία όλων των φοιτητών
{
    // εάν η λίστα είναι κενή
    if (l == NULL || l->head == NULL)
    {
        printf("\n%s", "NOTICE\n~~~~~~\n"); // επικεφαλίδα
        puts(">>> List is empty!\n");       // εμφάνισε μήνυμα
    }
    else // διαφορετικά
    {
        // δείκτης στη κεφαλή της λίστας
        node currentNode = l->head;

        // επικεφαλίδα
        printf("\n%s\n%s", "ID | Student Name", "~~~~~~~~~~~~~~~~~");

        // όσο δεν βρίσκεις το τέλος της λίστας
        while (currentNode != NULL)
        {
            // κάθε φοιτητής εκτυπώνεται σε ξεχωριστή γραμμή
            printf("\n%0*d | %12s\n-----------------", FIELD_WIDTH, currentNode->data.id, currentNode->data.name);
            currentNode = currentNode->next; // προχώρα σε επόμενο κόμβο
        }
        printf("\r%s\n\n", "~~~~~~~~~~~~~~~~~");
    }
}

list load(char *filename) // ανάκτηση λίστας φοιτητών από αρχείο
{
    list info;     // δείχνει στην αρχή και στο τέλος της ανακτόμενης λίστας
    int result;    // χρησιμοποιείται για να ελέγξει αν η fread μπορεί να διαβάσει byte
    FILE *filePtr; // δείκτης αρχείου γραμμής εντολών
    int i = 0;     // μετρητής

    // δημιούργησε έναν student με προεπιλεγμένες πληροφορίες
    student freshman = {"", 0};

    node newPtr;      // δείκτης σε νέο κόμβο
    node previousPtr; // δείκτης σε προηγούμενο κόμβο
    node currentPtr;  // δείκτης στον τρέχοντα κόμβο

    // η fopen ανοίγει το αρχείο γραμμής εντολών για ανάγνωση
    if ((filePtr = fopen(filename, "rb")) == NULL)
    {
        // εμφάνισε μήνυμα αν το αρχείο δεν υπάρχει
        printf(">>> File \"%s\" does not exist.\n", filename);

        // εάν το αρχείο δεν μπορεί να δημιουργηθεί
        if ((filePtr = fopen(filename, "wb")) == NULL)
        {
            printf(">>> Could not create file \"%s\".\n\n", filename);
            exit(EXIT_FAILURE); // τερμάτισε το πρόγραμμα
        }
        else // διαφορετικά
        {
            printf(">>> File \"%s\" successfully created.\n\n", filename);
            fclose(filePtr); // η fclose κλείνει το αρχείο
            return NULL;     // επέστρεψε άδεια λίστα
        }
    }
    else
    {
        // απόπειρα ανάγνωσης του πρώτου φοιτητή ώστε να λειτουργήσει σωστά η feof
        result = fread(&freshman, sizeof(student), 1, filePtr);

        // εάν η λίστα είναι άδεια
        if (feof(filePtr))
        {
            puts(">>> List is empty!\n"); // εμφάνισε μήνυμα
            fclose(filePtr);              // η fclose κλείνει το αρχείο
            return NULL;                  // επέστρεψε άδεια λίστα
        }
        else
        {
            // δημιούργησε την λίστα φοιτητών
            info = malloc(sizeof(struct listR));

            // εάν δεν κατανεμήθηκε χώρος
            if (info == NULL)
            {
                printf(">>> File \"%s\" could not load. No memory available.\n", filename);
                exit(EXIT_FAILURE); // ανεπιτυχής τερματισμός προγράμματος
            }
            else // διαφορετικά
            {
                info->head = NULL; // αρχικοποίησε την κεφαλή με NULL
                info->tail = NULL; // αρχικοποίησε την ουρά με NULL
            }

            rewind(filePtr);
            result = fread(&freshman, sizeof(student), 1, filePtr); // διάβασε τον πρώτο φοιτητή
            newPtr = malloc(sizeof(struct nodeR));                  // δημιούργησε κόμβο

            if (result == 1 && newPtr != NULL) // φόρτωσε τον πρώτο κόμβο
            {
                newPtr->data = freshman; // τοποθέτησε τιμή στον πρώτο κόμβο
                newPtr->previous = NULL; // ο πρώτος κόμβος δεν συνδέεται με προηγούμενο κόμβο
                newPtr->next = NULL;     // ο πρώτος κόμβος δεν συνδέεται με επόμενο κόμβο
                currentPtr = newPtr;     // ο τρέχων κόμβος γίνεται ο πρώτος κόμβος
                info->head = newPtr;     // ο πρώτος κόμβος είναι η κεφαλή
            }
            else if (result < 1) // εάν δεν διαβάστηκε ο πρώτος φοιτήτης
            {
                printf(">>> File \"%s\" could not load. Program failed.\n", filename);
                exit(EXIT_FAILURE); // ανεπιτυχής τερματισμός προγράμματος
            }
            else if (newPtr == NULL) // εάν δεν κανανεμήθηκε χώρος
            {
                printf(">>> File \"%s\" could not load. No memory available.\n", filename);
                exit(EXIT_FAILURE); // ανεπιτυχής τερματισμός προγράμματος
            }

            // διάβασε όλες τις εγγραφές από το αρχείο μέχρι το EOF
            while (!feof(filePtr))
            {
                result = fread(&freshman, sizeof(student), 1, filePtr); // διάβασε έναν-έναν τους φοιτητές από το αρχείο
                newPtr = malloc(sizeof(struct nodeR));                  // δημιούργησε κόμβο

                if (result == 1 && newPtr != NULL) // φόρτωσε τους υπόλοιπους κόμβους
                {
                    newPtr->data = freshman;       // τοποθέτησε τιμή στον νέο κόμβο
                    newPtr->previous = currentPtr; // o νέος κόμβος συνδέεται με τον προηγούμενο κόμβο
                    newPtr->next = NULL;           // ο νέος κόμβος δεν συνδέεται με επόμενο κόμβο
                    currentPtr->next = newPtr;     // ο τρέχων κόμβος συνδέεται με τον νέο κόμβο
                    currentPtr = newPtr;           // ο τρέχων κόμβος γίνεται ο νεός κόμβος
                }
                else if (result < 1 && feof(filePtr) == 0) // εάν δεν διαβάστηκε φοιτητής ενώ υπάρχουν ακόμα εγγραφές
                {
                    printf(">>> File \"%s\" could not load. Program failed.\n", filename);
                    exit(EXIT_FAILURE); // ανεπιτυχής τερματισμός προγράμματος
                }
                else if (newPtr == NULL) // εάν δεν κανανεμήθηκε χώρος
                {
                    printf(">>> File \"%s\" could not load. No memory available.\n", filename);
                    exit(EXIT_FAILURE); // ανεπιτυχής τερματισμός προγράμματος
                }
            }

            info->tail = currentPtr; // ο τελευταίος κόμβος είναι η ουρά

            printf(">>> File \"%s\" successfully loaded.\n", filename); // εμφάνισε μήνυμα
            printstudents(info);                                        // πρόβαλλε τη λίστα
            fclose(filePtr);                                            // η fclose κλείνει το αρχείο

            return info; // επέστρεψε την λίστα φοιτητών
        }
    }
}

void save(char *filename, list l) // αποθήκευση λίστας φοιτητών σε αρχείο
{
    FILE *filePtr; // δείκτης αρχείου γραμμής εντολών

    // η λίστα είναι άδεια
    if (l == NULL)
    {
        // εάν το αρχείο δεν ανοίγει για εγγραφή
        if ((filePtr = fopen(filename, "wb")) == NULL)
        {
            printf(">>> Could not save file \"%s\".\n", filename);
            exit(EXIT_FAILURE); // τερμάτισε το πρόγραμμα
        }
        else // διαφορετικά
        {
            printf(">>> File \"%s\" saved successfully.\n", filename);
            fclose(filePtr); // η fclose κλείνει το αρχείο
        }
        return;
    }

    node currentPtr = l->head; // ο τρέχων κόμβος δείχνει στον πρώτο κόμβο

    // εάν το αρχείο δεν ανοίγει για εγγραφή
    if ((filePtr = fopen(filename, "wb")) == NULL)
    {
        printf(">>> Could not save file \"%s\".\n", filename);
        exit(EXIT_FAILURE); // τερμάτισε το πρόγραμμα
    }

    // όσο δεν βρίσκεις το τέλος της λίστας
    while (currentPtr != NULL)
    {
        fwrite(&currentPtr->data, sizeof(student), 1, filePtr); // αποθήκευσε έναν-έναν τους φοιτητές στο αρχείο
        currentPtr = currentPtr->next;                          // ο επόμενος κόμβος γίνεται ο νέος τρέχοντας κόμβος
    }

    printf(">>> File \"%s\" saved successfully.\n", filename); // επιτυχής αποθήκευση

    // η fclose κλείνει το αρχείο γραμμής εντολών
    fclose(filePtr);
}

// προσθήκη νέου student
int addStudent(student st, list l)
{
    node newPtr;      // δείκτης σε νέο κόμβο
    node previousPtr; // δείκτης σε προηγούμενο κόμβο
    node currentPtr;  // δείκτης στον τρέχοντα κόμβο

    newPtr = malloc(sizeof(struct nodeR)); // δημιούργησε κόμβο

    if (newPtr != NULL) // υπάρχει διαθέσιμος χώρος
    {
        newPtr->data = st;       // τοποθέτησε τιμή στον κόμβο
        newPtr->next = NULL;     // ο κόμβος δεν συνδέεται με επόμενο κόμβο
        newPtr->previous = NULL; // ο κόμβος δεν συνδέεται με προηγούμενο κόμβο
        previousPtr = NULL;      // αρχικοποίησε τον previousPtr με NULL
        currentPtr = l->head;    // ο τρέχων κόμβος δείχνει στον πρώτο κόμβο της λίστας

        // επανέλαβε για να βρεις τη σωστή θέση στη λίστα
        while (currentPtr != NULL && st.id > currentPtr->data.id)
        {
            previousPtr = currentPtr;      // ο προηγούμενος κόμβος δείχνει στον τρέχοντα κόμβο
            currentPtr = currentPtr->next; // ο τρέχοντας κόμβος δείχνει στον επόμενο κόμβο
        }

        // εάν το ID χρησιμοποιείται ήδη στη λίστα
        if (currentPtr != NULL && st.id == currentPtr->data.id)
        {
            printf(">>> Student with ID: %0*d already exists.\n", FIELD_WIDTH, st.id);
            printstudents(l); // εμφάνισε όλους τους φοιτητές και τα ID τους
            return -1;        // ανεπιτυχής προσθήκη νέου φοιτητή, μη έγκυρο ID
        }
        else
        {
            if (currentPtr == NULL && previousPtr == NULL) // εισήγαγε τον πρώτο κόμβο της λίστας
            {
                l->head = newPtr;        // ο νέος κόμβος συνδέεται με την κεφαλή
                l->tail = newPtr;        // ο νέος κόμβος συνδέεται με την ουρά
                newPtr->previous = NULL; // ο νέος κόμβος δεν συνδέεται με προηγούμενο κόμβο
                newPtr->next = NULL;     // ο νέος κόμβος δεν συνδέεται με επόμενο κόμβο
            }
            else if (currentPtr != NULL && previousPtr == NULL) // εισήγαγε νέο κόμβο στην αρχή της λίστας
            {
                newPtr->previous = NULL;    // ο νέος κόμβος δεν συνδέεται με προηγούμενο κόμβο
                newPtr->next = l->head;     // ο νέος κόμβος συνδέεται με την κεφαλή
                l->head->previous = newPtr; // η κεφαλή συνδέεται με τον νέο κόμβο
                l->head = newPtr;           // η κεφαλή αλλάζει και είναι πλέον ο νέος κόμβος που εισήχθει
            }
            else if (currentPtr == NULL && previousPtr != NULL) // εισήγαγε νέο κόμβο στο τέλος της λίστας
            {
                previousPtr->next = newPtr;     // ο προηγούμενος κόμβος συνδέεται με τον νέο κόμβο
                newPtr->previous = previousPtr; // ο νέος κόμβος συνδέεται με τον προηγούμενο κόμβο
                newPtr->next = NULL;            // ο νέος κόμβος δεν συνδέεται με επόμενο κόμβο
                l->tail = newPtr;               // η ουρά αλλάζει και είναι πλέον ο νέος κόμβος που εισήχθει
            }
            else if (currentPtr != NULL && previousPtr != NULL) // εισήγαγε νέο κόμβο ανάμεσα σε previousPtr και currentPtr
            {
                previousPtr->next = newPtr;     // ο προηγούμενος κόμβος συνδέεται με τον νέο κόμβο
                newPtr->previous = previousPtr; // ο νέος κόμβος συνδέεται με τον προηγούμενο κόμβο
                currentPtr->previous = newPtr;  // ο τρέχοντας κόμβος συνδέεται με τον νέο κόμβο
                newPtr->next = currentPtr;      // ο νέος κόμβος συνδέεται με τον τρέχοντα κόμβο
            }
            return 1; // επιτυχής προσθήκη νέου φοιτητή
        }
    }
    else
        return 0; // ανεπιτυχής προσθήκη νέου φοιτητή
}

// αναζήτηση του student από τη λίστα με βάση το id
student *findStudent(int id, list l)
{
    node previousPtr; // δείκτης σε προηγούμενο κόμβο
    node currentPtr;  // δείκτης στον τρέχοντα κόμβο

    // εάν η λίστα είναι άδεια
    if (l == NULL)
        return NULL; // επέστρεψε NULL

    previousPtr = NULL;   // αρχικοποίησε τον previousPtr με NULL
    currentPtr = l->head; // ο τρέχων κόμβος δείχνει στον πρώτο κόμβο της λίστας

    // επανέλαβε μέχρι να βρείς το ID του φοιτητή που αναζητείται
    while (currentPtr != NULL && id != currentPtr->data.id)
    {
        previousPtr = currentPtr;      // ο προηγούμενος κόμβος δείχνει στον τρέχοντα κόμβο
        currentPtr = currentPtr->next; // ο τρέχοντας κόμβος δείχνει στον επόμενο κόμβο
    }

    if (currentPtr == NULL)
        return NULL; // εάν ο φοιτητής δεν βρέθηκε επέστρεψε NULL
    else
        return &currentPtr->data; // διαφορετικά επέστρεψε την θέση του φοιτητή
}

// διαγραφή με βάση το st.id
int deleteStudent(student st, list l)
{
    node previousPtr; // δείκτης σε προηγούμενο κόμβο
    node currentPtr;  // δείκτης στον τρέχοντα κόμβο
    node tempPtr;     // προσωρινός δείκτης κόμβου

    if (l->head == l->tail) // διέγραψε τον μόνο κόμβο της λίστας
    {
        free(l->head); // απελευθέρωσε τον μόνο κόμβο της λίστας
        return -1;     // επέστρεψε -1 αν η διαγραφή του μοναδικού κόμβου ήταν επιτυχής
    }
    else if (st.id == l->head->data.id) // διέγραψε πρώτο κόμβο
    {
        tempPtr = l->head;        // παράμεινε στον κόμβο που αφαιρείται
        l->head = l->head->next;  // κατάργησε τα νήματα του κόμβου
        l->head->previous = NULL; // η νέα κεφαλή δεν συνδέεται με προηγούμενο κόμβο
        free(tempPtr);            // απελευθέρωσε τον χωρίς νήμα κόμβο
        return 1;                 // επέστρεψε 1 αν η διαγραφή ήταν επιτυχής
    }
    else if (st.id == l->tail->data.id) // διέγραψε τελευταίο κόμβο
    {
        tempPtr = l->tail;           // παράμεινε στον κόμβο που αφαιρείται
        l->tail = l->tail->previous; // κατάργησε τα νήματα του κόμβου
        l->tail->next = NULL;        // η νέα ουρά δεν συνδέεται με επόμενο κόμβο
        free(tempPtr);               // απελευθέρωσε τον χωρίς νήμα κόμβο
        return 1;                    // επέστρεψε 1 αν η διαγραφή ήταν επιτυχής
    }
    else // διέγραψε κόμβο ανάμεσα σε previousPtr και currentPtr
    {
        previousPtr = NULL;   // αρχικοποίησε τον previousPtr με NULL
        currentPtr = l->head; // ο τρέχων κόμβος δείχνει στον πρώτο κόμβο της λίστας

        // επανέλαβε ώστε να βρεις τη σωστή θέση στη λίστα
        while (currentPtr != NULL && currentPtr->data.id != st.id)
        {
            previousPtr = currentPtr;      // ο προηγούμενος κόμβος δείχνει στον τρέχοντα κόμβο
            currentPtr = currentPtr->next; // ο τρέχοντας κόμβος δείχνει στον επόμενο κόμβο
        }

        // κατάργησε τα νήματα του κόμβου
        previousPtr->next = currentPtr->next;
        currentPtr->next->previous = previousPtr;

        free(currentPtr); // απελευθέρωσε τον χωρίς νήμα κόμβο
        return 1;         // επέστρεψε 1 αν η διαγραφή ήταν επιτυχής
    }
    return 0; // επέστρεψε 0 αν η διαγραφή ήταν ανεπιτυχής
}

// ανανέωση με βάση το st.id
int updateStudent(student st, list l)
{
    student *studentPtr = NULL; // δείκτης σε student
    node previousPtr;           // δείκτης σε προηγούμενο κόμβο
    node currentPtr;            // δείκτης στον τρέχοντα κόμβο

    previousPtr = NULL;   // αρχικοποίησε τον previousPtr με NULL
    currentPtr = l->head; // ο τρέχων κόμβος δείχνει στον πρώτο κόμβο της λίστας

    // επανέλαβε μέχρι να βρείς το ID του φοιτητή που αναζητείται
    while (currentPtr != NULL && st.id != currentPtr->data.id)
    {
        previousPtr = currentPtr;      // ο προηγούμενος κόμβος δείχνει στον τρέχοντα κόμβο
        currentPtr = currentPtr->next; // ο τρέχοντας κόμβος δείχνει στον επόμενο κόμβο
    }

    studentPtr = &currentPtr->data; // εκχώρησε την θέση του φοιτητή στον δείκτη studentPtr

    printf("\n%s", "________\nNew Data\n________");
    int searchID = enterID(NULL, l);

    // εάν ο χρήστης πληκτρολόγησε EOF στην enterID ή στην enterName
    if (searchID == -1 || enterName(studentPtr->name, NULL, l) == 0)
    {
        strcpy(studentPtr->name, st.name); // επανέφερε το όνομα του φοιτητή στην αρχική του τιμή
        return -1;                         // επέστρεψε -1 ώστε το πρόγραμμα να τερματίσει στην main
    }

    node searchPtr = l->head; // προσωρινός δείκτης κόμβου που δείχνει στην κεφαλή της λίστας

    // οσό δεν βρίσκεις το νέο ID που εισήγαγε ο χρήστης
    while (searchPtr != NULL && searchID != searchPtr->data.id)
    {
        searchPtr = searchPtr->next; // προχώρα σε επόμενο κόμβο
    }

    if (searchPtr != NULL && st.id == searchID) // ο χρήστης άλλαξε το όνομα του φοιτητή
    {
        printf("\n%s", "NOTICE\n~~~~~~\n");
        printf(">>> Student successfully renamed to \"%s\".\n\n", studentPtr->name);
        return 0; // επέστρεψε 0 αν έγινε μετονομασία
    }
    else if (searchPtr != NULL && st.id != searchID) // ο χρήστης έβαλε μη διαθέσιμο ID
    {
        printf("\n%s", "NOTICE\n~~~~~~\n");
        printf(">>> Student with ID: %0*d already exists.\n", FIELD_WIDTH, searchID);
        printf("%s\n", ">>> Try using another ID.\n");
        strcpy(studentPtr->name, st.name); // επανέφερε το όνομα του φοιτητή στην αρχική του τιμή
        return 0;                          // επέστρεψε 0 αν η αλλαγή στοιχείων ήταν ανεπιτυχής
    }
    else if (searchPtr == NULL) // το ID που εισήγαγε ο χρήστης δεν βρέθηκε στην λίστα
    {
        studentPtr->id = searchID; // ενημέρωσε τον φοιτητή με το νέο ID
        searchPtr = currentPtr;    // ο searchPtr δείνχει στον ανανεωμένο κόμβο
    }

    // σύνδεσε τους κόμβους που υπάρχουν πριν και μετά τον ανανεωμένο κόμβο
    if (searchPtr->previous == NULL && searchPtr->next == NULL) // ο searchPtr δείχνει στον μόνο κόμβο της λίστας
    {
        l->head = searchPtr;
        l->tail = searchPtr;
        return 1; // επιτυχής αλλαγή στοιχείων, δεν απαιτείται περαιτέρω επεξεργασία
    }
    else if (searchPtr->previous == NULL && searchPtr->next != NULL) // ο searchPtr δείχνει στον πρώτο κόμβο της λίστας
    {
        searchPtr->next->previous = NULL;
        l->head = searchPtr->next;
    }
    else if (searchPtr->previous != NULL && searchPtr->next == NULL) // ο searchPtr δείχνει στον τελευταίο κόμβο της λίστας
    {
        searchPtr->previous->next = NULL;
        l->tail = searchPtr->previous;
    }
    else if (searchPtr->previous != NULL && searchPtr->next != NULL) // ο searchPtr δείχνει σ' έναν ενδιάμεσο κόμβο
    {
        searchPtr->previous->next = searchPtr->next;
        searchPtr->next->previous = searchPtr->previous;
    }

    previousPtr = NULL;   // αρχικοποίησε τον previousPtr με NULL
    currentPtr = l->head; // αρχικοποίησε τον currentPtr με την κεφαλή

    // επανέλαβε για να βρεις τη σωστή θέση στη λίστα
    while (currentPtr != NULL && studentPtr->id > currentPtr->data.id)
    {
        previousPtr = currentPtr;      // ο προηγούμενος κόμβος δείχνει στον τρέχοντα κόμβο
        currentPtr = currentPtr->next; // ο τρέχοντας κόμβος δείχνει στον επόμενο κόμβο
    }

    if (currentPtr != NULL && previousPtr == NULL) // εισήγαγε τον κόμβο με τον ανανεωμένο φοιτητή στην αρχή της λίστας
    {
        searchPtr->previous = NULL;    // ο κόμβος δεν συνδέεται με προηγούμενο κόμβο
        searchPtr->next = l->head;     // ο κόμβος συνδέεται με την κεφαλή
        l->head->previous = searchPtr; // η κεφαλή συνδέεται με τον ανανεωμένο κόμβο
        l->head = searchPtr;           // η κεφαλή αλλάζει και είναι πλέον ο κόμβος με τον ανανεωμένο φοιτητή
    }
    else if (currentPtr == NULL && previousPtr != NULL) // εισήγαγε τον κόμβο με τον ανανεωμένο φοιτητή στο τέλος της λίστας
    {
        searchPtr->previous = l->tail; // ο κόμβος συνδέεται με την ουρά
        searchPtr->next = NULL;        // ο κόμβος δεν συνδέεται με επόμενο κόμβο
        l->tail->next = searchPtr;     // η ουρά συνδέεται με τον ανανεωμένο κόμβο
        l->tail = searchPtr;           // η ουρά αλλάζει και είναι πλέον ο κόμβος με τον ανανεωμένο φοιτητή
    }
    else if (currentPtr != NULL && previousPtr != NULL) // εισήγαγε τον κόμβο με τον ανανεωμένο φοιτητή ανάμεσα σε previousPtr και currentPtr
    {
        previousPtr->next = searchPtr;     // ο προηγούμενος κόμβος συνδέεται με τον κόμβο του ανανεωμένου φοιτητή
        searchPtr->previous = previousPtr; // ο κόμβος του ανανεωμένου φοιτητή συνδέεται με τον προηγούμενο κόμβο
        currentPtr->previous = searchPtr;  // ο τρέχοντας κόμβος συνδέεται με τον κόμβο του ανανεωμένου φοιτητή
        searchPtr->next = currentPtr;      // ο κόμβος του ανανεωμένου φοιτητή συνδέεται με τον τρέχοντα κόμβο
    }

    return 1; // επέστρεψε 1 αν η αλλαγή στοιχείων έγινε με επιτυχία
}

// αποθηκεύει την λίστα και επιστρέφει τον αριθμό των εγγεγραμμένων φοιτητών
int saveList(char *filename, list l)
{
    int entries = 0; // αριθμός εγγεγραμμένων φοιτητών
    FILE *filePtr;   // δείκτης αρχείου γραμμής εντολών

    // η λίστα είναι άδεια
    if (l == NULL)
    {
        // εάν το αρχείο δεν ανοίγει για εγγραφή
        if ((filePtr = fopen(filename, "wb")) == NULL)
        {
            printf(">>> Could not save file \"%s\".\n", filename);
            exit(EXIT_FAILURE); // τερμάτισε το πρόγραμμα
        }
        else // διαφορετικά
        {
            printf(">>> File \"%s\" saved successfully.\n", filename);
            fclose(filePtr); // η fclose κλείνει το αρχείο
        }
        return entries; // επέστρεψε τον αριθμό των εγγεγραμμένων φοιτητών
    }

    node currentPtr = l->head; // ο τρέχων κόμβος δείχνει στον πρώτο κόμβο

    // εάν το αρχείο δεν ανοίγει για εγγραφή
    if ((filePtr = fopen(filename, "wb")) == NULL)
    {
        printf(">>> Could not save file \"%s\".\n", filename);
        exit(EXIT_FAILURE); // τερμάτισε το πρόγραμμα
    }

    // όσο δεν βρίσκεις το τέλος της λίστας
    while (currentPtr != NULL)
    {
        fwrite(&currentPtr->data, sizeof(student), 1, filePtr); // αποθήκευσε έναν-έναν τους φοιτητές στο αρχείο
        currentPtr = currentPtr->next;                          // ο επόμενος κόμβος γίνεται ο νέος τρέχοντας κόμβος
        entries++;                                              // αύξησε τον αριθμό των εγγεγραμμένων φοιτητών
    }

    printf(">>> File \"%s\" saved successfully.\n", filename); // επιτυχής αποθήκευση

    fclose(filePtr); // η fclose κλείνει το αρχείο γραμμής εντολών
    return entries;  // επέστρεψε τον αριθμό των εγγεγραμμένων φοιτητών
}