#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
    1) help to list all commands
    2) print all contacts
    3) insert new contact
    4) delete contact
    5) modify contact information
    6) sort phone book
    7) search name
    8) save to file 
    9) load from file 
*/

// short-hand for frequentely used types
typedef char lastname[64];
typedef char firstname[64];
typedef char number[16];
typedef char Addr[128];
typedef char Email[64];
typedef struct
{
    char day[5];
    char month[5];
    char year[5];
} Birthday;
// Phone book contact
typedef struct contact
{
    firstname fname;
    lastname lname;
    Birthday date;
    number num;
    Addr addr;
    Email email;
} Contact, *PContact;

// prints the available commands
void help();

// prints the all the present phone book's contacts
void print(PContact *, int);

// inserts a contact in the phone book if it's not full
PContact *insert(PContact *, int *, int *);

// increases phone book's size
PContact *increase(PContact *, int *);

// deletes the contact at the position and shifts left by one the other contacts
// and decrements the indicator of fullness
void deleteContact(PContact *, int *, int);

// erases the phone book
void erase(PContact *, int *);

// modifies the contact's information if the contact exists
void modify(PContact *, int, int);

// sorts the phone book by name
void sort(PContact *, int);

// search contact in the phone book if it exists
void search(PContact *, char[], int);

// converts a string from mixed-case to lowercase
void toLower(char[]);

// unites name and surname
void concNameSur(PContact *, char[], int);

// unites surname and name
void concSurName(PContact *, char[], int);

// prints the contact found by search()
void printContact(PContact *, int);

// saves phone book on a file
void save(PContact *, FILE *, int);

// load phone book from file
PContact *load(PContact *, FILE *, int *, int *);

// method for input of strings
void getString(char *, int);

// driver
int main()
{
    PContact *phonebook; // phone book
    FILE *fp;

    int size = 30; // phone book's initial size

    phonebook = (PContact *)calloc(sizeof(PContact), size); // allocates memory for the pointer to the phone book

    char buffer[1024]; // buffer for the selection of commands to execute
    char b[64];        // buffer for the contact search
    int full_ind = 0;  // phone book's fullness indicator
    int n = 0;         // which contact to delete

    // user interface
    printf("Press h for the list of commands\n");
    do
    {
        printf(">> ");
        getString(buffer, 1023);

        switch (buffer[0])
        {
        case 'h':
        case 'H':
        case '?':
            help();
            break;
        case 'p':
        case 'P':
            print(phonebook, full_ind);
            break;
        case 'i':
        case 'I':
            phonebook = insert(phonebook, &full_ind, &size);
            break;
        case 'd':
        case 'D':
            if (full_ind > 0)
            {
                for (int i = 0; i < full_ind; i++)
                {
                    printf("#%d | Name: %s %s\n", i + 1, phonebook[i]->lname, phonebook[i]->fname);
                }
                printf("\nInsert the contact's position: ");
                scanf("%d", &n);
                getchar();
                deleteContact(phonebook, &full_ind, n);
            }
            else
                printf("Could not delete! Phone book's empty!\n");
            break;
        case 'a':
        case 'A':
            erase(phonebook, &full_ind);
            break;
        case 'm':
        case 'M':
            if (full_ind > 0)
            {
                for (int i = 0; i < full_ind; i++)
                {
                    printf("#%d | Name: %s %s\n", i + 1, phonebook[i]->lname, phonebook[i]->fname);
                }
                printf("Insert the contact's position: ");
                scanf("%d", &n);
                getchar();
                modify(phonebook, full_ind, n);
            }
            else
                printf("Could not modify! Phone book's empty!\n");
            break;
        case 'o':
        case 'O':
            sort(phonebook, full_ind);
            break;
        case 'r':
        case 'R':
            if (full_ind > 0)
            {
                sort(phonebook, full_ind);
                printf("Insert contact to search:\n");
                getString(b, 63);
                search(phonebook, b, full_ind);
            }
            else
                printf("Could not search! Phone book's empty!\n");
            break;
        case 's':
        case 'S':
            fp = fopen("phonebook.txt", "w");
            if (fp)
            {
                save(phonebook, fp, full_ind);
            }
            else
                printf("Could not find file!\n");
            fclose(fp);
            break;
        case 'l':
        case 'L':
            fp = fopen("phonebook.txt", "r");
            if (fp)
            {
                phonebook = load(phonebook, fp, &full_ind, &size);
            }
            else
                printf("Could not find file!\n");
            fclose(fp);
            break;
        case 'q':
            printf("---End of phone book program---\n");
            break;
        case '\0':
            break;
        default:
            printf("Error! Invalid character!\n");
            break;
        }
        printf("\n");
    } while (buffer[0] != 'q');

    // frees all allocated memory
    erase(phonebook, &full_ind);
    free(phonebook);

    getchar();
    return 0;
}

void help()
{
    printf("h - help\n");
    printf("p - print all contacts present in the phone book\n");
    printf("i - insert new contact\n");
    printf("d - delete contact\n");
    printf("a - erase phone book\n");
    printf("m - modify contact information\n");
    printf("o - sort phone book\n");
    printf("r - search name\n");
    printf("s - save to file\n");
    printf("l - load from file\n");
    printf("q - quit program\n");
}

void print(PContact *r, int p)
{
    // if the phone book is not empty print all the contacts' information
    if (p > 0)
    {
        for (int i = 0; i < p; i++)
        {
            printf("Contact #%d\n", i + 1);
            printf("Name:       %s %s\n", r[i]->lname, r[i]->fname);
            // Date of birth must be integer, otherwise will display 0-0-0
            // with the help atoi() function
            printf("Birth date: %d-%d-%d\n", atoi(r[i]->date.day), atoi(r[i]->date.month), atoi(r[i]->date.year));
            printf("Address:    %s\n", r[i]->addr);
            // Phone number must be integer, otherwise will display 0
            // with the help atoi() function
            printf("Phone number:  %d\n", atoi(r[i]->num));
            printf("Email:    %s\n\n", r[i]->email);
        }
    }
    else
        printf("Phone book empty!\n");
}

PContact *insert(PContact *r, int *p, int *size)
{
    // if the phone book is full it increases it's size by one
    if (*p == *size)
    {
        r = increase(r, size);
    }

    r[*p] = (PContact)calloc(sizeof(Contact), 1); // allocate the contact memory
    printf("Insert contact's firstname, lastname, birth date, address, phone number, email: \n");

    // insert the contact's information
    printf("Firstname: ");
    getString(r[*p]->fname, sizeof(firstname));

    printf("Lastname: ");
    getString(r[*p]->lname, sizeof(lastname));

    printf("Birth date: ");
    getString(r[*p]->date.day, sizeof(Birthday));
    getString(r[*p]->date.month, sizeof(Birthday));
    getString(r[*p]->date.year, sizeof(Birthday));

    printf("Address: ");
    getString(r[*p]->addr, sizeof(Addr));

    printf("Phone Number: ");
    getString(r[*p]->num, sizeof(number));

    printf("Email: ");
    getString(r[*p]->email, sizeof(Email));

    (*p)++; // increment the indicator of fullness

    return r; // returns a pointer to the phone book
}

PContact *increase(PContact *r, int *size)
{
    (*size)++; // increases the phone book size counter

    PContact *s = (PContact *)calloc(sizeof(PContact), *size); // allocates a new pointer of the new size

    memcpy(s, r, sizeof(PContact) * (*size)); // copies the old contacts in the new pointer

    free(r); // frees the old pointer

    return s; // returns the new pointer to phone book with increased size with the respect to the former
}

void deleteContact(PContact *r, int *p, int n)
{
    // if the position does not exceed the phone book size and the contact exists
    // shift copy the contacts
    if (n > 0 && n <= *p)
    {
        free(r[n - 1]); // deletes the contact

        for (int i = n - 1; i < (*p) - 1; i++)
        {
            // shift the contacts left by one position
            // until the penultimate is reached
            r[i] = r[i + 1];
        }
        (*p)--; // decrement the indicator of fullness
    }
    else
        printf("Could not delete! Contact at position #%d does not exist!\n", n);
}

void erase(PContact *r, int *p)
{
    // erases all the contacts in the phone book
    // and sets the fullness indicator to zero
    for (int i = 0; i < *p; i++)
        free(r[i]);
    (*p) = 0;
}

void modify(PContact *r, int p, int n)
{
    // if the position does not exceed the phone book size and the contact exists
    // modify the contact's information
    if (n > 0 && n <= p)
    {
        printf("Modify contact's info: \n");

        // insert new contact's information
        printf("Firstname: ");
        getString(r[n - 1]->fname, sizeof(lastname));

        printf("Lastname: ");
        getString(r[n - 1]->lname, sizeof(lastname));

        printf("Date of Birth: ");
        getString(r[n - 1]->date.day, sizeof(number));
        getString(r[n - 1]->date.month, sizeof(number));
        getString(r[n - 1]->date.year, sizeof(number));

        printf("Address: ");
        getString(r[n - 1]->addr, sizeof(Addr));

        printf("Phone Number: ");
        getString(r[n - 1]->num, sizeof(number));

        printf("Email: ");
        getString(r[n - 1]->email, sizeof(Addr));

        printf("\nContact modified successfully!\n");
    }
    else
        printf("Could not modify! Contact at position #%d does not exist!\n", n);
}

void sort(PContact *r, int p)
{
    // sorts if the phone book is not empty
    if (p > 0)
    {
        int c = 0;
        int d = 0;
        PContact temp;

        for (int i = 0; i < p - 1; i++)
        {
            for (int j = 0; j < p - i - 1; j++)
            {
                c = strcmp(r[j]->fname, r[j + 1]->fname);
                d = strcmp(r[j]->lname, r[j + 1]->lname);

                // compares the lastnames and firstnames, 1 a is greater than b, 0 equal, -1 a is less than b
                if (d > 0 || (d == 0 && c > 0))
                {
                    temp = r[j];
                    r[j] = r[j + 1];
                    r[j + 1] = temp;
                }
            }
        }
    }
    else
        printf("Could not sort! Empty phone book!\n");
}

void search(PContact *r, char x[], int p)
{
    int h = 0;
    for (int i = 0; i < p; i++)
    {
        char temp[129];
        //char temp2[129];

        char t1[64];
        //char t2[64];

        strcpy(t1, r[i]->lname);
        //strcpy(t2, r[i]->lname);

        toLower(t1);
        //toLower(t2);

        concNameSur(r, temp, i);
        //concSurName(r, temp2, i);
        toLower(x);

        if (strncmp(x, temp, strlen(x)) == 0)
        {
            printContact(r, i);
            h = 1;
        }
        else if (strncmp(x, t1, strlen(x)) == 0)
        {
            printContact(r, i);
            h = 1;
        }
    }

    if (h == 0)
        printf("Could not find contact!\n");
}

void toLower(char x[])
{
    for (int i = 0; x[i]; i++)
    {
        x[i] = tolower(x[i]);
    }
}

void concNameSur(PContact *r, char x[], int i)
{
    int j = 0;
    for (j = 0; r[i]->lname[j] != '\0'; ++j)
    {
        x[j] = r[i]->lname[j];
    }

    x[j] = ' ';

    toLower(x);
}

void printContact(PContact *r, int a)
{
    // if the phone book's not empty prints the desired contact's info
    if (a >= 0)
    {
        printf("Contact N.%d\n", a + 1);
        printf("Name:       %s %s\n", r[a]->lname, r[a]->fname);
        printf("Birth date: %s-%s-%s\n", r[a]->date.day, r[a]->date.month, r[a]->date.year);
        printf("Address:    %s\n", r[a]->addr);
        printf("Telephone:  %s\n", r[a]->num);
        printf("Email:    %s\n\n", r[a]->email);
    }
    else
        return;
}

void save(PContact *r, FILE *f, int p)
{
    // if the phone book is not empty save the contacts on the file
    if (p > 0)
    {
        for (int i = 0; i < p; i++)
        {
            fwrite(r[i], sizeof(Contact), 1, f);
        }

        printf("Phone book successfully saved on file!\n");
    }
    else
        printf("Could not save! Phone book's empty!\n");
}

PContact *load(PContact *r, FILE *f, int *p, int *size)
{
    Contact c;

    while (fread(&c, 1, sizeof(Contact), f))
    {
        // if the the number of contacts in the file exceeds
        // the phone book's size, it increases it
        if (*p >= *size)
            r = increase(r, size);

        r[*p] = (PContact)calloc(sizeof(Contact), 1); // allocates memory for the contact
        *r[*p] = c;                                   // stores the read contact in the phone book
        (*p)++;                                       // increases the fullness indicator
    }

    return r; // returns a pointer to the phone book
}

void getString(char *str, int ncar)
{
    char c;
    int n = 0;
    int cont = 1;

    while (cont)
    {
        c = getchar();

        if (c == EOF || n == (ncar - 2) || c == '\n')
        {
            cont = 0;
        }
        else
        {
            str[n++] = c;
        }
    }

    str[n] = '\0';
}