#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX_LINE_LENGTH 1024
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASSWORD "student"
#define DB_NAME "hospital"

MYSQL *conn; // MySQL connection object

// Execute a MySQL query and handle any errors
void execute_query(const char *query) {
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query execution error: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }
}

// Function to enter test results into the database
void enterTestResults() {
    int patientID, doctorID;
    char testType[255], result[255], testDate[11];

     // Get input from user
    printf("Enter Patient ID: ");
    scanf("%d", &patientID);
    printf("Enter Doctor ID: ");
    scanf("%d", &doctorID);
    printf("Enter Test Type: ");
    scanf("%s", testType);
    printf("Enter Result: ");
    scanf("%s", result);
    printf("Enter Test Date (YYYY-MM-DD): ");
    scanf("%s", testDate);

    // Construct and execute the SQL query
    char insertQuery[1024];
    snprintf(insertQuery, sizeof(insertQuery),
             "INSERT INTO TestResults (PatientID, DoctorID, TestType, Result, TestDate) "
             "VALUES (%d, %d, '%s', '%s', '%s')",
             patientID, doctorID, testType, result, testDate);

    execute_query(insertQuery);
    printf("Test results entered successfully!\n");
}

 // Function to retrieve test results from the database
void retrieveTestResults() {
    int patientID;

     // Get input from user
    printf("Enter Patient ID to retrieve results: ");
    scanf("%d", &patientID);

     // Construct and execute the SQL query
    char selectQuery[512];
    snprintf(selectQuery, sizeof(selectQuery),
             "SELECT * FROM TestResults WHERE PatientID = %d", patientID);

     // Print the retrieved test results
    execute_query(selectQuery);
    MYSQL_RES *result = mysql_use_result(conn);
    if (result) {
        MYSQL_ROW row;
        printf("\nTest Results:\n");
        while ((row = mysql_fetch_row(result))) {
            printf("TestResultID: %s, PatientID: %s, DoctorID: %s, TestType: %s, Result: %s, TestDate: %s\n",
                   row[0], row[1], row[2], row[3], row[4], row[5]);
        }
        mysql_free_result(result);
    } else {
        printf("No test results found for Patient ID %d\n", patientID);
    }
}

int main() {
    // Initialize MySQL connection
    conn = mysql_init(NULL);
    if (!conn) {
        fprintf(stderr, "Error initializing MySQL: %s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }

    // Connect to MySQL server
    if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, 0, NULL, 0)) {
        fprintf(stderr, "Error connecting to MySQL: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

     // Display menu and get user choice
    int choice;
    do {
        printf("\nSelect an option:\n");
        printf("1. Enter Test Results\n");
        printf("2. Retrieve Past Results\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Check if the choice is within the valid range
        if (choice < 1 || choice > 3) {
            printf("Invalid choice. Please enter a number between 1 and 5.\n");
            // Clear input buffer
            while(getchar() != '\n');
            continue; // Continue to the next iteration of the loop
        }

        switch (choice) {
            case 1:
                enterTestResults();
                break;
            case 2:
                retrieveTestResults();
                break;
            case 3:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    // Close MySQL connection
    mysql_close(conn);
    return EXIT_SUCCESS;
}
