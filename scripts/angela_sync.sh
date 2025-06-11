#!/bin/bash
PI_USER=angela  
PI_IP=192.168.68.62
SSHPASS=maria  # Replace with actual password

sshpass -p ${SSHPASS} rsync -avz --delete \
    --exclude='build/' \
    --exclude='.git/' \
    --exclude='.vscode/' \
    --rsh="ssh -o StrictHostKeyChecking=no" \
    ./ ${PI_USER}@${PI_IP}:~/angela-cpp