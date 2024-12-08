#include "PandACW_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Contact* head = NULL;

// Function to check if a phone number is valid
int isValidPhone(const char *phone) {
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i]) && phone[i] != '-') {
            return 0;
        }
    }
    return 1;
}

// Function to check if an email is valid
int isValidEmail(const char *email) {
    int at_found = 0;
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            at_found = 1;
        }
    }
    return at_found;
}

// Function to add a contact to the list
void addContact(const char *name, const char *phone, const char *address, const char *email) {
    if (!isValidPhone(phone)) {
        printf("Invalid phone number. Please try again.\n");
        return;
    }

    if (!isValidEmail(email)) {
        printf("Invalid email address. Please try again.\n");
        return;
    }

    Contact *newContact = (Contact *)malloc(sizeof(Contact));
    strncpy(newContact->name, name, MAX_NAME);
    strncpy(newContact->phone, phone, MAX_PHONE);
    strncpy(newContact->address, address, MAX_ADDRESS);
    strncpy(newContact->email, email, MAX_EMAIL);
    newContact->next = NULL;

    // Check for unique name
    Contact *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("A contact with the same name already exists.\n");
            free(newContact);
            return;
        }
        current = current->next;
    }

    // Insert while maintaining sorted order
    if (head == NULL || strcmp(name, head->name) < 0) {
        newContact->next = head;
        head = newContact;
    } else {
        current = head;
        while (current->next != NULL && strcmp(current->next->name, name) < 0) {
            current = current->next;
        }
        newContact->next = current->next;
        current->next = newContact;
    }

    // Now write the new contact to the file
    FILE *file = fopen("contact.txt", "a");  // Open in append mode
    if (file != NULL) {
        fprintf(file, "%s,%s,%s,%s\n", name, phone, address, email);  // Save the contact in CSV format
        fclose(file);
    } else {
        printf("Failed to open file to save contact.\n");
    }
}

// Function to load contacts from a file
void loadContacts() {
    FILE *file = fopen("contact.txt", "r");
    if (file == NULL) {
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char name[MAX_NAME], phone[MAX_PHONE], address[MAX_ADDRESS], email[MAX_EMAIL];
        sscanf(line, "%[^,],%[^,],%[^,],%s", name, phone, address, email);
        addContact(name, phone, address, email);
    }
    fclose(file);
}

// Function to display all contacts
void displayAllContacts() {
    if (head == NULL) {
        printf("No contacts available.\n");
        return;
    }

    Contact *current = head;
    printf("\nContact List:\n");
    while (current != NULL) {
        printf("Name: %s, Phone: %s, Address: %s, Email: %s\n", current->name, current->phone, current->address, current->email);
        current = current->next;
    }
}

// Function to search for a contact by name
Contact* searchContact(const char *name) {
    Contact *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to delete a contact by name
void deleteContact(const char *name) {
    if (head == NULL) {
        printf("Contact list is empty.\n");
        return;
    }

    Contact *current = head;
    Contact *previous = NULL;

    while (current != NULL && strcmp(current->name, name) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Contact not found.\n");
        return;
    }

    if (previous == NULL) {
        head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current);
    printf("Contact has been deleted.\n");
}

// Function to edit a contact
void editContact(const char *name, const char *newPhone, const char *newAddress, const char *newEmail) {
    Contact *contact = searchContact(name);
    if (contact == NULL) {
        printf("Contact not found.\n");
        return;
    }

    if (newPhone != NULL && isValidPhone(newPhone)) {
        strncpy(contact->phone, newPhone, MAX_PHONE);
    }
    if (newAddress != NULL) {
        strncpy(contact->address, newAddress, MAX_ADDRESS);
    }
    if (newEmail != NULL && isValidEmail(newEmail)) {
        strncpy(contact->email, newEmail, MAX_EMAIL);
    }

    printf("Contact has been updated.\n");
}
