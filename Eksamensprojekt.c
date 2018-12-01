/*First the needed libraries har included*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

/*This integer is initialized for error control*/
extern int errno;

/*Then some generel values are defined*/
#define MAX_RACE_NAME_LENGTH 20
#define MAX_CYCLIST_NAME_LENGTH 30
#define ABBREVIATIONS_LENGTH 4
#define NUMBER_OF_SYMPOLS_IN_TIME 8
#define MAX_DIGITS_OF_PLACEMENT 4
#define TOTAL_NUMBER_OF_PARTICIPENTS 1000
#define TOTAL_NUMBER_OF_RACES 10
#define OTL -1
#define DNF -2
#define HOUR_IN_SECONDS 3600
#define MINUTE_IN_SECONDS 60
#define INFINITY 999999

/*Then structs for holder the information for a race and a cyclist are defined*/
struct race{
    char name[MAX_RACE_NAME_LENGTH];
    int roster_size;
};typedef struct race race;

struct cyclist{
    char race_name[MAX_RACE_NAME_LENGTH];
    char cyclist_name[MAX_CYCLIST_NAME_LENGTH];
    char last_name[MAX_CYCLIST_NAME_LENGTH];
    int age;
    char team[ABBREVIATIONS_LENGTH];
    char nationality[ABBREVIATIONS_LENGTH];
    int placement;
    int time[3];
    int points;
}; typedef struct cyclist cyclist;

/*I have chosen to create a struct containing only name and number of races for
danes, so I would not have to assign a value of number of races to every cyclist*/
struct dane_who_finished_a_race{
    char cyclist_name[MAX_CYCLIST_NAME_LENGTH];
    int number_of_finished_races;
}; typedef struct dane_who_finished_a_race dane_who_finished_a_race;

/*Here I create prototypes for all my functions*/
void* read_file_to_list(int* list_length, race* races, int* number_of_races);
void print_assignment_number(int assignment_number);
void count_up_race_rosters(const int cyclist_number, const cyclist* output_pointer_to_array_of_cyclists, race* races, int* number_of_races);
void qsort(void *array, size_t lenght, size_t size, int (*compar)(const void *, const void*));
void print_italian_cyclists_above_thirty(const cyclist* list, const int list_length);
void print_list_of_cyclists(const cyclist* list, const int list_length, const int print_points);
dane_who_finished_a_race* danish_cyclists_who_finished_a_race(cyclist* list, const int full_list_length, int* list_length);
void print_danes(dane_who_finished_a_race* list_of_danish_cyclists_who_finished_a_race, int list_length_of_danes_who_finished);
void add_danes_to_array(dane_who_finished_a_race* list_of_danish_cyclists_who_finished_a_race, cyclist* list, int current_position_in_list_of_cyclists, int* current_length_of_danes_list);
void* create_and_sort_leaderboard(cyclist* list, const int full_list_length, int* list_length, const race* races);
void assign_points_to_cyclists_for_every_race(cyclist* list, const int full_list_length, const race* races);
void get_last_name(cyclist* cyclist);
int compare_points(const void* a, const void* b);
void find_fastes_cyclist_of_paris_and_amstel(cyclist* full_list, const int* full_list_length, cyclist* fastes_cyclist, int* time_for_fastes_cyclist);
void make_list_of_cyclists_paris_amstel(const cyclist* full_list, cyclist* paris_cyclists, cyclist* amstel_cyclists, const int* full_list_length, int* paris_list_length, int* amstel_list_length);
void make_array_with_positions_of_cyclists_in_the_two_arrays(const int paris_list_length, const int amstel_list_length, const cyclist* paris_cyclists, const cyclist* amstel_cyclists, int list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][3], int* number_of_cyclists_in_both_races);
int get_total_time(int paris_time[3], int amstel_time[3]);
int find_shortest_time(int list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][3], int list_length);
int biggest_of(const int a, const int b);
int smallest_of(const int a, const int b);
void print_total_time(const cyclist* cyclist, const int i);
double average_age_for_top_ten(cyclist* list, const int list_length, const int number_of_races);
int compare_race_name(const void* a, const void* b);
cyclist* remove_doublets(cyclist* list, int* list_length);
void calculate_average_age(cyclist* list, double* average_age, int list_length);

int main(int argc, char const *argv[]){

    int scan_result = 0, user_input = 0, list_length_of_all_cyclists, list_length_of_danes_who_finished, list_length_of_leaderboard, number_of_races = 0, total_time_for_fastes_cyclist;
    cyclist *list_of_cyclists, *leaderboard_of_cyclists_with_total_points, *fastes_cyclist;
    dane_who_finished_a_race* list_of_danish_cyclists_who_finished_a_race;
    race* races;
    races = (race *)malloc(TOTAL_NUMBER_OF_RACES * sizeof(race));

    if(argc > 1){
        if(strcmp(argv[1], "--print") == 0){
            printf("Results should be printed here\n");
        }else{
            printf("Incorrect argument for the program\n");
        }
    }else{

        /*The while loop makes sure the program does not just exit, if the input is wrong*/
        while(scan_result < 1){
            printf("Welcome to the program for calculating different aspects of:\nParis Roubaix, Amstel Gold Race, La La Fleche Wallonne, and Liege Bastogne Liege.\nYou can decide which one of the following results you want to see, by typing the corresponding number to the terminal.\n");
            printf("\"1\": A list of every Italian cyclist above 30\n");
            printf("\"2\": A list of every Danish cyclist, who has completed a race, and the number of races the cyclist has completed\n");
            printf("\"3\": The top 10 of most points overall\n");
            printf("\"4\": The fastes cyclist to complete both Paris Roubaix and Amstel Gold Race and the total time for both races\n");
            printf("\"5\": The average age for the top 10 cyclists in all the races\n");
            scan_result = scanf("%d", &user_input);
            printf("\n\n\n");
            if(scan_result < 1 || user_input > 5  || user_input < 1){
                printf("This is an invalid input, please type another one from the list (a number between 1-5)\n");
                scan_result = 0;
                printf("\n\n");
            }
        }

        /*I read the file to an array of structs using the read_file_to_list function,
        which returns a pointer to the list position*/
        list_of_cyclists = read_file_to_list(&list_length_of_all_cyclists, races, &number_of_races);
        print_assignment_number(user_input);
        switch(user_input){
            case 1:
                /*The function for finding and printing the italians above thirty is the called*/
                print_italian_cyclists_above_thirty(list_of_cyclists, list_length_of_all_cyclists);
            break;

            case 2:
                /*Then the list of danes who have finished at least one race is found*/
                list_of_danish_cyclists_who_finished_a_race = danish_cyclists_who_finished_a_race(list_of_cyclists, list_length_of_all_cyclists, &list_length_of_danes_who_finished);

                /*Here the danes are then printed*/
                print_danes(list_of_danish_cyclists_who_finished_a_race, list_length_of_danes_who_finished);

                /*Every dynamic allocated memory is then freed*/
                free(list_of_danish_cyclists_who_finished_a_race);
            break;

            case 3:
                /*The leaderboard is made using the create_and_sort_leaderboard function*/
                leaderboard_of_cyclists_with_total_points = create_and_sort_leaderboard(list_of_cyclists, list_length_of_all_cyclists, &list_length_of_leaderboard, races);

                /*The top ten is then printed*/
                print_list_of_cyclists(leaderboard_of_cyclists_with_total_points, 10, 1);

                /*Every dynamic allocated memory is then freed*/
                free(leaderboard_of_cyclists_with_total_points);
            break;

            case 4:
                /*The fastes_cyclist pointer is set to the same list, as it will need a value to be a parameter in the following function*/
                fastes_cyclist = list_of_cyclists;
                find_fastes_cyclist_of_paris_and_amstel(list_of_cyclists, &list_length_of_all_cyclists, fastes_cyclist, &total_time_for_fastes_cyclist);

                /*fastes_cyclist is the printed out with the total time*/
                print_total_time(fastes_cyclist, total_time_for_fastes_cyclist);
            break;

            case 5:
                /*The average age for the top ten cyclists is then printed and calculated*/
                printf("The average age for cyclists in the top 10 of any race is: %lf", average_age_for_top_ten(list_of_cyclists, list_length_of_all_cyclists, number_of_races));
            break;

            default:
                printf("Something went wrong...\n");
            break;
        }
    }
    /*Every dynamic allocated memory is then freed*/
    free(list_of_cyclists);
    free(races);

    /*Main returns 0 and the program stops*/
    return 0;
}

/*The following is all used functions in order of usage*/
void* read_file_to_list(int* list_length, race* races, int* number_of_races){
    int current_position_in_array = 0, scan_result;
    char temporary_placeholder_placement[4];
    FILE* read_file_pointer = fopen("cykelloeb","r");

    /*To make sure the file has been open, the pointer is checked. If it is not NULL, the function goes on*/
    if (read_file_pointer == NULL) {

        /*This prints the error code and exits the program*/
        fprintf(stderr, "Error opening file: %s\n", strerror( errno ));
        exit(-1);

    }else{

        cyclist* output_pointer_to_array_of_cyclists;
        output_pointer_to_array_of_cyclists = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));

        /*The while loop runs through the entire document and loads very line to a cyclist strcut*/
        while(!feof(read_file_pointer)){
            scan_result = fscanf(read_file_pointer, " %[a-zA-Z] \"%[a-zA-Z-' ]\" | %d %[A-Z] %[A-Z] | %3[A-Z0-9] %d : %d : %d",
                                                    output_pointer_to_array_of_cyclists[current_position_in_array].race_name,
                                                    output_pointer_to_array_of_cyclists[current_position_in_array].cyclist_name,
                                                    &output_pointer_to_array_of_cyclists[current_position_in_array].age,
                                                    output_pointer_to_array_of_cyclists[current_position_in_array].team,
                                                    output_pointer_to_array_of_cyclists[current_position_in_array].nationality,
                                                    temporary_placeholder_placement,
                                                    &output_pointer_to_array_of_cyclists[current_position_in_array].time[0],
                                                    &output_pointer_to_array_of_cyclists[current_position_in_array].time[1],
                                                    &output_pointer_to_array_of_cyclists[current_position_in_array].time[2]);
            /*The scan is check to ensure that it was done correctly, else the program outputs an error message*/
            if(scan_result < 9){
                /*Insert ERROR code*/
            }
            /*The following changes the placement string to an integer*/
            if(strcmp(temporary_placeholder_placement,"OTL") == 0){
                output_pointer_to_array_of_cyclists[current_position_in_array].placement = OTL;
            }else if(strcmp(temporary_placeholder_placement,"DNF") == 0){
                output_pointer_to_array_of_cyclists[current_position_in_array].placement = DNF;
            }else{
                output_pointer_to_array_of_cyclists[current_position_in_array].placement = atoi(temporary_placeholder_placement);
            }
            output_pointer_to_array_of_cyclists[current_position_in_array].points = 0;

            /*Then the lastname of the cyclist is found for every cyclists*/
            get_last_name(output_pointer_to_array_of_cyclists);

            /*Every race from the file is then added to a seperate struct with the roster_size for each race.
            Every racer is put into the function seperately, and the race_name is checked with the races already added*/
            count_up_race_rosters(current_position_in_array, output_pointer_to_array_of_cyclists, races, number_of_races);

            current_position_in_array++;
        }
        current_position_in_array--;

        /*The length of the list is return using a pointer*/
        *list_length = current_position_in_array;

        /*The file gets closed*/
        fclose(read_file_pointer);

        /*A pointer to the list is returned*/
        return output_pointer_to_array_of_cyclists;
    }
}
void print_assignment_number(int assignment_number){
    printf("-----Answers for assignment %d-----\n\n", assignment_number);
}
void count_up_race_rosters(const int cyclist_number, const cyclist* output_pointer_to_array_of_cyclists, race* races, int* number_of_races){
    int number_of_current_race = *number_of_races, current_position_in_race_array = 0, found_match = 0, end_of_array = 0;
    /*The race_name from the cyclist is checked with every race already added to the race list.
    If it is already in the list, its roster_size is increased by 1, else it is added*/
    while(current_position_in_race_array <= number_of_current_race && found_match == 0 && end_of_array == 0){
        if((current_position_in_race_array == number_of_current_race && strcmp(output_pointer_to_array_of_cyclists[cyclist_number].race_name, races[current_position_in_race_array].name) != 0)){
            strcpy(races[current_position_in_race_array].name, output_pointer_to_array_of_cyclists[cyclist_number].race_name);
            races[current_position_in_race_array].roster_size = 1;
            number_of_current_race++;
            end_of_array = 1;
        }else if(strcmp(output_pointer_to_array_of_cyclists[cyclist_number].race_name, races[current_position_in_race_array].name) == 0){
            races[current_position_in_race_array].roster_size++;
            found_match = 1;
        }else{
            current_position_in_race_array++;
        }
    }
    /*The number of races is updated*/
    *number_of_races = number_of_current_race;
}
void print_italian_cyclists_above_thirty(const cyclist* list, const int list_length){
    int current_position_in_list = 0, current_position_in_italian_array = 0;
    cyclist list_of_italian_cyclists_above_thirty[TOTAL_NUMBER_OF_PARTICIPENTS];

    /*The list of cyclists is run through for italians above 30 years of age, who then gets put into an array*/
    while(current_position_in_list < list_length){
        if((strcmp(list[current_position_in_list].nationality,"ITA") == 0) && list[current_position_in_list].age > 30){
            list_of_italian_cyclists_above_thirty[current_position_in_italian_array] = list[current_position_in_list];
            current_position_in_italian_array++;
        }
        current_position_in_list++;
    }

    /*The array is then printet without points*/
    print_list_of_cyclists(list_of_italian_cyclists_above_thirty, current_position_in_italian_array, 0);
}
void print_list_of_cyclists(const cyclist* list, const int list_length, const int print_points){
    int current_position_in_list;
    char placement_as_string[4];

    for(current_position_in_list = 0; current_position_in_list < list_length; current_position_in_list++){

        /*First the placement is converted to a string, so that OTL and DNF will not be printed as -1 and -2*/
        if(list[current_position_in_list].placement > -1){
            sprintf(placement_as_string, "%d", list[current_position_in_list].placement);
        }else if(list[current_position_in_list].placement == OTL){
            strncpy(placement_as_string, "OTL", 3);
        }else{
            strncpy(placement_as_string, "DNF", 3);
        }

        /*Then every cyclist is printed out*/
        printf("%-28s %-30s | %d %s %s | %-10s   %2d:%2d:%2d", list[current_position_in_list].race_name,
                                             list[current_position_in_list].cyclist_name,
                                             list[current_position_in_list].age,
                                             list[current_position_in_list].team,
                                             list[current_position_in_list].nationality,
                                             placement_as_string,
                                             list[current_position_in_list].time[0],
                                             list[current_position_in_list].time[1],
                                             list[current_position_in_list].time[2]);

        /*If the print_points input is 1, then the points are printed, else go to next line*/
        if(print_points == 1){
            printf("     Points: %d\n", list[current_position_in_list].points);
        }else{
            printf("\n");
        }
    }

    /*Then 3 new lines are added to create space and make it easier to differentiate the outputs*/
    printf("\n\n\n");
}
dane_who_finished_a_race* danish_cyclists_who_finished_a_race(cyclist* list, const int full_list_length, int* list_length){
    int current_position_in_list_of_cyclists = 0, current_length_of_danes_list = 0;
    dane_who_finished_a_race *list_of_danish_cyclists_who_finished_a_race;
    list_of_danish_cyclists_who_finished_a_race = (dane_who_finished_a_race *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(dane_who_finished_a_race));

    /*The list of cyclist is run through and every dane, who has achieved a placement above DNF, is added to a new array*/
    while(current_position_in_list_of_cyclists < full_list_length){
        if((strcmp(list[current_position_in_list_of_cyclists].nationality,"DEN") == 0) && (list[current_position_in_list_of_cyclists].placement > DNF)){
            add_danes_to_array(list_of_danish_cyclists_who_finished_a_race, list, current_position_in_list_of_cyclists, &current_length_of_danes_list);
        }
        current_position_in_list_of_cyclists++;
    }

    *list_length = current_length_of_danes_list;
    return list_of_danish_cyclists_who_finished_a_race;
}
void add_danes_to_array(dane_who_finished_a_race* list_of_danish_cyclists_who_finished_a_race, cyclist* list, int current_position_in_list_of_cyclists, int* current_length_of_danes_list){
    int current_position_in_list_of_danes = 0, found_match = 0, end_of_array = 0;

    /*Every dane with a better placement than DNF is either added to the array or, if the cyclist is already in the array, the number of races finished is increased by 1*/
    while(current_position_in_list_of_danes <= *current_length_of_danes_list && found_match == 0 && end_of_array == 0){
        if(strcmp(list[current_position_in_list_of_cyclists].cyclist_name, list_of_danish_cyclists_who_finished_a_race[current_position_in_list_of_danes].cyclist_name) == 0){
            list_of_danish_cyclists_who_finished_a_race[current_position_in_list_of_danes].number_of_finished_races++;
            found_match = 1;
        }else if((strcmp(list[current_position_in_list_of_cyclists].cyclist_name, list_of_danish_cyclists_who_finished_a_race[current_position_in_list_of_danes].cyclist_name) != 0 && current_position_in_list_of_danes == *current_length_of_danes_list) || *current_length_of_danes_list == 0){
            strncpy(list_of_danish_cyclists_who_finished_a_race[*current_length_of_danes_list].cyclist_name, list[current_position_in_list_of_cyclists].cyclist_name, sizeof(list[current_position_in_list_of_cyclists].cyclist_name));
            list_of_danish_cyclists_who_finished_a_race[*current_length_of_danes_list].number_of_finished_races = 1;
            *current_length_of_danes_list += 1;
            end_of_array = 1;
        }else{
            current_position_in_list_of_danes++;
        }
    }
}
void print_danes(dane_who_finished_a_race* list_of_danish_cyclists_who_finished_a_race, int list_length_of_danes_who_finished){
    int current_position_in_array;
    for(current_position_in_array = 0; current_position_in_array < list_length_of_danes_who_finished; current_position_in_array++){
        printf("%s %d\n", list_of_danish_cyclists_who_finished_a_race[current_position_in_array].cyclist_name,
                          list_of_danish_cyclists_who_finished_a_race[current_position_in_array].number_of_finished_races);
    }
    printf("\n\n\n");
}
void* create_and_sort_leaderboard(cyclist* list, const int full_list_length, int* list_length, const race* races){
    int current_position_in_list_of_cyclists = 0, current_length_of_leaderboard = 0, current_position_in_leaderboard = 0, found_match = 0, end_of_array = 0;

    cyclist* output_pointer_with_leaderboard;
    output_pointer_with_leaderboard = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));

    /*First every cyclist is given the points for every race*/
    assign_points_to_cyclists_for_every_race(list, full_list_length, races);

    /*Then the list is run through to get rid of doublets and accumulate the points for every single cyclist*/
    while(current_position_in_list_of_cyclists < full_list_length){
        while(current_position_in_leaderboard <= current_length_of_leaderboard && found_match == 0 && end_of_array == 0){
            if(strcmp(list[current_position_in_list_of_cyclists].cyclist_name, output_pointer_with_leaderboard[current_position_in_leaderboard].cyclist_name) == 0){
                output_pointer_with_leaderboard[current_position_in_leaderboard].points += list[current_position_in_list_of_cyclists].points;
                found_match = 1;
            }else if(current_length_of_leaderboard == 0 || (strcmp(list[current_position_in_list_of_cyclists].cyclist_name, output_pointer_with_leaderboard[current_position_in_leaderboard].cyclist_name) != 0 && current_position_in_leaderboard == current_length_of_leaderboard)){
                output_pointer_with_leaderboard[current_length_of_leaderboard] = list[current_position_in_list_of_cyclists];
                current_length_of_leaderboard++;
                end_of_array = 1;
            }else{
                current_position_in_leaderboard++;
            }
        }
        current_position_in_list_of_cyclists++;
        current_position_in_leaderboard = 0;
        found_match = 0;
        end_of_array = 0;
    }
    *list_length = current_length_of_leaderboard;

    qsort(output_pointer_with_leaderboard, current_length_of_leaderboard, sizeof(struct cyclist), compare_points);

    /*print_list_of_cyclists(output_pointer_with_leaderboard, current_length_of_leaderboard);*/

    return output_pointer_with_leaderboard;
}
void assign_points_to_cyclists_for_every_race(cyclist* list, const int full_list_length, const race* races){
    int i = 0, j = 0;
    while(i < full_list_length){
        if(list[i].placement > -1){
            list[i].points += 3;
            while(strcmp(list[i].race_name, races[j].name) != 0){
                j++;
            }
            list[i].points += (races[j].roster_size - list[i].placement)/13;
            if(list[i].placement == 1){
                list[i].points += 10;
            }else if(list[i].placement == 2){
                list[i].points += 5;
            }else if(list[i].placement == 3){
                list[i].points += 2;
            }
        }else if(list[i].placement == -1){
            list[i].points += 1;
        }
        i++;
    }
}
int compare_points(const void* a, const void* b){
    cyclist *pointer_one = (cyclist *) a;
    cyclist *pointer_two = (cyclist *) b;
    if(pointer_one->points < pointer_two->points){
        return 1;
    }else if(pointer_one->points > pointer_two->points){
        return -1;
    }else{
        if(pointer_one->last_name < pointer_two->last_name){
            return 1;
        }else if(pointer_one->last_name > pointer_two->last_name){
            return -1;
        }else{
            return 0;
        }
    }
}
void get_last_name(cyclist* cyclist){
    int i, exit = 0, full_name_length;
    full_name_length = strlen(cyclist->cyclist_name);
    for(i = full_name_length; i > 0 && exit == 0; i--){
        if(i == islower(*(cyclist->cyclist_name + i))){
            i+=2;
            strncpy(cyclist->last_name, (cyclist->cyclist_name) + i, full_name_length-i);
            exit = 1;
        }
    }
}
void find_fastes_cyclist_of_paris_and_amstel(cyclist* full_list, const int* full_list_length, cyclist* fastes_cyclist, int* time_for_fastes_cyclist){
    int i, paris_list_length = 0, amstel_list_length = 0, list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][3], number_of_cyclists_in_both_races = 0;
    cyclist *paris_cyclists, *amstel_cyclists;

    paris_cyclists = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));
    amstel_cyclists = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));

    make_list_of_cyclists_paris_amstel(full_list, paris_cyclists, amstel_cyclists, full_list_length, &paris_list_length, &amstel_list_length);
    make_array_with_positions_of_cyclists_in_the_two_arrays(paris_list_length, amstel_list_length, paris_cyclists, amstel_cyclists, list_placements, &number_of_cyclists_in_both_races);

    for(i = 0; i < number_of_cyclists_in_both_races; i++){
        list_placements[i][2] = get_total_time(paris_cyclists[list_placements[i][0]].time, amstel_cyclists[list_placements[i][1]].time);
    }

    i = find_shortest_time(list_placements, number_of_cyclists_in_both_races);

    fastes_cyclist = &full_list[i];
    *time_for_fastes_cyclist = list_placements[i][2];

    free(paris_cyclists);
    free(amstel_cyclists);
}
void make_list_of_cyclists_paris_amstel(const cyclist* full_list, cyclist* paris_cyclists, cyclist* amstel_cyclists, const int* full_list_length, int* paris_list_length, int* amstel_list_length){
    int i;
    for(i = 0; i < *full_list_length; i++){
        if(strcmp(full_list[i].race_name, "ParisRoubaix") == 0){
            paris_cyclists[*paris_list_length] = full_list[i];
            *paris_list_length += 1;
        }else if(strcmp(full_list[i].race_name, "AmstelGoldRace") == 0){
            amstel_cyclists[*amstel_list_length] = full_list[i];
            *amstel_list_length += 1;
        }
    }
}
void make_array_with_positions_of_cyclists_in_the_two_arrays(const int paris_list_length, const int amstel_list_length, const cyclist* paris_cyclists, const cyclist* amstel_cyclists, int list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][3], int* number_of_cyclists_in_both_races){
    int i, j, k = 0;
    for(i = 0; i < paris_list_length; i++){
        for(j = 0; j < amstel_list_length; j++){
            if(strcmp(paris_cyclists[i].cyclist_name, amstel_cyclists[j].cyclist_name) == 0 &&
            (paris_cyclists[i].time[0] + paris_cyclists[i].time[1] + paris_cyclists[i].time[2]) > 0 &&
            (amstel_cyclists[j].time[0] + amstel_cyclists[j].time[1] + amstel_cyclists[j].time[2]) > 0){
                list_placements[k][0] = i;
                list_placements[k][1] = j;
                k++;
            }
        }
    }
    *number_of_cyclists_in_both_races = k;
}
int get_total_time(int paris_time[3], int amstel_time[3]){
    return (paris_time[0]*HOUR_IN_SECONDS + paris_time[1]*MINUTE_IN_SECONDS + paris_time[2]) + (amstel_time[0]*HOUR_IN_SECONDS + amstel_time[1]*MINUTE_IN_SECONDS + amstel_time[2]);
}
int find_shortest_time(int list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][3], int list_length){
    int i, j = INFINITY, k;
    for(i = 0; i < list_length; i++){
        j = smallest_of(j, list_placements[i][2]);
        if(j == list_placements[i][2]){
            k = i;
        }
    }
    return k;
}
int biggest_of(const int a, const int b){
    return (a > b ? a : b);
}
int smallest_of(const int a, const int b){
    return (a < b ? a : b);
}
void print_total_time(const cyclist* cyclist, const int i){
    int hours, minutes, seconds;
    hours = i / HOUR_IN_SECONDS;
    seconds = i % HOUR_IN_SECONDS;
    minutes = seconds / MINUTE_IN_SECONDS;
    seconds = seconds % MINUTE_IN_SECONDS;
    printf("%s is one of the cyclist with the shortest total\ntime in both races, which is: %d hours, %d minutes and %d seconds\n",
            cyclist->cyclist_name, hours, minutes, seconds);
}
double average_age_for_top_ten(cyclist* list, const int list_length, const int number_of_races){
    int i, current_race_number = 0, current_cyclists_in_top_ten_for_current_race = 0, all_top_tens_length = 0;
    double average_age = 0.0;
    char* current_race;
    cyclist *top_ten_cyclists, *clean_list;
    current_race = (char *)malloc(MAX_RACE_NAME_LENGTH * sizeof(char));
    top_ten_cyclists = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));

    printf("\n\n\n");

    qsort(list, list_length, sizeof(cyclist), compare_race_name);

    for(i = 0; i < list_length; i++){
        if(strcmp(current_race, list[i].race_name) != 0){
            if(current_race_number < number_of_races){
                if(current_cyclists_in_top_ten_for_current_race != 0){
                    current_race_number++;
                }
                strcpy(current_race, list[i].race_name);
                current_cyclists_in_top_ten_for_current_race = 0;
                top_ten_cyclists[10*current_race_number + current_cyclists_in_top_ten_for_current_race] = list[i];
                current_cyclists_in_top_ten_for_current_race++;
            }
        }else if(current_cyclists_in_top_ten_for_current_race < 10){
                top_ten_cyclists[10*current_race_number + current_cyclists_in_top_ten_for_current_race] = list[i];
                current_cyclists_in_top_ten_for_current_race++;
        }
    }

    all_top_tens_length = 10 * current_race_number + current_cyclists_in_top_ten_for_current_race;

    clean_list = remove_doublets(top_ten_cyclists, &all_top_tens_length);

    calculate_average_age(clean_list, &average_age, all_top_tens_length);

    free(clean_list);
    free(current_race);
    free(top_ten_cyclists);

    return average_age;
}
int compare_race_name(const void* a, const void* b){
    cyclist *pointer_one = (cyclist *) a;
    cyclist *pointer_two = (cyclist *) b;

    if(strcmp(pointer_one->race_name, pointer_two->race_name) == 0){
        if((pointer_one->placement > 0 ? pointer_one->placement : 99999) > (pointer_two->placement > 0 ? pointer_two->placement : 99999)){
            return 1;
        }else if((pointer_one->placement > 0 ? pointer_one->placement : 99999) < (pointer_two->placement > 0 ? pointer_two->placement : 99999)){
            return -1;
        }else{
            return 0;
        }
    }else{
        return strcmp(pointer_one->race_name, pointer_two->race_name);
    }
}
cyclist* remove_doublets(cyclist* list, int* list_length){
    int i, j = 0, k = 0;
    cyclist* clean_list;
    clean_list = (cyclist *)malloc(*list_length * sizeof(cyclist));
    for(i = 0; i < *list_length; i++){
        j = 0;
        while(strcmp(clean_list[j].cyclist_name, list[i].cyclist_name) != 0 && j < k){
            j++;
        }
        if(j == k){
            clean_list[k] = list[i];
            k++;
        }
    }
    *list_length = k;
    return clean_list;
}
void calculate_average_age(cyclist* list, double* average_age, int list_length){
    int current_position_in_array;
    for(current_position_in_array = 0; current_position_in_array < list_length; current_position_in_array++){
        *average_age += list[current_position_in_array].age;
    }
    *average_age = *average_age / list_length;
}
