#ifndef FILE_MANAGEMENT
#define FILE_MANAGEMENT
#pragma once
// THIS HEADER IS USED TO HAVE ALL THE LIBRARIES NEEDED AND FOR FUNCTIONS USED AS TOOLS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dirent.h>
#include <windows.h>
#include <string.h>


unsigned file_exists(char fileName[200])// checks if the file exists
{
    FILE *file;
    unsigned value = 0;
    if(file = fopen(fileName, "r"))
        value = 1;
    
    fclose(file);
    return value;

}

unsigned directory_accounts(DIR *folder)// checks if the directory exists
{
    unsigned value = 0;
    if(folder = opendir("accounts"))
        value = 1;
    closedir(folder);
    return value;
    
}

void main_file_data_load(FILE *file, char content[][200], int *dim)// loads into local memory the
//content of the main file
{
    unsigned index = 0;
    while(!feof(file))
    {
        char textLine[200];
        fscanf(file, " %100[^\n]", textLine);
        if(strlen(textLine) >= 1)
        {
            strcpy(content[index], textLine);
            ++index;
        }
    }
    *dim = index;
}

unsigned isFileEmpty(FILE *file)// checks if the file is empty
{
    int c = fgetc(file);
    if(c == EOF)
        return 1;
    return 0;
}

unsigned keyExists(char content[100][200], char newKeyName[], int dim)// checks if the name already
//exists in the main file
{
    for(int index = 0; index < dim; index++)
        if(strcmp(newKeyName, content[index]) == 0)
            return 1;
    return 0;
}

void addNew_key(FILE *file, int *dim, char keyName[200], char username[200], char password[200])
//this functions creates a new file in the accounts folder with the input name
//and also adds the keyname to the registered accounts file
{
    char keyPath[] = "accounts\\";
    strcat(keyPath, keyName);
    strcat(keyPath, ".txt");
    FILE *newfile = fopen(keyPath, "a");
    fprintf(newfile, "\n%s\n%s", username, password);
    fclose(newfile);
    *dim += 1;
    fprintf(file, "%s\n", keyName);
    

}

void list_existing_keys(FILE *file, int dim)// lists every key name in the registered_accounts file
{
    char listing[100][200];
    int index = 0;

    while(!feof(file))
    {
        char keyName[200];
        fscanf(file, " %100[^\n]", keyName);
        if(strlen(keyName) >= 1)
        {
            strcpy(listing[index], keyName);
            ++index;
        }
    }
    dim = index - 1;
    for(index = 0; index < dim; index++)
        printf("[%i] - %s\n", index + 1, listing[index]);
    
}

void remove_file_process(FILE *mainFile, int targetIndex, int *dim)// removes a specified file and the key name in registered accounts file
{
    char targetFile[200], targetFilePath[200] = "accounts\\";
    mainFile = fopen("registered_accounts.txt", "r");

    int index = 0;
    while(!feof(mainFile))
    {
        char buffer[200];
        fscanf(mainFile, " %100[^\n]", buffer);
        if(targetIndex - 1 == index)
        {
            strcpy(targetFile, buffer);
            strcat(targetFilePath, buffer);
            strcat(targetFilePath, ".txt");
            break;
        }
        ++index;
    }
    fclose(mainFile);
    if(remove(targetFilePath) == 0)
    {
        index = 0;
        char keyNames[100][200];
        mainFile = fopen("registered_accounts.txt", "r");
        while(!feof(mainFile))
        {
            char buffer[200];
            fscanf(mainFile, " %100[^\n]", buffer);
            if(strlen(buffer) >= 1)
            {
                strcpy(keyNames[index], buffer);
                ++index;
            }
        }
        *dim = index - 1;
        fclose(mainFile);
        mainFile = fopen("registered_accounts.txt", "w");
        fprintf(mainFile, "\n");
        fclose(mainFile);
        mainFile = fopen("registered_accounts.txt", "a");
        for(index = 0; index < *dim; index++)
            if(strcmp(targetFile, keyNames[index]) != 0)
                fprintf(mainFile, "%s\n", keyNames[index]);
        fclose(mainFile);
    }
    *dim -= 1;
}

void file_dimension(FILE *file, int *dim)//counts how many keys are in the registered_accounts
{
    file = fopen("registered_accounts.txt", "r");
    int index = 0;

    while(!feof(file))
    {
        char buffer[200];
        fscanf(file, " %100[^\n]", buffer);
        if(strlen(buffer) >= 1)
            ++index;
    }
    *dim = index - 1;

    fclose(file);
}

void copyTOclipboard(char output[200])
{
    
    const size_t len = strlen(output) + 1;
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), output, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

void accTOclipboard(char accName[200])// reads the username and the password from a specified file and loads them into clipboard 
{
    char accFilePath[200] = "accounts\\";
    strcat(accFilePath, accName);
    strcat(accFilePath, ".txt");
    FILE *accFile = fopen(accFilePath, "r");

    char username[200], password[200];
    fscanf(accFile, " %100[^\n]", username);
    fscanf(accFile, " %100[^\n]", password);

    fclose(accFile);

    printf("\nUSERNAME has been loaded to clipboard. Press any key to copy the PASSWORD.");
    copyTOclipboard(username);
    _getch();
    printf("\nPASSWORD has been loaded to clipboard. Press any key to come back to the list.");
    copyTOclipboard(password);
    _getch();


}

#endif