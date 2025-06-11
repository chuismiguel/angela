@echo off

:: Define variables
set REMOTE_USER=angela
set REMOTE_HOST=angela.local
set REMOTE_PATH=/home/angela
set LOCAL_PATH=angela
set PASSWORD=maria

:: Copy the src folder from Windows to Linux using pscp
echo Copying src folder from Windows to Linux...

sshpass -p %PASSWORD% scp -r %LOCAL_PATH%/* %REMOTE_USER%@%REMOTE_HOST%:%REMOTE_PATH%

echo Folder src copied successfully to %REMOTE_PATH% on %REMOTE_HOST%
