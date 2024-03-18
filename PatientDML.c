#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASSWORD "student"
#define DB_NAME "hospital"

MYSQL *conn;

// Function to execute a query and handle errors
void execute_query(const char *query) {
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query execution error: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }
}

// Function to register a new patient
void registerNewPatient() {
    // Input variables for patient details
    char firstName[255], lastName[255], dob[11], gender[6], address[255], city[255], state[255], zipCode[20], phone[20], email[255];

    // Getting patient details from user
    printf("\nEnter Patient Details:\n");
    printf("First Name: ");
    scanf("%s", firstName);
    printf("Last Name: ");
    scanf("%s", lastName);
    printf("Date of Birth (YYYY-MM-DD): ");
    scanf("%s", dob);
    printf("Gender (Male/Female/Other): ");
    scanf("%s", gender);
    printf("Address: ");
    scanf(" %[^\n]", address);
    printf("City: ");
    scanf("%s", city);
    printf("State: ");
    scanf("%s", state);
    printf("Zip Code: ");
    scanf("%s", zipCode);
    printf("Phone: ");
    scanf("%s", phone);
    printf("Email: ");
    scanf("%s", email);

    // Constructing and executing query to insert patient details into database
    char insertQuery[2048];
    snprintf(insertQuery, sizeof(insertQuery),
             "INSERT INTO Patient (FirstName, LastName, DateOfBirth, Gender, Address, City, State, ZipCode, Phone, Email) "
             "VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
             firstName, lastName, dob, gender, address, city, state, zipCode, phone, email);

    if (mysql_query(conn, insertQuery)) {
        fprintf(stderr, "Failed to add patient: Error: %s\n", mysql_error(conn));
    } else {
        printf("Patient added successfully!\n");
    }
}

// Function to update existing patient details
void updateExistingPatient() {
    // Input variables for patient ID, update choice, and new value
    int patientID, updateChoice;
    char newValue[255];

    // Getting patient ID and update choice from user
    printf("\nEnter Patient ID to update: ");
    scanf("%d", &patientID);

    printf("\nWhat would you like to update?\n");
    printf("1. First Name\n");
    printf("2. Last Name\n");
    printf("3. Date of Birth\n");
    printf("4. Address\n");
    printf("5. City\n");
    printf("6. State\n");
    printf("7. Zip Code\n");
    printf("Enter your choice: ");
    scanf("%d", &updateChoice);

    // Getting new value based on update choice
    printf("Enter the new value: ");
    scanf(" %[^\n]", newValue);

    // Constructing and executing query to update patient details in database
    char updateQuery[512];
    switch (updateChoice) {
        case 1:
            sprintf(updateQuery, "UPDATE Patient SET FirstName = '%s' WHERE PatientID = %d", newValue, patientID);
            break;
        case 2:
            sprintf(updateQuery, "UPDATE Patient SET LastName = '%s' WHERE PatientID = %d", newValue, patientID);
            break;
        case 3:
            sprintf(updateQuery, "UPDATE Patient SET DateOfBirth = '%s' WHERE PatientID = %d", newValue, patientID);
            break;
        case 4:
            sprintf(updateQuery, "UPDATE Patient SET Address = '%s' WHERE PatientID = %d", newValue, patientID);
            break;
        case 5:
            sprintf(updateQuery, "UPDATE Patient SET City = '%s' WHERE PatientID = %d", newValue, patientID);
            break;
        case 6:
            sprintf(updateQuery, "UPDATE Patient SET State = '%s' WHERE PatientID = %d", newValue, patientID);
            break;
        case 7:
            sprintf(updateQuery, "UPDATE Patient SET ZipCode = '%s' WHERE PatientID = %d", newValue, patientID);
            break;
        default:
            printf("Invalid update choice.\n");
            return;
    }

    if (mysql_query(conn, updateQuery)) {
        fprintf(stderr, "Failed to update patient: Error: %s\n", mysql_error(conn));
    } else {
        printf("Patient updated successfully!\n");
    }
}

// Function to search patient records based on a search term
void searchPatientRecords() {
    // Input variable for search term
    char searchTerm[100];
    printf("Enter search term (First name, Last Name or ID): ");
    scanf("%s", searchTerm);

    // Constructing and executing query to search for patients based on search term
    char selectQuery[512];
    sprintf(selectQuery, "SELECT * FROM Patient WHERE FirstName LIKE '%%%s%%' OR LastName LIKE '%%%s%%' OR PatientID = '%s'", searchTerm, searchTerm, searchTerm);

    if (mysql_query(conn, selectQuery)) {
        fprintf(stderr, "Failed to search patients: Error: %s\n", mysql_error(conn));
        return;
    }

    // Displaying search results
    MYSQL_RES *result = mysql_use_result(conn);
    MYSQL_ROW row;

    printf("\nSearch Results:\n");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| PatientID | First Name | Last Name  | Date of Birth | Gender | Address          | City          | State  | Zip Code | Phone          | Email                 |\n");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("| %-9s | %-10s | %-10s | %-12s | %-6s | %-16s | %-14s | %-7s | %-8s | %-14s | %-20s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10]);
    }

    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    mysql_free_result(result);
}

// Function to search for a patient's emergency contact
void searchPatientEmergencyContact() {
    // Input variable for patient ID
    int patientID;
    printf("Enter Patient ID to search for emergency contact: ");
    scanf("%d", &patientID);

    // Constructing and executing query to search for emergency contact based on patient ID
    char selectQuery[1024];
    snprintf(selectQuery, sizeof(selectQuery),
             "SELECT * FROM EmergencyContacts WHERE PatientID=%d", patientID);

    execute_query(selectQuery);
    MYSQL_RES *result = mysql_use_result(conn);
    if (result) {
        MYSQL_ROW row;
        printf("\nEmergency Contact for Patient %d:\n", patientID);
        printf("+-------------------------+--------------+----------------------+\n");
        printf("| Name                    | Relationship | Phone                |\n");
        printf("+-------------------------+--------------+----------------------+\n");
        while ((row = mysql_fetch_row(result))) {
            char contactInfo[255];
            snprintf(contactInfo, sizeof(contactInfo), "%s %s", row[2], row[3]);
            printf("| %-23s | %-12s | %-20s |\n", contactInfo, row[4], row[5]);
        }
        printf("+-------------------------+--------------+----------------------+\n");
        mysql_free_result(result);
    } else {
        printf("No emergency contact found for Patient %d.\n", patientID);
    }
}


// Function to add an emergency contact for a patient
void addEmergencyContact() {
    // Input variables for patient ID, emergency contact name, relationship, and phone
    int patientID;
    char Firstname[255],Lastname[255], relationship[255], phone[20];

    // Getting patient ID, emergency contact name, relationship, and phone from user
    printf("Enter Patient ID: ");
    scanf("%d", &patientID);
    printf("Enter Emergency Contact First Name: ");
    scanf("%s", Firstname);
    printf("Enter Emergency Contact Last Name: ");
    scanf("%s", Lastname);
    printf("Enter Relationship: ");
    scanf("%s", relationship);
    printf("Enter Phone Number: ");
    scanf("%s", phone);

    // Constructing and executing query to add emergency contact for patient
    char insertQuery[1024];
    snprintf(insertQuery, sizeof(insertQuery),
             "INSERT INTO EmergencyContacts (PatientID, FirstName, LastName, Relationship, Phone) "
             "VALUES (%d, '%s', '%s', '%s', '%s')",
             patientID, Firstname, Lastname, relationship, phone);

    if (mysql_query(conn, insertQuery)) {
        fprintf(stderr, "Failed to add emergency contact: Error: %s\n", mysql_error(conn));
    } else {
        printf("Emergency contact added successfully!\n");
    }
}

int main() {
    // Initializing MySQL connection
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "Error initializing MySQL: %s\n", mysql_error(conn));
        return EXIT_FAILURE;
    }

    // Connecting to MySQL database
    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, 0, NULL, 0) == NULL) {
        fprintf(stderr, "Error connecting to MySQL: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    int choice;
    do {
        // Main menu
        printf("\nMain Menu:\n");
        printf("1. Register New Patient\n");
        printf("2. Update Existing Patient\n");
        printf("3. Search Patient Records\n");
        printf("4. Search Patient Emergency Contact\n");
        printf("5. Add Patient Emergency Contact\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Check if the choice is within the valid range
        if (choice < 1 || choice > 6) {
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
            // Clear input buffer
            while(getchar() != '\n');
            continue; // Continue to the next iteration of the loop
        }

        // Menu options
        switch (choice) {
            case 1:
                registerNewPatient();
                break;
            case 2:
                updateExistingPatient();
                break;
            case 3:
                searchPatientRecords();
                break;
            case 4:
                searchPatientEmergencyContact();
                break;
            case 5:
                addEmergencyContact();
                break;
            case 6:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    // Closing MySQL connection
    mysql_close(conn);
    return EXIT_SUCCESS;
}
