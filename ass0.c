/********************************************************************************************
* Filename: ass0.c
* Version: 1.0
* Author: Christina Tucker
* Student Number: 040717525
* Course Name and Number: C++
* Lab Section: 
* Assignment 0
* FleaBay
* Due: Sept 23, 2015
* Submission Date: Sept 23, 2015
* Profesor's Name: Andrew Tyler
* Purpose: C language console application for an online sales management system.
*********************************************************************************************/

#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <crtdbg.h>

typedef enum { FALSE = 0, TRUE }BOOL;

typedef struct
{
	char* ID;
	char* PassWord;
}Account, *pAccount, **ppAccount;

typedef struct
{
	unsigned int numAccounts;
	ppAccount accounts;
}FleaBay, *pFleaBay;

void FleaBayInit(pFleaBay);			/* Initialise the FleaBay instance */
int FleaBayLogin(pFleaBay);			/* login to the FleaBay */
int FleaBayReport(pFleaBay);			/* Print all the Accounts */
void FleaBayCleanUp(pFleaBay);			/* Free all dynamically allocated memory */
void AddNewAccount(pFleaBay);			/* Add a new Account to the FleaBay */

/********************************************************************************************
* Function Name: Main
* Purpose: The FleaBay main menu
* In Parameters: void
* Out Paramaters: None
* Version: 1.0
* Author: Andrew Tyler
********************************************************************************************/
int main(void)
{
	BOOL bRunning = TRUE;
	char i_response;
	FleaBay e;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	FleaBayInit(&e);

	while (bRunning)
	{
		printf("\nPlease enter your choice\n");
		printf("1. FleaBay Login\n");
		printf("2. FleaBay Report\n");
		printf("3. Quit\n");
		fflush(stdin);
		scanf("%c", &i_response);
		switch (i_response)
		{
		case '1':
			if (!FleaBayLogin(&e))
				return 1;
			break;
		case '2':
			if (!FleaBayReport(&e))
				return 1;
			break;
		case '3':
			FleaBayCleanUp(&e);
			bRunning = FALSE;
			break;
		}
	}
	return 0;
}

/********************************************************************************************
* Function Name: FleaBayInit
* Purpose: Initialize the FleaBay instance
* In Parameters: pFleaBay e
* Out Paramaters: None
* Version: 1.0
* Author: Christina Tucker
********************************************************************************************/
void FleaBayInit(pFleaBay e){		
	e->numAccounts = 0;		/*Initialize numAccounts and accounts*/
	e->accounts = NULL;
}			

/********************************************************************************************
* Function Name: FleaBayLogin
* Purpose: Log into FleaBay
* In Parameters: pFleaBay e
* Out Paramaters: None
* Version: 1.0
* Author: Christina Tucker
********************************************************************************************/
int FleaBayLogin(pFleaBay e){
	int choice;

	char idEntered[50];
	char idExisting[50];
	int validID = 0;
	char passwordEntered[50];
	char passwordExisting[50];
	int validPassword = 0;
	int flagID = 0;
	int flagPassword = 0;
	

	if (e->numAccounts == 0){
		printf("**NO ACCOUNTS**\n");	/*Let user know if there are no accounts*/
	} 

	printf("1. Add a new Account\n2. Open an existing Account\n3. Return to Main Menu\n");
	scanf("%d", &choice);
	switch (choice){
		case 1:
			AddNewAccount(e);	/*Go to AddNewAccount to add a new account to FleaBay*/
			break;
		case 2:
			printf("please enter your account ID: ");	/*Check if the ID already exists in FleaBay*/
			scanf(" %[^\n]s", &idEntered);
			for (int i = 0; i < e->numAccounts; i++){
				validID = strcmp(idEntered, e->accounts[i]->ID);
				if (validID == 0){
					flagID = 1;
					printf("please enter your password: ");	/*Check if the password matches with the ID*/
					scanf(" %[^\n]s", &passwordEntered);
					for (int i = 0; i < e->numAccounts; i++){
						validPassword = strcmp(passwordEntered, e->accounts[i]->PassWord);
						if (validPassword == 0){
							flagPassword = 1;
							printf("Account is valid.\n");
						}
					}
				}
			}

			if (flagID != 1){
				printf("%s\n is invalid ID\n", idEntered);	/*Print an error message if the ID or Password don't exist*/
			}
			if (flagID == 1 && flagPassword != 1){
				printf("%s\n password doesn't match this ID:\n", passwordEntered);
			}
			break;
		case 3:
			break;
		default: 
			printf("Invalid choice!");
			break;
	}

}

/********************************************************************************************
* Function Name: AddNewAccount
* Purpose: Add a new account to FleaBay
* In Parameters: pFleaBay e
* Out Paramaters: None
* Version: 1.0
* Author: Christina Tucker
********************************************************************************************/
void AddNewAccount(pFleaBay e){
	int i;
	int validID = 0;
	char * id = (char*)malloc(sizeof(char)* 100);	/*Dynamically allocate memory for the ID and Password*/
	char * password = (char*)malloc(sizeof(char) * 100);

	ppAccount array = malloc(sizeof(pAccount)*(e->numAccounts + 1));	/*Create an array to hold the accouts before they get copied over to e->accounts*/
	pAccount acc = malloc(sizeof(Account)); /*Hold the account information for one user so that it can be compied to temp array 'array'*/

	printf("please enter your account ID: ");
	//fgets(id, 100, stdin);
	scanf(" %[^\n]s", id);
	for (int i = 0; i < e->numAccounts; i++){	/*Check that the accound ID doesn't already exist*/
		validID = strcmp(id, e->accounts[i]->ID);
		if (validID == 0){
			printf("This ID already exists.");	
			free(acc);	/*Free memory to avoid memory leaks when an already existing ID is entered by the user*/
			free(array);
			free(id);
			free(password);
			return 1;	/*Go back to the menu*/
		}
	}
	acc->ID = malloc(strlen(id) + 1);
	acc->ID = strcpy(acc->ID, id);
	free(id);

	printf("please enter your account password: ");
	scanf(" %[^\n]s", password);
	acc->PassWord = malloc(strlen(password) + 1);
	acc->PassWord = strcpy(acc->PassWord, password);
	free(password);

	array[e->numAccounts] = acc;

	for (i = 0; i < e->numAccounts; i++){	/*Copy all the accounts currently in e->accounts into the array 'array'*/
		array[i] = e->accounts[i]; 
	}

	free(e->accounts);	/*Empty e->accounts to make room for all the accounts that were perviously in it as well as the new account that was just created*/

	e->accounts = array; /*Copy back all of those accounts as well as the new account to e->accounts*/

	e->numAccounts++;	/*Increment numAccounts by 1*/

}

/********************************************************************************************
* Function Name: FleaBayReport
* Purpose: Print all of the accounts that currently exist on FleaBay
* In Parameters: pFleaBay e
* Out Paramaters: None
* Version: 1.0
* Author: Christina Tucker
********************************************************************************************/
int FleaBayReport(pFleaBay e){ 
	int i;

	for (i = 0; i < e->numAccounts; i++){
		printf("Account ID: %s\n\n", e->accounts[i]->ID);	/*Print out all of the existing ID's by incrementing through e->accounts*/
	}

}	

/********************************************************************************************
* Function Name: FleaBayCleanUp
* Purpose: Free any dynamically allocated memory
* In Parameters: pFleaBay e
* Out Paramaters: None
* Version: 1.0
* Author: Christina Tucker
********************************************************************************************/
void FleaBayCleanUp(pFleaBay e){
	for (int i = 0; i < e->numAccounts; i++){	
		free(e->accounts[i]->ID);
	}
	for (int j = 0; j < e->numAccounts; j++){
		free(e->accounts[j]->PassWord);			/*Free all of the dynamically allocated memory*/
	}
	for (int p = 0; p < e->numAccounts; p++){
		free(e->accounts[p]);
	}
	free(e->accounts);
}			
