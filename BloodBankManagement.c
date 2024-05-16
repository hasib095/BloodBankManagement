// Header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structures
typedef struct Date
{
    int day, month, year;
} Date;

typedef struct Donor
{
    char name[100];
    char gender[2];
    char bloodType[5];
    char contactNumber[12];
    char gmailAddress[100];
    Date lastDonationDate;
    char status[50];
    struct Donor *next;
} Donor;

// Global variables
Donor *head = NULL; // Linked list head pointer
Donor *stack[100];  // Stack for deleted donors
int top = -1;       // Stack top pointer
int numDonors = 0;  // Number of donors

// Function prototypes
void push(Donor *donor);
Donor *pop();
void enqueue(Donor *donor);
Donor *dequeue();
void addDonor();
void displayDonors();
void searchDonorsByBloodType();
void updateDonor();
void deleteDonor();
void saveDonorsToFile();
void readDonorsFromFile();

// Main function
int main()
{
    readDonorsFromFile();

    int choice;
    while (1)
    {
        printf("===============================================");
        printf("\n         Blood Bank Management System        \n");
        printf("===============================================\n");
        printf("                 1. Add Donor\n");
        printf("                 2. Display Donors\n");
        printf("                 3. Search Donors by Blood Type\n");
        printf("                 4. Update Donor\n");
        printf("                 5. Delete Donor\n");
        printf("                 6. Exit\n");
        printf("                 Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addDonor();
            break;
        case 2:
            displayDonors();
            break;
        case 3:
            searchDonorsByBloodType();
            break;
        case 4:
            updateDonor();
            break;
        case 5:
            deleteDonor();
            break;
        case 6:
            saveDonorsToFile();
            printf("Successfully Exit.\n");
            exit(0);
        default:
            printf("Invalid choice!\n");
        }
    }
}
// Function to push a donor into the stack
void push(Donor *donor)
{
    if (top == 99)
    {
        printf("Stack overflow!\n");
        return;
    }
    stack[++top] = donor;
}

// Function to pop a donor from the stack
Donor *pop()
{
    if (top == -1)
    {
        printf("Stack underflow!\n");
        return NULL;
    }
    return stack[top--];
}

// Function to enqueue a donor into the queue
void enqueue(Donor *donor)
{
    if (head == NULL)
    {
        head = donor;
        head->next = NULL;
    }
    else
    {
        Donor *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = donor;
        donor->next = NULL;
    }
}

// Function to dequeue a donor from the queue
Donor *dequeue()
{
    if (head == NULL)
    {
        printf("Queue underflow!\n");
        return NULL;
    }
    Donor *temp = head;
    head = head->next;
    return temp;
}

int isLeapYear(int year)
{
    if (year % 400 == 0)
    {
        return 1;
    }
    else if (year % 100 == 0)
    {
        return 0;
    }
    else if (year % 4 == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int getDaysInMonth(int month, int year)
{
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year))
    {
        return 29;
    }
    else
    {
        return daysInMonth[month - 1];
    }
}

int dateDifference(struct Date date1, struct Date date2)
{
    int days1 = date1.day;
    int days2 = date2.day;

    for (int i = 1; i < date1.month; i++)
    {
        days1 += getDaysInMonth(i, date1.year);
    }

    for (int i = 1; i < date2.month; i++)
    {
        days2 += getDaysInMonth(i, date2.year);
    }

    int res = days2 - days1;
    for (int i = date1.year; i < date2.year; i++)
    {
        res += isLeapYear(i) ? 366 : 365;
    }

    return res;
}

// Function to get the current date
void getCurrentDate(Date *currDate)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    currDate->day = tm_info->tm_mday;
    currDate->month = tm_info->tm_mon + 1;    // Month is 0-based, so add 1
    currDate->year = tm_info->tm_year + 1900; // Years since 1900
}

// Function to add a new donor
void addDonor()
{
    Donor *newDonor = (Donor *)malloc(sizeof(Donor));
    // setbuf(stdin, NULL);
    fflush(stdin);
    printf("\nEnter Donor Name: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]s", newDonor->name);
    printf("Enter Gender (M/F): ");
    setbuf(stdin, NULL);
    scanf("%[^\n]s", newDonor->gender);
    printf("Enter Blood Type (A/B/AB/O +/-): ");
    setbuf(stdin, NULL);
    scanf("%[^\n]s", newDonor->bloodType);
    printf("Enter Contact Number: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]s", newDonor->contactNumber);
    printf("Enter Gmail address: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]s", newDonor->gmailAddress);
    printf("Enter last date of donating blood (e.g., 29-2-2023): ");
    scanf("%d-%d-%d", &newDonor->lastDonationDate.day, &newDonor->lastDonationDate.month, &newDonor->lastDonationDate.year);
    fflush(stdout);

    Date currDate;
    getCurrentDate(&currDate);
    int daysInBetween = dateDifference(newDonor->lastDonationDate, currDate);
    if ((strcmp(newDonor->gender, "F") == 0 && daysInBetween >= 120) || (strcmp(newDonor->gender, "M") == 0 && daysInBetween >= 90))
        strcpy(newDonor->status, "Eligible");
    else
        strcpy(newDonor->status, "Not Eligible");

    enqueue(newDonor);
    numDonors++;
    printf("Donar Added Successfully\n");
}

// Function to display all donors
void displayDonors()
{
    if (head == NULL)
    {
        printf("\nNo donors found.\n");
        return;
    }
    printf("\nList of Donors:\n");
    Donor *temp = head;
    while (temp != NULL)
    {
        printf("\nName: %s\nGender: %s\nBlood Type: %s\nContact Number: %s\nGmail Address: %s\nLast Donation Date: %d-%d-%d\nStatus: %s\n", temp->name, temp->gender, temp->bloodType, temp->contactNumber, temp->gmailAddress, temp->lastDonationDate.day, temp->lastDonationDate.month, temp->lastDonationDate.year, temp->status);
        temp = temp->next;
    }
}

// Function to search donors by blood type
void searchDonorsByBloodType()
{
    char bloodType[5];
    printf("\nEnter blood type to search (A/B/AB/O +/-): ");
    scanf("%s", bloodType);

    if (head == NULL)
    {
        printf("\nNo donors found.\n");
        return;
    }

    Donor *temp = head;
    int found = 0;
    while (temp != NULL)
    {
        if (strcmp(temp->bloodType, bloodType) == 0)
        {
            printf("\nName: %s\nGender: %s\nBlood Type: %s\nContact Number: %s\nGmail Address: %s\nLast Donation Date: %d-%d-%d\nStatus: %s\n", temp->name, temp->gender, temp->bloodType, temp->contactNumber, temp->gmailAddress, temp->lastDonationDate.day, temp->lastDonationDate.month, temp->lastDonationDate.year, temp->status);
            found = 1;
        }
        temp = temp->next;
    }
    if (!found)
        printf("\nNo donors found with blood type %s.\n", bloodType);
}

// Function to update donor information
void updateDonor()
{
    char donorName[100];
    printf("\nEnter the name of the donor you want to update: ");
    scanf("%s", donorName);

    Donor *temp = head;
    int found = 0;
    while (temp != NULL)
    {
        if (strcmp(temp->name, donorName) == 0)
        {
            printf("\nUpdating donor information for %s:\n", donorName);
            printf("Enter new Contact Number: ");
            scanf("%s", temp->contactNumber);
            printf("Enter new Gmail address: ");
            scanf("%s", temp->gmailAddress);
            printf("Enter new last donation date (e.g., 29-2-2023): ");
            scanf("%d-%d-%d", &temp->lastDonationDate.day, &temp->lastDonationDate.month, &temp->lastDonationDate.year);

            int daysInBetween;
            if ((strcmp(temp->gender, "F") == 0 && daysInBetween >= 120) || (strcmp(temp->gender, "M") == 0 && daysInBetween >= 90))
                strcpy(temp->status, "Eligible for Blood Donation.");
            else
                strcpy(temp->status, "Not Eligible");
            printf("\nDonor information updated successfully!\n");
            found = 1;
            break;
        }
        temp = temp->next;
    }
    if (!found)
        printf("\nNo donor found with the name %s.\n", donorName);
}

// Function to delete a donor
void deleteDonor()
{
    if (head == NULL)
    {
        printf("\nNo donors found.\n");
        return;
    }

    char donorName[100];
    printf("\nEnter the name of the donor you want to delete: ");
    scanf("%s", donorName);

    Donor *prev = NULL;
    Donor *temp = head;
    int found = 0;
    while (temp != NULL)
    {
        if (strcmp(temp->name, donorName) == 0)
        {
            if (prev == NULL)
                head = temp->next;
            else
                prev->next = temp->next;
            push(temp);
            printf("\nDonor %s has been deleted from the database.\n", donorName);
            numDonors--;
            found = 1;
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    if (!found)
        printf("\nNo donor found with the name %s.\n", donorName);
}

// Function to save donors to a file
void saveDonorsToFile()
{
    FILE *fp = fopen("donors.txt", "w");
    if (fp == NULL)
    {
        printf("\nError: Cannot open the file for writing.\n");
        return;
    }

    Donor *temp = head;
    while (temp != NULL)
    {
        fprintf(fp, "%s,%s,%s,%s,%s,%d-%d-%d,%s\n", temp->name, temp->gender, temp->bloodType, temp->contactNumber, temp->gmailAddress, temp->lastDonationDate.day, temp->lastDonationDate.month, temp->lastDonationDate.year, temp->status);
        temp = temp->next;
    }

    fclose(fp);
}

// Function to read donors from a file
void readDonorsFromFile()
{
    FILE *fp = fopen("donors.txt", "r");
    if (fp == NULL)
    {
        printf("\nNo existing donor data found.\n");
        return;
    }
    while (!feof(fp))
    {
        Donor *newDonor = (Donor *)malloc(sizeof(Donor));

        // rewind(fp);
        fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%d-%d-%d,%[^\n]\n", newDonor->name, newDonor->gender, newDonor->bloodType, newDonor->contactNumber, newDonor->gmailAddress, &newDonor->lastDonationDate.day, &newDonor->lastDonationDate.month, &newDonor->lastDonationDate.year, newDonor->status);
        enqueue(newDonor);
        numDonors++;
    }
    fclose(fp);
}
