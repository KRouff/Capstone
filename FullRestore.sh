#!/bin/bash

# Define the backup folder path
backup_folder="/home/student/Capstone/Backup"

# Define the destination folder path
destination_folder="/home/student/Capstone/Test"

# Copy all files from the backup folder to the destination folder
cp -r "$backup_folder"/* "$destination_folder"

echo "Files restored successfully."

# MySQL database name
database="hospital"

# Check if the database exists
if mysqlshow --defaults-file=/home/student/Capstone/.my.cnf "$database" >/dev/null; then
    echo "Database already exists."
else
    echo "Database does not exist. Creating..."
    mysql --defaults-file=/home/student/Capstone/.my.cnf -e "CREATE DATABASE $database;"
    echo "Database created."
fi

# Restore the MySQL database from the backup
mysql --defaults-file=/home/student/Capstone/.my.cnf "$database" < "$backup_folder"/hospital_backup.sql

echo "Database restored successfully."
