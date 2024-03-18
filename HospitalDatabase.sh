#!/bin/bash

# Run each object file individually
for file in hospitalDatabase PatientCSV DoctorCSV AppointmentsCSV TestResultsCSV EmergencyContactsCSV; do
    echo "Running $file..."
    ./"$file"
done
