#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUSES 5
#define MAX_SEATS 50
#define TICKET_FARE 1400
#define COUPON_CODE "VVV20OFF"
#define DISCOUNTED_FARE 1250
#define HASH_TABLE_SIZE 50

int hashTable[HASH_TABLE_SIZE] = {0};

struct Passenger
{
    char name[50];
    int age;
    char gender;
    char pickupPoint[50];
    char destination[50];
    int fare;
};

struct Bus
{
    int busNumber;
    char name[50];
    int seats[MAX_SEATS];
    struct Passenger passengers[MAX_SEATS];
};

struct Bus buses[MAX_BUSES];

int checkMaleFemaleSeating(int busNumber, int seatNumber, struct Passenger passenger)
{
    if (busNumber >= 0 && busNumber < MAX_BUSES)
    {
        for (int i = 0; i < MAX_SEATS; i++)
        {
            if (buses[busNumber].seats[i] == 1)
            {
                struct Passenger existingPassenger = buses[busNumber].passengers[i];

                if (existingPassenger.gender != passenger.gender)
                {
                    if (abs(i - seatNumber) == 1)
                    {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

void applyCoupon(struct Passenger *passenger, const char *couponCode)
{
    if (strcmp(couponCode, COUPON_CODE) == 0)
    {
        passenger->fare = DISCOUNTED_FARE;
        printf("Previous Amount %d\n", TICKET_FARE);
    }
    else
    {
        passenger->fare = rand() % (2000 - 1000 + 1) + 1000; // Generate a random fare between 1000 and 2000
    }
}

void askForRating()
{
    int rating;
    printf("Please rate our service (1-5): ");
    scanf("%d", &rating);

    if (rating >= 1 && rating <= 5)
    {
        printf("Thank you for your feedback! You rated our service with %d stars.\n", rating);
    }
    else
    {
        printf("Invalid rating. Please enter a rating between 1 and 5.\n");
    }
}

void storeTicketInFile(int busNumber, int seatNumber, struct Passenger *passenger)
{
    FILE *filePointer;
    filePointer = fopen("booked_tickets.txt", "a");

    if (filePointer == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    fprintf(filePointer, "Bus Number: %d, Seat Number: %d\n", busNumber, seatNumber);
    fprintf(filePointer, "Passenger Name: %s\n", passenger->name);
    fprintf(filePointer, "Age: %d\n", passenger->age);
    fprintf(filePointer, "Gender: %c\n", passenger->gender);
    fprintf(filePointer, "Pick up Point: %s\n", passenger->pickupPoint);
    fprintf(filePointer, "Destination: %s\n", passenger->destination);
    fprintf(filePointer, "Fare: %d Rs\n", buses[busNumber].passengers[seatNumber].fare);
    fprintf(filePointer, "BBBBB  OOO   OOO  K  K  EEEE  DDDD\n"
                         "B   B O   O O   O K K   E     D   D\n"
                         "BBBBB O   O O   O KK    EEEE  D   D\n"
                         "B   B O   O O   O K K   E     D   D\n"
                         "BBBBB  OOO   OOO  K  K  EEEE  DDDD\n\n\n\n");

    fclose(filePointer);
}

void bookOneWayTicket(int busNumber, int seatNumber, struct Passenger *passenger, const char *couponCode)
{
    if (busNumber >= 0 && busNumber < MAX_BUSES && seatNumber >= 0 && seatNumber < MAX_SEATS)
    {
        if (buses[busNumber].seats[seatNumber] == 0)
        {
            if (checkMaleFemaleSeating(busNumber, seatNumber, *passenger) == 1)
            {
                buses[busNumber].seats[seatNumber] = 1;
                buses[busNumber].passengers[seatNumber] = *passenger;

                applyCoupon(&buses[busNumber].passengers[seatNumber], couponCode);

                printf("One-way ticket booked successfully! Fare: %d Rs\n", buses[busNumber].passengers[seatNumber].fare);
                storeTicketInFile(busNumber, seatNumber, passenger);
                askForRating();
            }
            else
            {
                printf("Cannot book a one-way ticket. Male and female passengers cannot sit together.\n");
            }
        }
        else
        {
            printf("Seat is already occupied.\n");
        }
    }
    else
    {
        printf("Invalid bus or seat number.\n");
    }
}

void displayTicket(int busNumber, int seatNumber)
{
    if (busNumber >= 0 && busNumber < MAX_BUSES && seatNumber >= 0 && seatNumber < MAX_SEATS)
    {
        if (buses[busNumber].seats[seatNumber] == 1)
        {
            struct Passenger passenger = buses[busNumber].passengers[seatNumber];
            printf("Bus Number: %d, Bus Name: %s, Seat Number: %d\n", busNumber, buses[busNumber].name, seatNumber);
            printf("Name: %s\n", passenger.name);
            printf("Age: %d\n", passenger.age);
            printf("Gender: %c\n", passenger.gender);
            printf("Pick up Point: %s\n", passenger.pickupPoint);
            printf("Destination: %s\n", passenger.destination);
            printf("Fare: %d Rs\n", passenger.fare);
        }
        else
        {
            printf("Seat is unoccupied.\n");
        }
    }
    else
    {
        printf("Invalid bus or seat number.\n");
    }
}

void cancelTicket(int busNumber, int seatNumber)
{
    if (busNumber >= 0 && busNumber < MAX_BUSES && seatNumber >= 0 && seatNumber < MAX_SEATS)
    {
        if (buses[busNumber].seats[seatNumber] == 1)
        {
            buses[busNumber].seats[seatNumber] = 0;
            printf("Ticket canceled successfully!\n");
            printf("Your fee of Rs 1400 will be refunded to you shortly!\n");
        }
        else
        {
            printf("Seat is already unoccupied.\n");
        }
    }
    else
    {
        printf("Invalid bus or seat number.\n");
    }
}

void displayTicketsLeft(int busNumber)
{
    if (busNumber >= 0 && busNumber < MAX_BUSES)
    {
        int ticketsLeft = 0;
        for (int i = 0; i < MAX_SEATS; i++)
        {
            if (buses[busNumber].seats[i] == 0)
            {
                ticketsLeft++;
            }
        }
        printf("Bus Number: %d, Bus Name: %s, Tickets Left: %d\n", busNumber, buses[busNumber].name, ticketsLeft);
    }
    else
    {
        printf("Invalid bus number.\n");
    }
}

int main()
{
    char busNames[] = {'a', 'b', 'c', 'd', 'e'};

    for (int i = 0; i < MAX_BUSES; i++)
    {
        buses[i].busNumber = i;
        memset(buses[i].seats, 0, sizeof(buses[i].seats));
        snprintf(buses[i].name, sizeof(buses[i].name), "Bus %c", busNames[i]);
    }

    int choice;
    do
    {
        printf("\nWelcome To NammaBus\n");
        printf("\nBus Booking System Menu:\n");
        printf("1. Book a Ticket\n");
        printf("2. Display a Ticket\n");
        printf("3. Cancel a Ticket\n");
        printf("4. Display Tickets Left for a Bus\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            int busNumber, seatNumber;
            struct Passenger passenger;

            printf("Enter Bus Number (0-4): ");
            scanf("%d", &busNumber);
            if (busNumber < 0 || busNumber >= MAX_BUSES)
            {
                printf("Invalid bus number. Please try again.\n");
                continue;
            }
            printf("Enter Seat Number (0-49): ");
            scanf("%d", &seatNumber);
            if (seatNumber < 0 || seatNumber >= MAX_SEATS)
            {
                printf("Invalid seat number. Please try again.\n");
                continue;
            }

            printf("Enter Passenger Name: ");
            scanf("%s", passenger.name);
            printf("Enter Passenger Age: ");
            scanf("%d", &passenger.age);
            printf("Enter Passenger Gender (M/F): ");
            scanf(" %c", &passenger.gender);
            printf("Enter Pick up Point: ");
            scanf("%s", passenger.pickupPoint);
            printf("Enter Destination: ");
            scanf("%s", passenger.destination);
            char couponCode[20];
            printf("Enter Coupon Code (if available): ");
            scanf("%s", couponCode);

            bookOneWayTicket(busNumber, seatNumber, &passenger, couponCode);
            break;
        }
        case 2:
        {
            int busNumber, seatNumber;
            printf("Enter Bus Number: ");
            scanf("%d", &busNumber);
            printf("Enter Seat Number: ");
            scanf("%d", &seatNumber);
            displayTicket(busNumber, seatNumber);
            break;
        }
        case 3:
        {
            int busNumber, seatNumber;
            printf("Enter Bus Number: ");
            scanf("%d", &busNumber);
            printf("Enter Seat Number: ");
            scanf("%d", &seatNumber);
            cancelTicket(busNumber, seatNumber);
            break;
        }
        case 4:
        {
            int busNumber;
            printf("Enter Bus Number: ");
            scanf("%d", &busNumber);
            displayTicketsLeft(busNumber);
            break;
        }
        case 5:
        {
            printf("Exiting the program. Thank you!\n");
            break;
        }
        default:
            printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);

    return 0;
}