#!/bin/bash

# Function to install notify-send (part of libnotify-bin package)
install_notify_send() {
    # Check if apt-get is available (Ubuntu/Debian)
    if command -v apt-get &> /dev/null; then
        sudo apt-get install -y libnotify-bin
    # Check if yum is available (CentOS/RHEL)
    elif command -v yum &> /dev/null; then
        sudo yum install -y libnotify-bin
    # If neither apt-get nor yum is available, display an error message
    else
        echo "Error: Package manager not found. Please install libnotify-bin manually."
        exit 1
    fi
}

# Check if notify-send is installed
if ! command -v notify-send &> /dev/null; then
    # If notify-send is not installed, call the install_notify_send function
    install_notify_send
fi

# Define the filesystems to monitor
filesystems=("/home/student")

# Loop through each filesystem
for i in "${filesystems[@]}"; do
    # Get disk usage percentage for the filesystem
    usage=$(df -h "$i" | tail -n 1 | awk '{print $5}' | cut -d % -f1)

    # Check if disk usage is over 50%
    if [ $usage -ge 50 ]; then
        # If disk usage is over 50%, construct an alert message
        alert="Running out of space on $i, Usage is: $usage%"
        echo "Alert: $alert"
        # Send a desktop notification using notify-send
        DISPLAY=:0 notify-send "Disk Space Alert" "$alert"
    fi
done
