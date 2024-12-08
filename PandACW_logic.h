#ifndef PANDACW_LOGIC_H
#define PANDACW_LOGIC_H

#define MAX_NAME 50
#define MAX_PHONE 15
#define MAX_ADDRESS 100
#define MAX_EMAIL 50

typedef struct Contact {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char address[MAX_ADDRESS];
    char email[MAX_EMAIL];
    struct Contact *next;
} Contact;

extern Contact *head;

// Function declarations
void addContact(const char *name, const char *phone, const char *address, const char *email);
void loadContacts();
void displayAllContacts();
int isValidPhone(const char *phone);
int isValidEmail(const char *email);

// Additional functions
void deleteContact(const char *name);  // Declaration for deleting a contact
void editContact(const char *name, const char *newPhone, const char *newAddress, const char *newEmail);  // Declaration for editing a contact

#endif
