#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int year;
    int month;
    int day;
    int ahh;
    int amm;
    int bhh;
    int bmm;
} datetime;

char departments[15][50] = {{"Computer Science"},
                            {"Biotechnology"},
                            {"Physics"},
                            {"Mathematics"},
                            {"Computer Engineering"},
                            {"Life Sciences"},
                            {"Pharmacy"},
                            {"Civil Engineering"},
                            {"Mechanical Engineering"}, 
                            {"Geomatics Engineering"},
                            {"Development Studies"},
                            {"Music"},
                            {"Arts and Design"},
                            {"Languages and Mass Communication"},
                            {"Electrical and Electronics Engineering"}};


void write_to_csv(FILE* file, datetime datetime, int department, char* event) {
    fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%s,%s\n", datetime.year, datetime.month, datetime.day, datetime.ahh, datetime.amm, datetime.bhh, datetime.bmm, event, departments[department]);
    printf("The hall was successfully booked!\n\n");
}

int check_booking(datetime datetime1) {
    datetime datetime2;
    FILE * stream_file = fopen("data.csv", "r");
    if(!stream_file) {
        printf("There was an error reading the file.\n");
        return 0;
    }
    if(datetime1.ahh < 6 || datetime1.bhh > 18) {
        printf("Can't book the hall at given time!\n\n");
        return 0;
    }
    char line[100];
    int row_count = 0;
    int field_count = 0;
    while(fgets(line, 100, stream_file)) {
        row_count++;
        if(row_count == 1) {
            continue;
        }
        char * tline = strdup(line);
        char * token;
        for((token = strtok(tline, ",")); token && *token; token = strtok(NULL, ",")) {
            if(field_count == 0)
                datetime2.year = atoi(token);
            else if(field_count == 1)
                datetime2.month = atoi(token);
            else if(field_count == 2)
                datetime2.day = atoi(token);
            else if(field_count == 3)
                datetime2.ahh = atoi(token);
            else if(field_count == 4)
                datetime2.amm = atoi(token);
            if(field_count == 5)    
                datetime2.bhh = atoi(token);
            else if(field_count == 6)
                datetime2.bmm = atoi(token);

            field_count++;
        }
        
        if(datetime2.year == datetime1.year) {
            if(datetime2.month == datetime1.month) {
                if(datetime2.day == datetime1.day) {
                    if(((datetime1.ahh >= datetime2.ahh || datetime1.ahh <= datetime2.ahh) && !(datetime1.ahh >= datetime2.bhh)) &&
                        ((datetime1.bhh >= datetime2.bhh || datetime1.bhh <= datetime2.bhh) && !(datetime1.bhh <= datetime2.ahh))) {
                            printf("Booking Failed! The hall is already booked at given time!\n");
                            return 0;
                    }
                }
            }
        }

        field_count = 0;
        free(tline);
    }
    fclose(stream_file);

    return 1;
}

int main() {
    int flag_loop = 1;
    datetime datetime;
    int command, i, department;
    char event[50];

    //for case 3
    int lno, ctr;
    char ch;
    FILE *fptr1, *fptr2;    //fptr1 for data.csv & fptr2 for temp.csv
    char fname[10];         //store "data.csv"
    char temp[10];          //store "temp.csv"
    printf("\e[1;1H\e[2J");
    printf("\n----------------------Kathmandu University CV Raman Auditorium----------------------\n\n");

    while(flag_loop) {
        printf("---List of commands---\n");
        printf("1) Book\n");
        printf("2) Show Records\n");
        printf("3) Delete Bookings\n");
        printf("4) Exit\n\n");

        printf("Command: ");
        scanf("%d", &command);
        getchar();
        printf("\n");

        switch (command) {
        case 1:
            printf("\e[1;1H\e[2J");         //system("cls"); | clears the screen
            printf("\n----------------------Kathmandu University CV Raman Auditorium----------------------\n\n");
            printf("----List Of Departments----\n");
            for(i = 0; i < 15; i++) {
                printf("%d) %s\n", i+1, departments[i]);
            }
            printf("---------------------------\n\n");
            printf("Enter a number corresponding to your department: ");
            scanf("%d", &department);       
            getchar();
            department--;

            printf("Enter the event name: ");
            fgets(event, 50, stdin);
            event[strcspn(event, "\n")] = 0;        //removes line terminator from the event string
            
            printf("Enter a date(DD/MM/YYYY): ");
            scanf("%d/%d/%d", &datetime.day, &datetime.month, &datetime.year);
            printf("From: ");
            scanf("%d:%d", &datetime.ahh, &datetime.amm);
            printf("To: ");
            scanf("%d:%d", &datetime.bhh, &datetime.bmm);
            getchar();

            char sure;
            printf("Are you sure you want to book the hall on %d/%d/%d from %d:%d to %d:%d?\n(Y/N): ", datetime.day, datetime.month, datetime.year, datetime.ahh, datetime.amm, datetime.bhh, datetime.bmm);
            scanf("%c", &sure);
            if(sure == 'Y') {
                printf("\e[1;1H\e[2J");
                if(check_booking(datetime)) {
                    FILE * csv_file = fopen("data.csv", "a");
                    if(!csv_file) {
                        printf("There was an error reading the file.");
                        break;
                    }
                    write_to_csv(csv_file, datetime, department, event);
                    fclose(csv_file);
                }
            }
            break;

        case 2:
            printf("\e[1;1H\e[2J");
            printf("\n----------------------Kathmandu University CV Raman Auditorium----------------------\n\n");
            printf("----------------------------------List Of Bookings----------------------------------\n\n");
            printf("SN   Date\t  From\t  To\t  Event\t\t\t    Department\n\n");
            FILE * print_file = fopen("data.csv", "r");
            if(!print_file) {
                    printf("There was an error reading the file.");
                    break;
            }
            char line[100];
            int row_count = 0;
            int field_count = 0;
            while(fgets(line, 100, print_file)) {
                row_count++;
                if(row_count == 1) {
                    continue;
                }
                char * tline = strdup(line);
                char * token;
                printf("%-3d", row_count-1);
                for((token = strtok(tline, ",")); token && *token; token = strtok(NULL, ",")) {
                    if(field_count == 0)
                        printf("  %*s", 4, token);    //year
                    else if(field_count == 1)
                        (atoi(token) < 10) ? printf("/0%d", atoi(token)): printf("/%s", token);   //month
                    else if(field_count == 2)
                        (atoi(token) < 10) ? printf("/0%d", atoi(token)): printf("/%s", token); //day
                    else if(field_count == 3)
                        // printf("\t  %s", token);    //ahh
                        (atoi(token) < 10) ? printf("\t  0%d", atoi(token)): printf("\t  %s", token);
                    else if(field_count == 4)
                        (atoi(token) < 10) ? printf(":0%d", atoi(token)): printf(":%s", token); //amm
                    else if(field_count == 5)    
                        // printf("\t  %s", token);
                        (atoi(token) < 10) ? printf("\t  0%d", atoi(token)): printf("\t  %s", token);    //bhh
                    else if(field_count == 6)
                        (atoi(token) < 10) ? printf(":0%d", atoi(token)): printf(":%s", token); //bmm
                    else if(field_count == 7)
                        printf("\t  %-20s", token);  //event
                    else if(field_count == 8)
                        printf("\t    %s", token);    //department

                    field_count++;
                }
                field_count = 0;
                printf("\n");
                free(tline);
            }
            printf("------------------------------------------------------------------------------------\n\n");
            fclose(print_file);
            break;
        
        case 3:
            ctr = 0;
            strcpy(fname, "data.csv");
            strcpy(temp, "temp.csv");

            fptr1 = fopen(fname, "r");
            if(!fptr1) {
                printf("There was an error reading the file!");
                break;
            }
            fptr2 = fopen(temp, "w");
            if(!fptr2) {
                printf("Unable To Open A Temporary File");
                fclose(fptr1);
                break;
            }
            
            printf("Enter a number you want to remove record from: ");
            scanf("%d", &lno);
            lno++;

            char str[100];
            while(!feof(fptr1)) {
                fgets(str, 100, fptr1);
                if(!feof(fptr1)) {
                    ctr++;
                    if(ctr != lno) {
                        fprintf(fptr2, "%s", str);
                    }
                }
            }
            fclose(fptr1);
            fclose(fptr2);
            remove(fname);
            rename(temp, fname);

            printf("\e[1;1H\e[2J");
            printf("Deleted record no. %d.\n", lno-1);
            break;
        
        case 4:
            printf("\e[1;1H\e[2J");
            flag_loop = 0;
            break;

        default:
            printf("\e[1;1H\e[2J");
            printf("Command Error!\n");
            break;
        }
        
    }

    return 0;
}