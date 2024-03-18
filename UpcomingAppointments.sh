#!/bin/bash

# MySQL connection information
HOST="localhost"
USER="root"
PASS="student"
DB="hospital"

# Query to fetch upcoming appointments with status 'Scheduled'
QUERY="SELECT * FROM Appointment WHERE AppointmentDate >= CURDATE() AND Status = 'Scheduled' ORDER BY AppointmentDate ASC;"

# Connect to MySQL and fetch appointments, then save to CSV
mysql -h $HOST -u $USER -p$PASS $DB -e "$QUERY" | sed 's/\t/,/g' > UpcomingAppointments.csv

# Check if there are any upcoming appointments
if [ ! -s UpcomingAppointments.csv ]; then
    echo "No upcoming appointments found."
    exit 0
fi

echo "Upcoming appointments have been saved to UpcomingAppointments.csv."
