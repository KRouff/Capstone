#!/bin/bash

# Function to install notify-send (part of libnotify-bin package)
install_notify_send() {
    if command -v apt-get &> /dev/null; then
        sudo apt-get install -y libnotify-bin
    elif command -v yum &> /dev/null; then
        sudo yum install -y libnotify-bin
    else
        echo "Error: Package manager not found. Please install libnotify-bin manually."
        exit 1
    fi
}

# Check if notify-send is installed
if ! command -v notify-send &> /dev/null; then
    install_notify_send
fi


# Define the filesystems to monitor
filesystems=("/home/student")

for i in "${filesystems[@]}"; do
    # Get disk usage percentage
    usage=$(df -h "$i" | tail -n 1 | awk '{print $5}' | cut -d % -f1)

    # Check if disk usage is over 50%
    if [ $usage -ge 50 ]; then
        alert="Running out of space on $i, Usage is: $usage%"
        echo "Alert: $alert"
        DISPLAY=:0 notify-send "Disk Space Alert" "$alert"
    fi
done
