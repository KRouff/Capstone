#!/bin/bash

# Define the backup folder path
backup_folder="/home/student/Capstone/Backup"

# Define the destination folder path
destination_folder="/home/student/Capstone"

# Copy all files from the backup folder to the destination folder
cp -r "$backup_folder"/* "$destination_folder"

echo "Files restored successfully."
