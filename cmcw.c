#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_NAME 50
#define MAX_PHONE 15
#define MAX_ADDRESS 100
#define MAX_EMAIL 50
#define MAX_DATE 11 // Format: YYYY-MM-DD

// encryption/decryption functions
#define ENCRYPTION_KEY 7

void encrypt(char text[])
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalnum(text[i]))
        {
            if (isupper(text[i]))
            {
                text[i] = ((text[i] - 'A' + ENCRYPTION_KEY) % 26) + 'A';
            }
            else if (islower(text[i]))
            {
                text[i] = ((text[i] - 'a' + ENCRYPTION_KEY) % 26) + 'a';
            }
            else if (isdigit(text[i]))
            {
                text[i] = ((text[i] - '0' + ENCRYPTION_KEY) % 10) + '0';
            }
        }
    }
}

void decrypt(char *text)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalnum(text[i]))
        {
            if (isupper(text[i]))
            {
                text[i] = ((text[i] - 'A' - ENCRYPTION_KEY + 26) % 26) + 'A';
            }
            else if (islower(text[i]))
            {
                text[i] = ((text[i] - 'a' - ENCRYPTION_KEY + 26) % 26) + 'a';
            }
            else if (isdigit(text[i]))
            {
                text[i] = ((text[i] - '0' - ENCRYPTION_KEY + 10) % 10) + '0';
            }
        }
    }
}

typedef struct Contact
{
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char address[MAX_ADDRESS];
    char email[MAX_EMAIL];
    char dateAdded[MAX_DATE];
    struct Contact *next;
} Contact;

// global head pointer
Contact *head = NULL;

// validation functions
int isValidPhone(const char *phone)
{
    for (int i = 0; phone[i] != '\0'; i++)
    {
        if (!isdigit(phone[i]) && phone[i] != '-')
        {
            return 0;
        }
    }
    return 1;
}

int isValidEmail(const char *email)
{
    int at_found = 0;
    for (int i = 0; email[i] != '\0'; i++)
    {
        if (email[i] == '@')
        {
            at_found = 1;
        }
    }
    return at_found;
}

// helper function to get the current date in YYYY-MM-DD format
void getCurrentDate(char *dateBuffer)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(dateBuffer, MAX_DATE, "%d-%m-%Y", t);
}

// function to save all contacts to the file
void saveContactsToFile()
{
    FILE *file = fopen("contact.txt", "w");
    if (file == NULL)
    {
        printf("Failed to save contacts to file.\n");
        return;
    }

    Contact *current = head;
    while (current != NULL)
    {
        char name[MAX_NAME], phone[MAX_PHONE],
            address[MAX_ADDRESS], email[MAX_EMAIL];

        strcpy(name, current->name);
        strcpy(phone, current->phone);
        strcpy(address, current->address);
        strcpy(email, current->email);

        encrypt(name);
        encrypt(phone);
        encrypt(address);
        encrypt(email);

        fprintf(file, "%s,%s,%s,%s,%s\n",
                name, phone, address, email, current->dateAdded);
        current = current->next;
    }
    fclose(file);
}

// function to insert a contact while maintaining sorted order
void addContact(const char *name, const char *phone, const char *address, const char *email)
{
    if (!isValidPhone(phone))
    {
        printf("Invalid phone number. Please try again.\n");
        return;
    }

    if (!isValidEmail(email))
    {
        printf("Invalid email address. Please try again.\n");
        return;
    }

    Contact *newContact = (Contact *)malloc(sizeof(Contact));
    strncpy(newContact->name, name, MAX_NAME);
    strncpy(newContact->phone, phone, MAX_PHONE);
    strncpy(newContact->address, address, MAX_ADDRESS);
    strncpy(newContact->email, email, MAX_EMAIL);
    getCurrentDate(newContact->dateAdded);
    newContact->next = NULL;

    if (head == NULL || strcmp(name, head->name) < 0)
    {
        newContact->next = head;
        head = newContact;
    }
    else
    {
        Contact *current = head;
        while (current->next != NULL && strcmp(current->next->name, name) < 0)
        {
            current = current->next;
        }
        newContact->next = current->next;
        current->next = newContact;
    }

    saveContactsToFile();
    printf("Contact has been added with date: %s.\n", newContact->dateAdded);
}

// function to load contacts from file
void loadContacts()
{
    FILE *file = fopen("contact.txt", "r");
    if (file == NULL)
    {
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char name[MAX_NAME], phone[MAX_PHONE],
            address[MAX_ADDRESS], email[MAX_EMAIL],
            dateAdded[MAX_DATE];

        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%s",
               name, phone, address, email, dateAdded);

        decrypt(name);
        decrypt(phone);
        decrypt(address);
        decrypt(email);

        Contact *newContact = (Contact *)malloc(sizeof(Contact));
        strncpy(newContact->name, name, MAX_NAME);
        strncpy(newContact->phone, phone, MAX_PHONE);
        strncpy(newContact->address, address, MAX_ADDRESS);
        strncpy(newContact->email, email, MAX_EMAIL);
        strncpy(newContact->dateAdded, dateAdded, MAX_DATE);
        newContact->next = NULL;

        if (head == NULL)
        {
            head = newContact;
        }
        else
        {
            Contact *current = head;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = newContact;
        }
    }
    fclose(file);
}

// function to display all contacts
void displayAllContacts()
{
    if (head == NULL)
    {
        printf("No contacts available.\n");
        return;
    }

    Contact *current = head;
    printf("\nContact List:\n");
    while (current != NULL)
    {
        printf("Name: %s, Phone: %s, Address: %s, Email: %s, Date Added: %s\n",
               current->name, current->phone, current->address, current->email, current->dateAdded);
        current = current->next;
    }
}
// search contact
void searchContact(int searchType, const char *query)
{
    Contact *current = head;
    int found = 0;

    while (current != NULL)
    {
        int match = 0;
        switch (searchType)
        {
        case 1:
            match = strstr(current->name, query) != NULL;
            break;
        case 2:
            match = strstr(current->phone, query) != NULL;
            break;
        case 3:
            match = strstr(current->address, query) != NULL;
            break;
        case 4:
            match = strstr(current->email, query) != NULL;
            break;
        }
        if (match)
        {
            printf("Name: %s, Phone: %s, Address: %s, Email: %s, Date Added: %s\n", current->name, current->phone, current->address, current->email, current->dateAdded);
            found = 1;
        }
        current = current->next;
    }
    if (!found)
    {
        printf("No contacts found.\n");
    }
}

// delete contact
void deleteContact(const char *name)
{
    if (head == NULL)
    {
        printf("Contact list is empty.\n");
        return;
    }

    Contact *current = head;
    Contact *previous = NULL;

    while (current != NULL && strcmp(current->name, name) != 0)
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Contact not found.\n");
        return;
    }

    if (previous == NULL)
    {
        head = current->next;
    }
    else
    {
        previous->next = current->next;
    }

    free(current);
    saveContactsToFile();
    printf("Contact has been deleted.\n");
}

// edit contact
void editContact(const char *name)
{
    Contact *current = head;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            char newPhone[MAX_PHONE], newAddress[MAX_ADDRESS], newEmail[MAX_EMAIL];

            printf("Editing contact: %s\n", name);
            printf("New phone (leave empty to keep current): ");
            fgets(newPhone, MAX_PHONE, stdin);
            newPhone[strcspn(newPhone, "\n")] = '\0';
            if (strlen(newPhone) > 0)
            {
                strncpy(current->phone, newPhone, MAX_PHONE);
            }

            printf("New address (leave empty to keep current): ");
            fgets(newAddress, MAX_ADDRESS, stdin);
            newAddress[strcspn(newAddress, "\n")] = '\0';
            if (strlen(newAddress) > 0)
            {
                strncpy(current->address, newAddress, MAX_ADDRESS);
            }

            printf("New email (leave empty to keep current): ");
            fgets(newEmail, MAX_EMAIL, stdin);
            newEmail[strcspn(newEmail, "\n")] = '\0';
            if (strlen(newEmail) > 0)
            {
                strncpy(current->email, newEmail, MAX_EMAIL);
            }

            saveContactsToFile();
            printf("Contact has been updated.\n");
            return;
        }
        current = current->next;
    }

    printf("Contact not found.\n");
}

// sort contacts
void sortContacts()
{
    if (head == NULL)
    {
        printf("No contacts available to sort.\n");
        return;
    }

    // bubble sort for linked list
    int swapped;
    Contact *ptr1;
    Contact *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr)
        {
            if (strcmp(ptr1->name, ptr1->next->name) > 0)
            {
                // Swap contacts
                char tempName[MAX_NAME], tempPhone[MAX_PHONE], tempAddress[MAX_ADDRESS], tempEmail[MAX_EMAIL], tempDate[MAX_DATE];
                strcpy(tempName, ptr1->name);
                strcpy(tempPhone, ptr1->phone);
                strcpy(tempAddress, ptr1->address);
                strcpy(tempEmail, ptr1->email);
                strcpy(tempDate, ptr1->dateAdded);

                strcpy(ptr1->name, ptr1->next->name);
                strcpy(ptr1->phone, ptr1->next->phone);
                strcpy(ptr1->address, ptr1->next->address);
                strcpy(ptr1->email, ptr1->next->email);
                strcpy(ptr1->dateAdded, ptr1->next->dateAdded);

                strcpy(ptr1->next->name, tempName);
                strcpy(ptr1->next->phone, tempPhone);
                strcpy(ptr1->next->address, tempAddress);
                strcpy(ptr1->next->email, tempEmail);
                strcpy(ptr1->next->dateAdded, tempDate);

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    printf("Contacts have been sorted successfully.\n");
}

// advanced search function
void advancedSearch()
{
    if (head == NULL)
    {
        printf("No contacts available to search.\n");
        return;
    }

    int choice;
    printf("\n=====================================\n");
    printf("         Advanced Search Menu        \n");
    printf("=====================================\n");
    printf("1. Search by Partial Match (Name starts with...)\n");
    printf("2. Auto-complete Name\n");
    printf("3. Search by Multiple Fields (Name AND/OR Phone AND/OR Email)\n");
    printf("Choose an option: ");
    scanf("%d", &choice);
    getchar(); // Remove newline character

    if (choice == 1)
    {
        char prefix[MAX_NAME];
        printf("Enter the starting letters of the name: ");
        fgets(prefix, MAX_NAME, stdin);
        prefix[strcspn(prefix, "\n")] = '\0'; // Remove newline

        Contact *current = head;
        int found = 0;
        while (current != NULL)
        {
            if (strncmp(current->name, prefix, strlen(prefix)) == 0)
            {
                printf("Name: %s, Phone: %s, Address: %s, Email: %s\n", current->name, current->phone, current->address, current->email);
                found = 1;
            }
            current = current->next;
        }
        if (!found)
        {
            printf("No contacts found starting with '%s'.\n", prefix);
        }
    }
    else if (choice == 2)
    {
        char query[MAX_NAME];
        printf("Enter the partial name to auto-complete: ");
        fgets(query, MAX_NAME, stdin);
        query[strcspn(query, "\n")] = '\0'; // Remove newline

        Contact *current = head;
        int found = 0;
        while (current != NULL)
        {
            if (strstr(current->name, query) != NULL)
            { // Check if query is part of the name
                printf("Did you mean: %s? (Phone: %s, Email: %s)\n", current->name, current->phone, current->email);
                found = 1;
            }
            current = current->next;
        }
        if (!found)
        {
            printf("No contacts found matching '%s'.\n", query);
        }
    }
    else if (choice == 3)
    {
        char nameQuery[MAX_NAME], phoneQuery[MAX_PHONE], emailQuery[MAX_EMAIL];
        printf("Enter name to search (leave empty to skip): ");
        fgets(nameQuery, MAX_NAME, stdin);
        nameQuery[strcspn(nameQuery, "\n")] = '\0';

        printf("Enter phone to search (leave empty to skip): ");
        fgets(phoneQuery, MAX_PHONE, stdin);
        phoneQuery[strcspn(phoneQuery, "\n")] = '\0';

        printf("Enter email to search (leave empty to skip): ");
        fgets(emailQuery, MAX_EMAIL, stdin);
        emailQuery[strcspn(emailQuery, "\n")] = '\0';

        Contact *current = head;
        int found = 0;
        while (current != NULL)
        {
            int match = 1;
            if (strlen(nameQuery) > 0 && strstr(current->name, nameQuery) == NULL)
            {
                match = 0;
            }
            if (strlen(phoneQuery) > 0 && strstr(current->phone, phoneQuery) == NULL)
            {
                match = 0;
            }
            if (strlen(emailQuery) > 0 && strstr(current->email, emailQuery) == NULL)
            {
                match = 0;
            }
            if (match)
            {
                printf("Name: %s, Phone: %s, Address: %s, Email: %s\n", current->name, current->phone, current->address, current->email);
                found = 1;
            }
            current = current->next;
        }
        if (!found)
        {
            printf("No contacts found matching the criteria.\n");
        }
    }
    else
    {
        printf("Invalid choice. Returning to main menu.\n");
    }
}

// filter contacts by date range
void filterContactsByDate(const char *startDate, const char *endDate)
{
    if (head == NULL)
    {
        printf("No contacts available.\n");
        return;
    }

    Contact *current = head;
    int found = 0;

    printf("\nContacts added between %s and %s:\n", startDate, endDate);
    while (current != NULL)
    {
        // making sure the date matches or is within the range
        if (strcmp(current->dateAdded, startDate) >= 0 && strcmp(current->dateAdded, endDate) <= 0)
        {
            printf("Name: %s, Phone: %s, Address: %s, Email: %s, Date Added: %s\n",
                   current->name, current->phone, current->address, current->email, current->dateAdded);
            found = 1;
        }
        current = current->next;
    }

    if (!found)
    {
        printf("No contacts found within the specified date range.\n");
    }
}

// function to filter by the dates
void filterMenu()
{
    char startDate[MAX_DATE], endDate[MAX_DATE];
    char currentDate[MAX_DATE];

    // getting the current date
    getCurrentDate(currentDate);

    printf("Enter start date (DD-MM-YYYY): ");
    fgets(startDate, MAX_DATE, stdin);
    startDate[strcspn(startDate, "\n")] = '\0'; // Remove trailing newline

    // clear the input buffer to avoid issues with leftover characters
    while (getchar() != '\n' && getchar() != EOF)
    {
    }

    printf("Enter end date (DD-MM-YYYY) (leave empty for today's date): ");
    fgets(endDate, MAX_DATE, stdin);
    endDate[strcspn(endDate, "\n")] = '\0'; // Remove trailing newline

    if (strlen(endDate) == 0)
    {
        strncpy(endDate, currentDate, MAX_DATE);
    }

    filterContactsByDate(startDate, endDate);
}

// Main menu
int main()
{
    loadContacts();

    int choice;
    char name[MAX_NAME], phone[MAX_PHONE], address[MAX_ADDRESS], email[MAX_EMAIL];
    char input[MAX_NAME];

    while (1)
    {
        printf("\n=====================================\n");
        printf("         Contact Management          \n");
        printf("=====================================\n");
        printf("1. Add Contact\n");
        printf("2. Search Contact\n");
        printf("3. Delete Contact\n");
        printf("4. Edit Contact\n");
        printf("5. Display All Contacts\n");
        printf("6. Sort Contacts\n");
        printf("7. Advanced Search Contacts\n");
        printf("8. Filter Contacts by Date Range\n");
        printf("9. Exit\n");
        printf("Choose an option: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("INVALID INPUT. Please enter a number.\n");
            while (getchar() != '\n')
                ; // clear invalid input from buffer
            continue;
        }
        getchar(); // remove newline character
        switch (choice)
        {
        case 1:
            printf("Name: ");
            fgets(name, MAX_NAME, stdin);
            name[strcspn(name, "\n")] = '\0';
            printf("Phone: ");
            fgets(phone, MAX_PHONE, stdin);
            phone[strcspn(phone, "\n")] = '\0';
            printf("Address: ");
            fgets(address, MAX_ADDRESS, stdin);
            address[strcspn(address, "\n")] = '\0';
            printf("Email: ");
            fgets(email, MAX_EMAIL, stdin);
            email[strcspn(email, "\n")] = '\0';

            addContact(name, phone, address, email);
            break;
        case 2:
            printf("Search by:\n1. Name\n2. Phone\n3. Address\n4. Email\nChoose an option: ");
            int searchType;
            if (scanf("%d", &searchType) != 1 || searchType < 1 || searchType > 4)
            {
                printf("INVALID INPUT. Please choose a valid option.\n");
                while (getchar() != '\n')
                    ; // clear invalid input from buffer
                continue;
            }
            getchar(); // remove newline character
            printf("Enter search term: ");
            fgets(input, MAX_NAME, stdin);
            input[strcspn(input, "\n")] = '\0';
            searchContact(searchType, input);
            break;
        case 3:
            printf("Enter name to delete: ");
            fgets(name, MAX_NAME, stdin);
            name[strcspn(name, "\n")] = '\0';
            deleteContact(name);
            break;
        case 4:
            printf("Enter name to edit: ");
            fgets(name, MAX_NAME, stdin);
            name[strcspn(name, "\n")] = '\0';
            editContact(name);
            break;
        case 5:
            displayAllContacts();
            break;
        case 6:
            sortContacts();
            break;
        case 7:
            advancedSearch();
            break;
        case 8:
            filterMenu();
            break;
        case 9:
            printf("Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
