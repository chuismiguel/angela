#!/bin/bash

# Define variables
REMOTE_USER="angela"
REMOTE_HOST="192.168.68.62"
REMOTE_PATH="/home/angela"
LOCAL_PATH="angela"
PASSWORD="maria"

# Copy the src folder from Windows to Linux using scp
echo "Copying src folder from Windows to Linux..."

sshpass -p "$PASSWORD" scp -r "$LOCAL_PATH"/* "$REMOTE_USER@$REMOTE_HOST:$REMOTE_PATH"

if [ $? -eq 0 ]; then
    echo "Folder src copied successfully to $REMOTE_PATH on $REMOTE_HOST"
else
    echo "Failed to copy folder src to $REMOTE_PATH on $REMOTE_HOST"
fi
