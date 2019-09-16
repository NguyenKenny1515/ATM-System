#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void menu(int *choicePtr);
//void printArray(struct account arr[], int *newAccountIndex);
void transaction(struct account arr[], int *newAccountIndex);
void createAccount(struct account arr[], int *newAccountIndex);
void menu2(int *choicePtr, int accountIndex);
void deposit(struct account arr[], int accountIndex);
void withdraw(struct account arr[], int accountIndex);
void end(struct account arr[], int size);
void welcome();
void farewell();

struct account {
	unsigned int accountNumber;	// will be randomly generated
	char name[30]; //name of account holder
	double balance;
};

int main() {
	srand(time(NULL));
	// Opens file, and fills arr with data in file
	FILE *accountsPtr; // Stores all account numbers, upon program initiation, read everything from file and put in array. Search array for matching numbers.
	struct account arr[100] = { 0 }; // For now, initialize an array of size 100. Possibly allocate memory dynamically depending on number of account numbers in file.
	int newAccountIndex;	// Keep track of the index to put new account into arr
	if ((accountsPtr = fopen("AccountNumbers.txt", "r")) == NULL) {
		puts("File could not be opened");
	}
	else {
		// Fill array with all accounts
		int i = 0;
		while (!feof(accountsPtr)) {
			fscanf(accountsPtr, "%d%29s%lf", &arr[i].accountNumber, arr[i].name, &arr[i].balance);
			i++;
		}
		newAccountIndex = i - 1;	// Set newAccountIndex to next empty index in arr
	}
	fclose(accountsPtr);
	welcome(); // Greet user
	transaction(arr, &newAccountIndex); // Start of program interaction with user
	end(arr, newAccountIndex); // Write array to file
	farewell(); // Say goodbye to user
	system("pause");
	return 0;
}

void transaction(struct account arr[], int *newAccountIndex) {
	int choice = -1;

	while (choice != 0) {
		menu(&choice); // Output menu options
		switch (choice) {
			// Quits the program
		case 0:
			printf("Signing out . . .\n");
			break;

			// Creates a new bank account
		case 1:
			createAccount(arr, newAccountIndex);
			//int choice2 = 0;
			printf("Your account has been created! Redirecting to main menu...\n");

			break;

			// Search for an already existing account. If such account does not exist, ask if user wants to create an account or quit
		case 2: {
			unsigned int enteredNum = 0;
			int i = 0;
			printf("Please enter your account number: ");
			scanf_s("%u", &enteredNum);
			// linear search through array
			for (; i <= *newAccountIndex - 1; i++) {
				if (arr[i].accountNumber == enteredNum)
					break;
			}
			// If account does not exist, return to menu
			if (i == *newAccountIndex) {
				puts("Sorry, no account linked to your account number has been found! Redirecting to main menu...\n");
				break;
			}
			// If account exists, open second menu
			printf("\nAccount successfully found!\nAccount number: %u\nName: %s\nBalance: $%.2lf\n\n", arr[i].accountNumber, arr[i].name, arr[i].balance);
			int choice2 = -1;
			while (choice2 != 0) {
				menu2(&choice2, i);
				switch (choice2) {
				case 1:
					deposit(arr, i);
					printf("Returning to options...\n");
					break;
				case 2:
					withdraw(arr, i);
					printf("Returning to options...\n");
					break;
				}
				puts("");
			}
		}
		}
	}
}

// Outputs various menu options. Also updates choice with user input. Includes input validation
void menu(int *choicePtr) {
	do {
		printf("Choice 0: Exit the program\n");
		printf("Choice 1: Create an Account\n");
		printf("Choice 2: Access your Account\n");
		printf("Please choose an option: ");
		scanf_s("%d", choicePtr);
		puts("");
	} while (*choicePtr < 0 && *choicePtr > 2);
}

//Output choices such as deposit, withdraw, go back, etc.
void menu2(int *choicePtr, int index) {
	do {
		printf("Choice 0: Return to the start menu\n");
		printf("Choice 1: Deposit to your Account\n");
		printf("Choice 2: Withdraw from your Account\n");
		printf("Please choose an option: ");
		scanf_s("%d", choicePtr);
		puts("");
	} while (*choicePtr < 0 && *choicePtr > 2);
}

// Creates a new bank account
void createAccount(struct account arr[], int *newAccountIndex) {
	printf("Please enter your first name: ");
	scanf("%29s", arr[*newAccountIndex].name);

	// Input validation: balance has to be positive
	do {
		printf("Please enter your balance: ");
		scanf("%lf", &arr[*newAccountIndex].balance);
	} while (arr[*newAccountIndex].balance < 0);

	arr[*newAccountIndex].accountNumber = 100000 + rand() % 900000;

	printf("\nAccount successfully created!\nAccount number: %u\nName: %s\nBalance: $%.2lf\n\n", arr[*newAccountIndex].accountNumber, arr[*newAccountIndex].name, arr[*newAccountIndex].balance);
	*newAccountIndex = *newAccountIndex + 1;	// Increment newAccountIndex to next empty index in arr
}

// Adds money to the account's balance
void deposit(struct account arr[], int accountIndex) {
	double amount;
	do {
		printf("Enter the amount you want to deposit from your Account: ");
		scanf("%lf", &amount);
	} while (amount < 0);
	arr[accountIndex].balance += amount;
	printf("Your balance is now $%.2lf\n\n", arr[accountIndex].balance);
}

// Removes money from the account's balance
void withdraw(struct account arr[], int accountIndex) {
	double amount;
	do {
		printf("Enter the amount you want to withdraw from your Account: ");
		scanf("%lf", &amount);
	} while (amount > arr[accountIndex].balance || amount < 0);
	arr[accountIndex].balance -= amount;
	printf("Your balance is now $%.2lf\n\n", arr[accountIndex].balance);
}

// Write whole array to file (accounts for any new accounts created, any deposits/withdrawals made)
void end(struct account arr[], int size) {
	FILE *ptr = fopen("AccountNumbers.txt", "wb");
	for (int i = 0; i < size; i++) {
		fprintf(ptr, "%d %s %.2f\n", arr[i].accountNumber, arr[i].name, arr[i].balance);
	}
	fclose(ptr);
}

void welcome() {
	printf("********************************************\n");
	printf("Welcome to our ATM!\n");
	printf("********************************************\n");
}

void farewell() {
	printf("********************************************\n");
	printf("We hope to see you soon!\n");
	printf("********************************************\n");
}