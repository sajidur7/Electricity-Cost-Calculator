#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#define MAX_USERS 100

typedef struct
{
    char username[50];
    char password[50];
    bool hasAccount;
} User;

typedef struct
{
    char username[50];
    double quantity;
    double unitPrice;
    double unitsConsumed;
    double currentMonthBill;
} UserData;

// Function prototypes
void calculateBill(UserData *user);
void provideTips(double currentMonthBill);
int loadUserData(User *users, int *numUsers);
void saveUserData(User *users, int numUsers);
int findUser(User *users, int numUsers, const char *username, const char *password);
void createAccount(User *users, int *numUsers, const char *username, const char *password);
void historicalDataAnalysis(UserData *userData, int numData);
void viewHistoricalData(User *users, UserData *historicalUserData, int numUsers, int numData, int loggedInUserIndex);
void displayWelcomeMenu();
void loginPage(User *users, int *numUsers, int *loggedInUserIndex);
void createAccountPage(User *users, int *numUsers);
void userDataEntry(User *users, UserData *historicalUserData, int numUsers, int *numData, int loggedInUserIndex, int isGuest);
int loadData(UserData *historicalUserData);
void changePassword(User *users, int numUsers, int loggedInUserIndex);
void deleteAccount(User *users, int numUsers, int loggedInUserIndex);
void userMenu(User *users, int numUsers, int loggedInUserIndex);
void clearHistoricalData(const char *username);
void about();

int main()
{
    system("cls");
    system("COLOR 5E");
    User users[MAX_USERS];
    UserData historicalUserData[MAX_USERS];
    int numUsers = 0;
    int numData = 0;
    int loggedInUserIndex = -1;
    int isGuest = 0;

    if (!loadUserData(users, &numUsers))
    {
        FILE *file = fopen("users.txt", "w");
    }

    int choice;
    while (1)
    {
        system("cls");
        displayWelcomeMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            system("cls");
            loginPage(users, &numUsers, &loggedInUserIndex);
            isGuest = 0;
            break;
        case 2:
            system("cls");
            createAccountPage(users, &numUsers);
            isGuest = 0;
            break;
        case 3:
            system("cls");
            isGuest = 1;
            userDataEntry(users, historicalUserData, numUsers, &numData, loggedInUserIndex, isGuest);
        case 4:
            system("cls");
            about();
            break;
        case 5:
            exit(0);
        default:
            printf("Invalid choice.\n");
        }

        if (loggedInUserIndex != -1)
        {
            userDataEntry(users, historicalUserData, numUsers, &numData, loggedInUserIndex, isGuest);
        }
    }

    return 0;
}

void displayWelcomeMenu()
{

    printf("\t\t\t\t*******************************************\t\t\t\t\n");
    printf("\t\t\t\tWelcome to the Electricity Cost Calculator!\t\t\t\t\n");
    printf("\t\t\t\t*******************************************\t\t\t\t\n\n");
    printf("\t\t\t\t1. Login\t\t\t\t\n");
    printf("\t\t\t\t2. Create an Account\t\t\t\t\n");
    printf("\t\t\t\t3. Guest Mode\t\t\t\t\n");
    printf("\t\t\t\t4. About Programmer\t\t\t\t\n");
    printf("\t\t\t\t5. Exit\t\t\t\t\n");
}

void loginPage(User *users, int *numUsers, int *loggedInUserIndex)
{
    printf("\t\t\t\t\t******\t\t\t\t\t\n");
    printf("\t\t\t\t\tLog In\t\t\t\t\n");
    printf("\t\t\t\t\t******\t\t\t\t\n\n");
    char username[50];
    char password[50];

    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    *loggedInUserIndex = findUser(users, *numUsers, username, password);

    if (*loggedInUserIndex == -1)
    {
        printf("Error: Invalid username or password.\n");
        system("pause");
    }
    else{
        printf("\n");
        printf("--------------Successfully Log In");
        printf("\n");
    }

}

void createAccountPage(User *users, int *numUsers)
{

    printf("\t\t\t\t\t**************\t\t\t\t\t\n");
    printf("\t\t\t\t\tCreate Account\t\t\t\t\n");
    printf("\t\t\t\t\t**************\t\t\t\t\n\n");
    char username[50];
    char password[50];

    printf("Enter a new username: ");
    scanf("%s", username);
    printf("Enter a new password: ");
    scanf("%s", password);

    createAccount(users, numUsers, username, password);
}

void userDataEntry(User *users, UserData *historicalUserData, int numUsers, int *numData, int loggedInUserIndex, int isGuest)
{
    UserData userData;
    strcpy(userData.username, isGuest ? "Guest" : users[loggedInUserIndex].username);
    printf("\nEnter the quantity of electricity used (in kWh): ");
    scanf("%lf", &userData.quantity);
    printf("Enter the unit price (in $/kWh): ");
    scanf("%lf", &userData.unitPrice);
    printf("Enter the units consumed: ");
    scanf("%lf", &userData.unitsConsumed);
    system("cls");
    calculateBill(&userData);

    if (loggedInUserIndex != -1 && users[loggedInUserIndex].hasAccount)
    {
        FILE *dataFile = fopen("userdata.txt", "a");
        if (dataFile == NULL)
        {
            printf("Error: Unable to open data file for writing.\n");
            return;
        }
        fprintf(dataFile, "%s %.2lf %.2lf %.2lf %.2lf\n", userData.username, userData.quantity, userData.unitPrice, userData.unitsConsumed, userData.currentMonthBill);
        fclose(dataFile);
    }

    provideTips(userData.currentMonthBill);

    *numData = loadData(historicalUserData);
    viewHistoricalData(users, historicalUserData, numUsers, *numData, loggedInUserIndex);
    system("pause");
}

void calculateBill(UserData *user)
{
    user->currentMonthBill = user->quantity * user->unitPrice * user->unitsConsumed;
}

void provideTips(double currentMonthBill)
{

    printf("\nElectricity Bill for the Current Month: $%.2f\n\n", currentMonthBill);

    if (currentMonthBill < 50)
    {
        printf("Great job! You are using electricity efficiently.\n");
    }
    else if (currentMonthBill < 100)
    {
        printf("You can save money by reducing your electricity usage.\n");
    }
    else
    {
        printf("Your electricity bill is high. Consider energy-saving practices.\n");
    }

    printf("\nTips to Save Electricity:\n");
    printf("*************************\n");
    printf("1. Use energy-efficient LED bulbs.\n");
    printf("2. Unplug chargers and devices when they are not charging.\n");
    printf("3. Set your thermostat to an energy-saving temperature.\n");
    printf("4. Regularly maintain your appliances for optimal efficiency.\n");
}

int loadUserData(User *users, int *numUsers)
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        return 0;
    }

    int hasData = 0;

    while ((*numUsers < MAX_USERS) && (fscanf(file, "%s %s %d", users[*numUsers].username, users[*numUsers].password, &users[*numUsers].hasAccount) != EOF))
    {
        (*numUsers)++;
        if (users[*numUsers - 1].hasAccount)
        {
            hasData = 1;
        }
    }

    fclose(file);
    return 1;
}

void saveUserData(User *users, int numUsers)
{
    FILE *file = fopen("users.txt", "w");
    if (file == NULL)
    {
        printf("Error: Unable to save user data.\n");
        return;
    }

    for (int i = 0; i < numUsers; i++)
    {
        fprintf(file, "%s %s %d\n", users[i].username, users[i].password, users[i].hasAccount);
    }

    fclose(file);
}

int findUser(User *users, int numUsers, const char *username, const char *password)
{
    for (int i = 0; i < numUsers; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
        {
            return i;
        }
    }
    return -1;
}

void createAccount(User *users, int *numUsers, const char *username, const char *password)
{
    if (*numUsers >= MAX_USERS)
    {
        printf("Error: Maximum number of users reached.\n");
        return;
    }

    strcpy(users[*numUsers].username, username);
    strcpy(users[*numUsers].password, password);
    users[*numUsers].hasAccount = true;

    saveUserData(users, *numUsers + 1);

    printf("Account created successfully!\n");
    (*numUsers)++;
}

void historicalDataAnalysis(UserData *userData, int numData)
{
    double totalBill = 0;
    double totalQuantity = 0;
    double averageBill;

    for (int i = 0; i < numData; i++)
    {
        totalBill += userData[i].currentMonthBill;
        totalQuantity += userData[i].quantity;
    }

    switch (numData)
    {
    case 0:
        printf("\nNo historical data available for analysis.\n");
        break;
    default:
        averageBill = totalBill / numData;

        printf("\nUser Data Analysis:\n");
        printf("*******************\n");
        printf("Total Electricity Consumed: %.2lf kWh\n", totalQuantity);
        printf("Average Monthly Bill: $%.2f\n", averageBill);
        printf("\n\n");

        break;
    }

    return 0;
}

void viewHistoricalData(User *users, UserData *historicalUserData, int numUsers, int numData, int loggedInUserIndex)
{

    printf("\nUser Monthly History:\n");
    printf("******************\n");

    if (loggedInUserIndex != -1 && users[loggedInUserIndex].hasAccount)
    {
        printf("User: %s\n", users[loggedInUserIndex].username);
        for (int j = 0; j < numData; j++)
        {
            if (strcmp(historicalUserData[j].username, users[loggedInUserIndex].username) == 0)
            {
                printf("Month: %d - Quantity: %.2lf kWh - Bill: $%.2f\n", j + 1, historicalUserData[j].quantity, historicalUserData[j].currentMonthBill);
            }
        }
        historicalDataAnalysis(historicalUserData, numData);
    }
    else
    {
        printf("No user is currently logged in.\n");
    }
}

int loadData(UserData *historicalUserData)
{
    int numData = 0;
    FILE *historicalDataFile = fopen("userdata.txt", "r");
    if (historicalDataFile != NULL)
    {
        while (fscanf(historicalDataFile, "%s %lf %lf %lf %lf",
                      historicalUserData[numData].username, &historicalUserData[numData].quantity,
                      &historicalUserData[numData].unitPrice, &historicalUserData[numData].unitsConsumed,
                      &historicalUserData[numData].currentMonthBill) != EOF)
        {
            numData++;
        }
        fclose(historicalDataFile);
    }
    return numData;
}

void changePassword(User *users, int numUsers, int loggedInUserIndex)
{
    char newPassword[50];
    printf("Enter a new password: ");
    scanf("%s", newPassword);
    strcpy(users[loggedInUserIndex].password, newPassword);
    saveUserData(users, numUsers);
    printf("Password updated successfully!\n");
}

void deleteAccount(User *users, int numUsers, int loggedInUserIndex)
{
    users[loggedInUserIndex].hasAccount = false;
    saveUserData(users, numUsers);
    clearHistoricalData(users[loggedInUserIndex].username);
    printf("Account deleted successfully!\n");
}

void userMenu(User *users, int numUsers, int loggedInUserIndex)
{
    int choice;

    while (1)
    {
        printf("\nUser Menu:\n");
        printf("1. Change Password\n");
        printf("2. Delete Account\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            changePassword(users, numUsers, loggedInUserIndex);
            break;
        case 2:
            deleteAccount(users, numUsers, loggedInUserIndex);
            return;
        case 3:
            return;
        default:
            printf("Invalid choice.\n");
        }
    }
}
void clearHistoricalData(const char *username)
{
    FILE *historicalDataFile = fopen("userdata.txt", "r");
    FILE *tempFile = fopen("tempdata.txt", "w");

    if (historicalDataFile != NULL && tempFile != NULL)
    {
        char buffer[200];
        while (fgets(buffer, sizeof(buffer), historicalDataFile))
        {
            char dataUsername[50];
            sscanf(buffer, "%s", dataUsername);
            if (strcmp(dataUsername, username) != 0)
            {
                fprintf(tempFile, "%s", buffer);
            }
        }

        fclose(historicalDataFile);
        fclose(tempFile);

        remove("userdata.txt");
        rename("tempdata.txt", "userdata.txt");

        printf("Historical data cleared successfully!\n");
    }
    else
    {
        printf("Error: Unable to clear historical data.\n");
    }

    int main()
    {
        User users[MAX_USERS];
        UserData historicalUserData[MAX_USERS];
        int numUsers = 0;
        int numData = 0;
        int loggedInUserIndex = -1;

        if (!loadUserData(users, &numUsers))
        {
            printf("Error: Unable to load or create user data file.\n");
            return 1;
        }

        int choice;
        while (1)
        {
            switch (loggedInUserIndex)
            {
            case -1:
                choice = mainMenu(users, historicalUserData, &numUsers, &numData, &loggedInUserIndex);
                break;
            default:
                userMenu(users, numUsers, loggedInUserIndex);
                break;
            }

            if (choice == -1)
            {
                break;
                // Exit the loop if the user chooses to exit
            }
        }

        return 0;
    }
}

void about()
{
    system("cls");
    printf("\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t****************\n");
    printf("\t\t\t\tAbout Programmer\n");
    printf("\t\t\t\t*****************\n");
    printf("\n");
    printf("\t\t\t\t******************************************\n");
    printf("\t\t\t\tProject Name : Electricity Cost Calculator\n");
    printf("\t\t\t\t******************************************\n");
    printf("\t\t\t\t\n");
    printf("\t\t\t\tSubmitted To\n");
    printf("\t\t\t\t************\n");
    printf("\t\t\t\tMd. Shohel Arman\n");
    printf("\t\t\t\tAssistant Professor\n");
    printf("\t\t\t\tDepartment of Software Engineering\n");
    printf("\t\t\t\tDaffodil International University\n");
    printf("\t\t\t\t\n");
    printf("\t\t\t\tSubmitted By\n");
    printf("\t\t\t\t************\n");
    printf("\t\t\t\tName : Md Sajidur Rahman Shimul\n");
    printf("\t\t\t\tID : 0242220005341051\n");
    printf("\t\t\t\tBatch : 39th\n");
    printf("\t\t\t\tSection : A\n");
    printf("\t\t\t\tDepartment : Software Engineering\n");
    printf("\t\t\t\tInstitute : Daffodil International University\n");
    printf("\t\t\t\t*********************************************\n");
    printf("\n\n\n\t\t\t\t\tEnter any keys to continue.......");
    getch();
}
