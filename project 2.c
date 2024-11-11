#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <Windows.h>
#include <ctype.h>

struct airline {
    char passport[6];
    char name[15];
    char destination[15];
    int seat_num;
    char email[15];
    char seat_class;
    int price;
    struct airline *following;
} *begin, *stream;

struct airline *dummy;
int total_revenue = 0;

int seats_z[51] = {0};
int seats_j[51] = {0};
int seats_y[51] = {0};

void details(struct airline *reservation);
void reserve();
void cancel();
void display();
void show_available_seats();
void check_seat_status(char seat_prefix, int seat_num);
void display_revenue();
int get_price(char seat_class);
int is_seat_taken(char seat_prefix, int seat_num);

int main() {
    int choice;
    begin = stream = NULL;
    do {
        printf("\n\n\t\t ******************************************************************************************");
        printf("\n\n\t\t          __|__                                                     __|__ \n");
        printf("                   --@--@--(_)--@--@--                                       --@--@--(_)--@--@-- ");
        printf("\n\t\t                                                                 ");
        printf("\n\t\t                 	Welcome to Fokir Marka Airlines  LTD                ");
        printf("\n\t\t ******************************************************************************************");
        printf("\n\n\n\t\t Please enter your choice from below:");
        printf("\n\n\t\t 1. Reservation");
        printf("\n\n\t\t 2. Cancel");
        printf("\n\n\t\t 3. Display records");
        printf("\n\n\t\t 4. Show available seats");
        printf("\n\n\t\t 5. Check seat status");
        printf("\n\n\t\t 6. Display total revenue");
        printf("\n\n\t\t 7. EXIT");
        printf("\n\n\t\t Enter your choice :");

        scanf("%d", &choice); fflush(stdin);
        system("cls");
        switch (choice) {
            case 1:
                reserve();
                break;
            case 2:
                cancel();
                break;
            case 3:
                display();
                break;
            case 4:
                show_available_seats();
                break;
            case 5: {
                char seat_prefix;
                int seat_num;
                printf("\nEnter seat class (Z for First, J for Business, Y for Economy): ");
                scanf(" %c", &seat_prefix);
                printf("Enter seat number to check status (1-50): ");
                scanf("%d", &seat_num);
                fflush(stdin);
                check_seat_status(seat_prefix, seat_num);
                break;
            }
            case 6:
                display_revenue();
                break;
            case 7:
                printf("Thank you for considering Fokir Marka Airlines LTD.\n");
                exit(0);
                break;
            default:
                printf("\n\n\t SORRY INVALID CHOICE!");
                printf("\n\n\t PLEASE CHOOSE FROM 1-7");
        }
        getch();
        } while (choice != 7);
}

void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void details(struct airline *reservation) {
    printf("\n\t Enter your passport number: ");
    fgets(reservation->passport, sizeof(reservation->passport), stdin);
    remove_newline(reservation->passport);
    fflush(stdin);

    printf("\n\t Enter your name: ");
    fgets(reservation->name, sizeof(reservation->name), stdin);
    remove_newline(reservation->name);
    fflush(stdin);

    printf("\n\t Enter your email address: ");
    fgets(reservation->email, sizeof(reservation->email), stdin);
    remove_newline(reservation->email);
    fflush(stdin);

    printf("\n\t Enter the Destination: ");
    fgets(reservation->destination, sizeof(reservation->destination), stdin);
    remove_newline(reservation->destination);
    fflush(stdin);

    // Display seat class options
    printf("\n\t Seat Class Options:");
    printf("\n\t - Economy (Y): $60");
    printf("\n\t - Business (J): $150");
    printf("\n\t - First Class (Z): $300");

    // Prompt for seat class
    printf("\n\t Enter seat class (Z for First Class, J for Business, Y for Economy): ");
    scanf(" %c", &reservation->seat_class);
    reservation->seat_class = toupper(reservation->seat_class);  // Convert to uppercase
    reservation->price = get_price(reservation->seat_class);  // Set price based on class
    fflush(stdin);
}

int get_price(char seat_class) {
    switch (seat_class) {
        case 'Y': return 60;
        case 'J': return 150;
        case 'Z': return 300;
        default:
            printf("\nInvalid seat class. Setting default price to $60 (Economy).\n");
            return 60;
    }
}

int is_seat_taken(char seat_prefix, int seat_num) {
    switch (seat_prefix) {
        case 'Z':
            return seats_z[seat_num];
        case 'J':
            return seats_j[seat_num];
        case 'Y':
            return seats_y[seat_num];
        default:
            return 1;
    }
}

void reserve() {
    int num_seats, seat_num;
    char seat_prefix;

    printf("\nEnter the number of seats you would like to reserve: ");
    scanf("%d", &num_seats);
    fflush(stdin);

    for (int i = 0; i < num_seats; i++) {
        struct airline *new_reservation = (struct airline *)malloc(sizeof(struct airline));

        details(new_reservation);
        
        seat_prefix = new_reservation->seat_class;
        for (seat_num = 1; seat_num <= 50; seat_num++) {
            if (!is_seat_taken(seat_prefix, seat_num)) {
                new_reservation->seat_num = seat_num;
                break;
            }
        }

        if (seat_num > 50) {
            printf("\nNo available seats in %c class for reservation %d.\n", seat_prefix, i + 1);
            free(new_reservation);
            continue;
        }

        switch (seat_prefix) {
            case 'Z':
                seats_z[seat_num] = 1;
                break;
            case 'J':
                seats_j[seat_num] = 1;
                break;
            case 'Y':
                seats_y[seat_num] = 1;
                break;
        }

        new_reservation->following = NULL;
        if (begin == NULL) {
            begin = new_reservation;
        } else {
            stream = begin;
            while (stream->following) {
                stream = stream->following;
            }
            stream->following = new_reservation;
        }

        total_revenue += new_reservation->price;

        printf("\n\t Seat booking successful for reservation %d!", i + 1);
        printf("\n\t Seat: %c-%d", seat_prefix, new_reservation->seat_num);
        printf("\n\t Class: %c", new_reservation->seat_class);
        printf("\n\t Price: $%d\n", new_reservation->price);
    }
}

void show_available_seats() {
    printf("\nAvailable seats:");

    printf("\nFirst Class (Z):");
    for (int i = 1; i <= 50; i++) {
        if (!seats_z[i]) {
            printf(" Z-%d", i);
        }
    }

    printf("\nBusiness Class (J):");
    for (int i = 1; i <= 50; i++) {
        if (!seats_j[i]) {
            printf(" J-%d", i);
        }
    }

    printf("\nEconomy Class (Y):");
    for (int i = 1; i <= 50; i++) {
        if (!seats_y[i]) {
            printf(" Y-%d", i);
        }
    }
    printf("\n\n++=====================================================++\n");
}

void check_seat_status(char seat_prefix, int seat_num) {
    int taken;
    seat_prefix = toupper(seat_prefix); // Convert input to uppercase
    switch (seat_prefix) {
        case 'Z':
            taken = seats_z[seat_num];
            break;
        case 'J':
            taken = seats_j[seat_num];
            break;
        case 'Y':
            taken = seats_y[seat_num];
            break;
        default:
            printf("\nInvalid seat class!");
            return;
    }
    if (seat_num >= 1 && seat_num <= 50) {
        if (taken)
            printf("\nSeat %c-%d is already reserved.", seat_prefix, seat_num);
        else
            printf("\nSeat %c-%d is available.", seat_prefix, seat_num);
    } else {
        printf("\nInvalid seat number! Please enter a number between 1 and 50.");
    }
}

void cancel() {
    stream = begin;
    system("cls");
    char passport[6];
    printf("\n\n Enter passport number to delete record?: ");
    fgets(passport, sizeof(passport), stdin);
    remove_newline(passport);

    if (strcmp(begin->passport, passport) == 0) {
        dummy = begin;
        switch (dummy->seat_class) {
            case 'Z':
                seats_z[dummy->seat_num] = 0;
                break;
            case 'J':
                seats_j[dummy->seat_num] = 0;
                break;
            case 'Y':
                seats_y[dummy->seat_num] = 0;
                break;
        }
        begin = begin->following;
        total_revenue -= dummy->price;
        free(dummy);
        printf("Booking has been deleted");
        Sleep(800);
        return;
    }
    while (stream->following) {
        if (strcmp(stream->following->passport, passport) == 0) {
            dummy = stream->following;
            switch (dummy->seat_class) {
                case 'Z':
                    seats_z[dummy->seat_num] = 0;
                    break;
                case 'J':
                    seats_j[dummy->seat_num] = 0;
                    break;
                case 'Y':
                    seats_y[dummy->seat_num] = 0;
                    break;
            }
            stream->following = stream->following->following;
            total_revenue -= dummy->price;
            free(dummy);
            printf("Booking has been deleted");
            getch();
            Sleep(800);
            return;
        }
        stream = stream->following;
    }
    printf("Passport number is incorrect, please check your passport.");
}

void display() {
    if (begin == 0) {
        printf("\n\n\t No reservations yet.\n");
        return;
    }
    
    stream = begin;
    while (stream != 0) {
        printf("\n\n Passport Number : %-6s", stream->passport);
        printf("\n         Name : %-15s", stream->name);
        printf("\n      Email Address: %-15s", stream->email);
        printf("\n      Seat number: %c-%d", stream->seat_class, stream->seat_num);
        printf("\n     Destination: %-15s", stream->destination);
        printf("\n      Seat Class: %-10c", stream->seat_class);
        printf("\n           Price: $%d", stream->price);
        printf("\n\n++=====================================================++");
        stream = stream->following;
    }
}

void display_revenue() {
    printf("\nTotal revenue from all reservations: $%d\n", total_revenue);
}