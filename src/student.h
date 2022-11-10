#define MAXSTRING 14  // μέγιστο μέγεθος ονόματος φοιτητή
#define FIELD_WIDTH 2 // πλάτος πεδίου για printf
#define MAX_ID 99     // μέγιστος αριθμός ID
#define MIN_ID 0      // ελάχιστος αριθμός ID

// ορισμός δομής student
typedef struct
{
    char name[MAXSTRING]; // όνομα φοιτητή
    int id;               // ID φοιτητή
} student;

typedef struct nodeR *node; // συνώνυμο του struct nodeR*
typedef struct listR *list; // συνώνυμο του struct listR*

// ορισμός δομής struct nodeR
struct nodeR
{
    student data;  // τιμή κόμβου
    node next;     // δείκτης στον επόμενο κόμβο
    node previous; // δείκτης στον προηγούμενο κόμβο
};

// ορισμός δομής struct listR
struct listR
{
    node head; // δείκτης στην κεφαλή της λίστας
    node tail; // δείκτης στην ουρά της λίστας
};

// δημιούργησε αρχείο μορφοποιημένου κειμένου για προβολή
void textFile(char *filename, list l);

// χειρισμός της εισόδου χρήστη για επιλογή μενού
int enterChoice(char *filename, list l);

// χειρισμός της εισόδου χρήστη για όνομα φοιτητή
int enterName(char name[], char *filename, list l);

// χειρισμός της εισόδου χρήστη για ID
int enterID(char *filename, list l);

// εκτύπωσε έναν ή όλους τους κανόνες
void printRules(int rule);

// μενού επιλογών
void printMenu(void);

// εκτύπωσε τα στοιχεία ενός φοιτητή
void print(student st);

// εκτύπωσε τα στοιχεία όλων των φοιτητών
void printstudents(list l);

// ανάκτηση λίστας φοιτητών από αρχείο
list load(char *filename);

// αποθήκευση λίστας φοιτητών σε αρχείο
void save(char *filename, list l);

// προσθήκη νέου student
int addStudent(student st, list l);

// αναζήτηση του student από τη λίστα με βάση το id
student *findStudent(int id, list l);

// διαγραφή με βάση το st.id
int deleteStudent(student st, list l);

// ανανέωση με βάση το st.id
int updateStudent(student st, list l);

// αποθηκεύει την λίστα και επιστρέφει
// τον αριθμό των εγγεγραμμένων φοιτητών
int saveList(char *filename, list l);