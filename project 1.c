#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SEATS 50
#define MAX_RESERVATIONS 150

struct airline {
    char passport[50];
    char name[50];
    char destination[50];
    int seat_num;
    char email[50];
    char seat_class;
    int price;
};

struct airline reservations[MAX_RESERVATIONS];
int reservation_count = 0;
int total_revenue = 0;

int seats_z[MAX_SEATS + 1] = {0};
int seats_j[MAX_SEATS + 1] = {0};
int seats_y[MAX_SEATS + 1] = {0};

void details(struct airline *reservation);
void reserve();
void cancel();
void display();
void show_available_seats();
void check_seat_status(char seat_prefix, int seat_num);
void display_revenue();
int get_price(char seat_class);
int is_seat_taken(char seat_prefix, int seat_num);
void remove_newline(char *str);

int main() {
    int choice;
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

        scanf("%d", &choice);
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
    

    printf("\n\t Enter your name: ");
    fgets(reservation->name, sizeof(reservation->name), stdin);
    remove_newline(reservation->name);
    

    char email_prefix[50];
    printf("\n\t Enter your email address (without @gmail.com): ");
    fgets(email_prefix, sizeof(email_prefix), stdin);
    remove_newline(email_prefix);
    snprintf(reservation->email, sizeof(reservation->email), "%s@gmail.com", email_prefix);
    

    printf("\n\t Enter the Destination: ");
    fgets(reservation->destination, sizeof(reservation->destination), stdin);
    remove_newline(reservation->destination);
    

    printf("\n\t Seat Class Options:");
    printf("\n\t - Economy (Y): $60");
    printf("\n\t - Business (J): $150");
    printf("\n\t - First Class (Z): $300");

    printf("\n\t Enter seat class (Z for First Class, J for Business, Y for Economy): ");
    scanf(" %c", &reservation->seat_class);
    reservation->seat_class = toupper(reservation->seat_class);
    reservation->price = get_price(reservation->seat_class);
   
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
        if (reservation_count >= MAX_RESERVATIONS) {
            printf("\nCannot reserve more seats; reservation list is full.\n");
            return;
        }

        details(&reservations[reservation_count]);
        
        seat_prefix = reservations[reservation_count].seat_class;
        for (seat_num = 1; seat_num <= MAX_SEATS; seat_num++) {
            if (!is_seat_taken(seat_prefix, seat_num)) {
                reservations[reservation_count].seat_num = seat_num;
                break;
            }
        }

        if (seat_num > MAX_SEATS) {
            printf("\nNo available seats in %c class for reservation %d.\n", seat_prefix, i + 1);
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

        total_revenue += reservations[reservation_count].price;

        printf("\n\t Seat booking successful for reservation %d!", i + 1);
        printf("\n\t Seat: %c-%d", seat_prefix, reservations[reservation_count].seat_num);
        printf("\n\t Class: %c", reservations[reservation_count].seat_class);
        printf("\n\t Price: $%d\n", reservations[reservation_count].price);

        reservation_count++;
        fflush(stdin);
    }
}

void show_available_seats() {
    printf("\nAvailable seats:");

    printf("\nFirst Class (Z):");
    for (int i = 1; i <= MAX_SEATS; i++) {
        if (!seats_z[i]) {
            printf(" Z-%d", i);
        }
    }

    printf("\nBusiness Class (J):");
    for (int i = 1; i <= MAX_SEATS; i++) {
        if (!seats_j[i]) {
            printf(" J-%d", i);
        }
    }

    printf("\nEconomy Class (Y):");
    for (int i = 1; i <= MAX_SEATS; i++) {
        if (!seats_y[i]) {
            printf(" Y-%d", i);
        }
    }
    printf("\n\n++=====================================================++\n");
}

void check_seat_status(char seat_prefix, int seat_num) {
    int taken;
    seat_prefix = toupper(seat_prefix);
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
    if (seat_num >= 1 && seat_num <= MAX_SEATS) {
        if (taken)
            printf("\nSeat %c-%d is already reserved.", seat_prefix, seat_num);
        else
            printf("\nSeat %c-%d is available.", seat_prefix, seat_num);
    } else {
        printf("\nInvalid seat number! Please enter a number between 1 and %d.", MAX_SEATS);
    }
}

void cancel() {
    char passport[50];
    printf("\n\n Enter passport number to delete record?: ");
    fflush(stdin);
    fgets(passport, sizeof(passport), stdin);
    

    for (int i = 0; i < reservation_count; i++) {
        if (strcmp(reservations[i].passport, passport) == 0) {
            // Seat found, proceed to cancel the booking
            switch (reservations[i].seat_class) {
                case 'Z':
                    seats_z[reservations[i].seat_num] = 0;
                    break;
                case 'J':
                    seats_j[reservations[i].seat_num] = 0;
                    break;
                case 'Y':
                    seats_y[reservations[i].seat_num] = 0;
                    break;
            }
            total_revenue -= reservations[i].price;

            // Shift the remaining reservations to fill the gap
            for (int j = i; j < reservation_count - 1; j++) {
                reservations[j] = reservations[j + 1];
            }
            reservation_count--;
            printf("Booking has been deleted.\n");
            return;
        }
    }
    printf("Passport number is incorrect, please check your passport.\n");
}


void display() {
    if (reservation_count == 0) {
        printf("\n\n\t No reservations yet.\n");
        return;
    }
    
    for (int i = 0; i < reservation_count; i++) {
        printf("\n   Passport Number : %s", reservations[i].passport);
        printf("\n   Name :            %s", reservations[i].name);
        printf("\n   Email Address:    %s", reservations[i].email);
        printf("\n   Seat number:      %c-%d", reservations[i].seat_class, reservations[i].seat_num);
        printf("\n   Destination:      %s", reservations[i].destination);
        printf("\n   Seat Class:       %c", reservations[i].seat_class);
        printf("\n   Price:           $%d", reservations[i].price);
        printf("\n\n++=====================================================++");
    }
}

void display_revenue() {
    printf("\nTotal revenue from all reservations: $%d\n", total_revenue);
}
