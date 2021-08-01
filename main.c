#include "file_management.h"


void init_parts(DIR *folder) // this function makes sure the main file and directory exist
{
    FILE *file;
    if(!file_exists("registered_accounts.txt"))
    {
        file = fopen("registered_accounts.txt", "w");
        fprintf(file, "\n");
        fclose(file);
    }

    if(!directory_accounts(folder))
        mkdir("accounts");

}

void new_key(FILE *file, int *dim)// the main function that manages the process
//of registering a new file with log in details
{
    
    char file_content[100][200], textInput[200];// textInput este key name
    char username[200], password[200];
    unsigned isValid;// it turn 0 if at least one of the conditions is broken

    system("cls");
    printf("============= NEW KEY =============\n");

    do{
        file = fopen("registered_accounts.txt", "r");
        isValid = 1;
        printf("\n        KEY NAME: ");
        
        scanf(" %100[^\n]", textInput);
        if(strlen(textInput) < 2)
            isValid = 0;
        if(!isFileEmpty(file))
        {
            main_file_data_load(file, file_content, dim);
            if(keyExists(file_content, textInput, *dim))
                isValid = 0;
        }
        fclose(file);
        if(!isValid)
            printf("\nThe Key name given as input is invalid.\nMake sure your key name is longer than 1 character and doesn\'t already exists");
    }while(!isValid);

    
    

    do{
        isValid = 1;
        printf("\n        USERNAME: ");
        scanf(" %100[^\n]", username);
        if(strlen(username) < 2)
            isValid = 0;
        if(!isValid)
            printf("\nThe input must be longer than 1 character!");
    }while(!isValid);

    do{
        isValid = 1;
        printf("\n        PASSWORD: ");
        scanf(" %100[^\n]", password);
        if(strlen(password) < 2)
            isValid = 0;
        if(!isValid)
            printf("\nThe input must be longer than 1 character!");
    }while(!isValid);

    file = fopen("registered_accounts.txt", "a");
    addNew_key(file, dim, textInput, username, password);
    fclose(file);
    printf("\n\nA new key by the name %s has been created.\n\nUSERNAME: %s\nPASSWORD: %s", textInput, username, password);
    printf("\n\nPress any key to go back to main menu.");
    _getch();
}

void remove_keys(FILE *file, int *dim)
{
    

    char select;
    do{
        do{
            system("cls");
            printf("============= REMOVE KEYS =============\n");
            printf("\n[1] - Remove one key");
            printf("\n[2] - Remove all keys");
            printf("\n[3] - Back to menu");
            printf("\nSelect: ");
            select = _getch();
        }while(select < '1' || select > '3');

        switch(select)
        {
            case '1':
            {
                int sel;
                do{
                    system("cls");
                    printf("============= REMOVE KEYS - List =============\n");
                    file = fopen("registered_accounts.txt", "r");
                    list_existing_keys(file, *dim);
                    printf("\n[0] - Back to remover menu\n");
                    fclose(file);
                    printf("\nSelect: ");
                    scanf("%i", &sel);
                    if(sel == 0)
                        break;
                    remove_file_process(file, sel, dim);
                    
                }while(sel < 0 || sel > *dim - 1);
                break;
            }
            case '2':
            {
                
                file_dimension(file, dim);
                int initialDimension = *dim;
                for(int index = 1; index <= initialDimension; index++)
                    remove_file_process(file, 1, dim);
                printf("\nAll keys were removed");
                _getch();
                break;
            }
        }

    }while(select != '3');
}

void list_select_accounts(FILE *file, int *dim)
{
    file_dimension(file, dim);
    int sel;
    do{
        do{
            system("cls");
            printf("============= YOUR KEYS =============\n\n");
            file = fopen("registered_accounts.txt", "r");
            list_existing_keys(file, *dim);
            fclose(file);
            printf("\n[0] - Back to menu\n");
            printf("\nSelect: ");
            scanf("%i", &sel);

        }while(sel < 0 || sel > *dim);

    if(sel != 0)
    {
        int index = 0;
        char targetKey[200];
        file = fopen("registered_accounts.txt", "r");
        while(!feof(file))
        {
            char buffer[200];
            fscanf(file, " %100[^\n]", buffer);
            if(strlen(buffer) >= 1)
                ++index;
            if(index == sel)
            {
                strcpy(targetKey, buffer);
                break;
            }
        }
        fclose(file);
        accTOclipboard(targetKey);
        
    }


    }while(sel != 0);
}

void main_menu(FILE *file, int *dim)// the function that offers the main options and decides where to
//redirect the user
{
    char select;
    do{
        do{
            system("cls");
            printf("============= CLIPBOARD KEY =============\n\n");
            printf("[1] - List your accounts\n");
            printf("[2] - Store a new key\n");
            printf("[3] - Remove keys\n");
            printf("[4] - Close the program\n\n");
            printf("=========================================\n");
            printf("Select: ");
            select = _getch();
        }while(select < '1' || select > '4');
        switch(select)
        {
            case '1':
                list_select_accounts(file, dim);
                break;
            case '2':
                new_key(file, dim);
                break;
            case '3':
                remove_keys(file, dim);
                break;
            case '4':
                exit(0);
                break;

        }
    }while(select != '4');
}

int main()
{
    FILE *accounts_list;
    DIR *directory_folder_accounts;
    int accounts_list_dimension;
    
    init_parts(directory_folder_accounts);
    main_menu(accounts_list, &accounts_list_dimension);

    _getch();
    return 0;
}