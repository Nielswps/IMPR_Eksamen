/*Niels Frederik Sinding Vistisen
nvisti18@student.aau.dk
A401 - Software*/

/*First the needed libraries har included*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

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

/*Then structs for holding the information for a cyclist and a race are defined*/
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

struct race{
    char name[MAX_RACE_NAME_LENGTH];
    int roster_size;
};typedef struct race race;

/*I have chosen to create a struct containing only name and number of races for a
dane, so I would not have to assign a value of number of races to every cyclist*/
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
void print_list_of_cyclists(const cyclist* list, const int list_length);
dane_who_finished_a_race* danish_cyclists_who_finished_a_race(cyclist* list, const int full_list_length, int* list_length);
void print_danes(const dane_who_finished_a_race* list_of_danish_cyclists_who_finished_a_race, const int list_length_of_danes_who_finished);
void add_danes_to_array(dane_who_finished_a_race* list_of_danish_cyclists_who_finished_a_race, cyclist* list, int current_position_in_list_of_cyclists, int* current_length_of_danes_list);
void* create_and_sort_leaderboard(cyclist* list, const int full_list_length, int* list_length, const race* races);
void assign_points_to_cyclists_for_every_race(cyclist* list, const int full_list_length, const race* races);
void podium_points(cyclist* current_cyclist, const race* races);
void accumulate_points_for_every_cyclist_and_remove_doublets(cyclist* list, cyclist* output_pointer_with_leaderboard, int current_position_in_list_of_cyclists, int* current_length_of_leaderboard);
int compare_points(const void* a, const void* b);
void print_overall_leaderboard(const cyclist* list, const int list_length);
void get_last_name(cyclist* cyclist);
void find_fastes_cyclist_of_paris_and_amstel(cyclist* full_list, const int* full_list_length, cyclist* fastes_cyclist, int* time_for_fastes_cyclist);
void make_list_of_cyclists_paris_amstel(const cyclist* full_list, cyclist* paris_cyclists, cyclist* amstel_cyclists, const int* full_list_length, int* paris_list_length, int* amstel_list_length);
void make_array_with_positions_of_cyclists_in_the_two_arrays(const int paris_list_length, const int amstel_list_length, const cyclist* paris_cyclists, const cyclist* amstel_cyclists, int list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][3], int* number_of_cyclists_in_both_races);
void checking_amstel_race_and_time_for_both_races(const int paris_list_length, const int amstel_list_length, const cyclist* paris_cyclists, const cyclist* amstel_cyclists, int current_position_in_paris_list, int list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][3], int* current_length_of_placement_list);
int get_total_time(int paris_time[3], int amstel_time[3]);
int find_shortest_time(int list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][3], int list_length);
int smallest_of(const int a, const int b);
void print_total_time(const cyclist* cyclist, const int total_time_in_seconds);
double average_age_for_top_ten(cyclist* list, const int list_length, const int number_of_races);
int compare_race_name(const void* a, const void* b);
cyclist* remove_doublets(cyclist* list, int* list_length);
void add_unique_cyclists_to_list(cyclist* clean_list, cyclist* list, int current_position_in_list, int* clean_list_length);
void calculate_average_age(cyclist* list, double* average_age, int list_length);
void handle_input(int* scan_result, int* user_input, int* user_typed_exit);

int main(int argc, char const *argv[]){

    int scan_result = 0, user_input = 0, user_typed_exit = 0, list_length_of_all_cyclists, list_length_of_danes_who_finished,
    list_length_of_leaderboard, number_of_races = 0, total_time_for_fastes_cyclist;
    cyclist *list_of_cyclists, *leaderboard_of_cyclists_with_total_points, *fastes_cyclist;
    dane_who_finished_a_race* list_of_danish_cyclists_who_finished_a_race;
    race* races;
    races = (race *)malloc(TOTAL_NUMBER_OF_RACES * sizeof(race));

    printf("\n\n\n");
    printf("Made by: Niels Frederik Sinding Vistisen, 2018");
    printf("\n\n\n");

    /*The number of input parameters is checked*/
    if(argc > 1){
        if(strcmp(argv[1], "--print") == 0){
            /*If the input is --print, the results for every assignment should be printed*/
            /*The file is first read*/
            list_of_cyclists = read_file_to_list(&list_length_of_all_cyclists, races, &number_of_races);
            print_assignment_number(1);

            /*The function for finding and printing the italians above thirty is called*/
            print_italian_cyclists_above_thirty(list_of_cyclists, list_length_of_all_cyclists);

            print_assignment_number(2);
            /*Then the list of danes who have finished at least one race is found*/
            list_of_danish_cyclists_who_finished_a_race = danish_cyclists_who_finished_a_race(list_of_cyclists, list_length_of_all_cyclists, &list_length_of_danes_who_finished);

            /*Here the danes are printed*/
            print_danes(list_of_danish_cyclists_who_finished_a_race, list_length_of_danes_who_finished);

            /*The dynamicly allocated memory is then freed*/
            free(list_of_danish_cyclists_who_finished_a_race);

            print_assignment_number(3);
            /*The leaderboard is made using the create_and_sort_leaderboard function*/
            leaderboard_of_cyclists_with_total_points = create_and_sort_leaderboard(list_of_cyclists, list_length_of_all_cyclists, &list_length_of_leaderboard, races);

            /*The top ten is then printed*/
            print_overall_leaderboard(leaderboard_of_cyclists_with_total_points, 10);

            /*That dynamicly allocated memory is then freed*/
            free(leaderboard_of_cyclists_with_total_points);

            print_assignment_number(4);
            /*The fastes_cyclist pointer is set to point to the full list, as it will need a value to be a parameter in the following function*/
            fastes_cyclist = list_of_cyclists;
            find_fastes_cyclist_of_paris_and_amstel(list_of_cyclists, &list_length_of_all_cyclists, fastes_cyclist, &total_time_for_fastes_cyclist);

            /*fastes_cyclist is then printed out, with the total time*/
            print_total_time(fastes_cyclist, total_time_for_fastes_cyclist);

            print_assignment_number(5);
            /*The average age for the top ten cyclists is then printed and calculated*/
            printf("The average age for cyclists in the top 10 of any race is: %lf years\n\n\n", average_age_for_top_ten(list_of_cyclists, list_length_of_all_cyclists, number_of_races));
        }else{
            /*If there is a second input parameter, but it is not --print, the program terminates*/
            printf("Incorrect argument for the program\n");
            exit(-1);
        }
    }else{
        handle_input(&scan_result, &user_input, &user_typed_exit);

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

                /*The dynamicly allocated memory is then freed*/
                free(list_of_danish_cyclists_who_finished_a_race);
            break;

            case 3:
                /*The leaderboard is made using the create_and_sort_leaderboard function*/
                leaderboard_of_cyclists_with_total_points = create_and_sort_leaderboard(list_of_cyclists, list_length_of_all_cyclists, &list_length_of_leaderboard, races);

                /*The top ten is then printed*/
                print_overall_leaderboard(leaderboard_of_cyclists_with_total_points, 10);

                /*The dynamicly allocated memory is then freed*/
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
                printf("The average age for cyclists in the top 10 of any race is: %lf years\n\n\n", average_age_for_top_ten(list_of_cyclists, list_length_of_all_cyclists, number_of_races));
            break;

            default:
                /*If an unexpected input slips through my if statement in the
                handle_input function, the program terminates*/
                printf("Something went wrong with the input...\n");
                exit(-1);
            break;
        }
        /*The remaining dynamicly allocated memory is then freed*/
        free(list_of_cyclists);
        free(fastes_cyclist);
        free(races);
    }

    /*Main returns 0 and the program ends*/
    return 0;
}

/*The following is all the functions needed, sorted in order of usage*/
void* read_file_to_list(int* list_length, race* races, int* number_of_races){
    int current_position_in_array = 0, errno;
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
            fscanf(read_file_pointer, " %[a-zA-Z] \"%[a-zA-Z-' ]\" | %d %[A-Z] %[A-Z] | %3[A-Z0-9] %d : %d : %d",
                                    output_pointer_to_array_of_cyclists[current_position_in_array].race_name,
                                    output_pointer_to_array_of_cyclists[current_position_in_array].cyclist_name,
                                    &output_pointer_to_array_of_cyclists[current_position_in_array].age,
                                    output_pointer_to_array_of_cyclists[current_position_in_array].team,
                                    output_pointer_to_array_of_cyclists[current_position_in_array].nationality,
                                    temporary_placeholder_placement,
                                    &output_pointer_to_array_of_cyclists[current_position_in_array].time[0],
                                    &output_pointer_to_array_of_cyclists[current_position_in_array].time[1],
                                    &output_pointer_to_array_of_cyclists[current_position_in_array].time[2]);

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
        /*The current_position_in_array is decreased by one, so that is gives the
        correct length of the list (starting from 0)*/
        current_position_in_array--;

        /*The length of the list is returned using a pointer*/
        *list_length = current_position_in_array;

        /*The file gets closed*/
        fclose(read_file_pointer);

        /*A pointer to the list is returned*/
        return output_pointer_to_array_of_cyclists;
    }
}
void print_assignment_number(int assignment_number){
    printf("-----Result for assignment %d-----\n\n", assignment_number);
}
void count_up_race_rosters(const int cyclist_number, const cyclist* output_pointer_to_array_of_cyclists, race* races, int* number_of_races){
    int number_of_current_race = *number_of_races, current_position_in_race_array = 0, found_match = 0, end_of_array = 0;
    /*The race_name from the cyclist is checked with every race already added to the race list.
    If it is already in the list, its roster_size is increased by 1, else the race is added*/
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

    /*The array is then printet*/
    print_list_of_cyclists(list_of_italian_cyclists_above_thirty, current_position_in_italian_array);
}
void print_list_of_cyclists(const cyclist* list, const int list_length){
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
        printf("%-28s %-30s | %d %s %s | %-10s   ", list[current_position_in_list].race_name,
                                             list[current_position_in_list].cyclist_name,
                                             list[current_position_in_list].age,
                                             list[current_position_in_list].team,
                                             list[current_position_in_list].nationality,
                                             placement_as_string);

        /*The time is printed out, if the cyclist finished, else "--:--:--" is printed*/
        if(list[current_position_in_list].placement > DNF){
            printf("%02d:%02d:%02d\n", list[current_position_in_list].time[0],
                                    list[current_position_in_list].time[1],
                                    list[current_position_in_list].time[2]);
        }else{
            printf("--:--:--\n");
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

    /*Every dane with a better placement than DNF is either added to the array or,
    if the cyclist is already in the array, the number of races finished is increased by 1*/
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
void print_danes(const dane_who_finished_a_race* list_of_danish_cyclists_who_finished_a_race, const int list_length_of_danes_who_finished){
    int current_position_in_array;
    /*The input array is run through and printed*/
    for(current_position_in_array = 0; current_position_in_array < list_length_of_danes_who_finished; current_position_in_array++){
        printf("%-28s %d\n", list_of_danish_cyclists_who_finished_a_race[current_position_in_array].cyclist_name,
                          list_of_danish_cyclists_who_finished_a_race[current_position_in_array].number_of_finished_races);
    }
    printf("\n\n\n");
}
void* create_and_sort_leaderboard(cyclist* list, const int full_list_length, int* list_length, const race* races){
    int current_position_in_list_of_cyclists = 0, current_length_of_leaderboard = 0;

    cyclist* output_pointer_with_leaderboard;
    output_pointer_with_leaderboard = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));

    /*First every cyclist is given the points they earned for every race*/
    assign_points_to_cyclists_for_every_race(list, full_list_length, races);

    /*Then the list is run through to get rid of doublets and accumulate the points for every single cyclist*/
    while(current_position_in_list_of_cyclists < full_list_length){
        accumulate_points_for_every_cyclist_and_remove_doublets(list, output_pointer_with_leaderboard, current_position_in_list_of_cyclists, &current_length_of_leaderboard);
        current_position_in_list_of_cyclists++;
    }
    *list_length = current_length_of_leaderboard;

    /*The leaderboard is then sorted, so that the points are decreasing down the list*/
    qsort(output_pointer_with_leaderboard, current_length_of_leaderboard, sizeof(cyclist), compare_points);

    return output_pointer_with_leaderboard;
}
void assign_points_to_cyclists_for_every_race(cyclist* list, const int full_list_length, const race* races){
    int current_position_in_list = 0;

    while(current_position_in_list < full_list_length){
        if(list[current_position_in_list].placement > -1){
            list[current_position_in_list].points += 3;
            podium_points(&list[current_position_in_list], races);
        }else if(list[current_position_in_list].placement == -1){
            list[current_position_in_list].points += 1;
        }
        current_position_in_list++;
    }
}
void podium_points(cyclist* current_cyclist, const race* races){
    int current_position_in_races = 0;
    while(strcmp(current_cyclist->race_name, races[current_position_in_races].name) != 0){
        current_position_in_races++;
    }
    current_cyclist->points += (races[current_position_in_races].roster_size - current_cyclist->placement)/13;
    if(current_cyclist->placement == 1){
        current_cyclist->points += 10;
    }else if(current_cyclist->placement == 2){
        current_cyclist->points += 5;
    }else if(current_cyclist->placement == 3){
        current_cyclist->points += 2;
    }
}
void accumulate_points_for_every_cyclist_and_remove_doublets(cyclist* list, cyclist* output_pointer_with_leaderboard, int current_position_in_list_of_cyclists, int* current_length_of_leaderboard){
    int current_position_in_leaderboard = 0, found_match = 0, end_of_array = 0;

    /*Goes through the leaderboard to accumulate the points for every cyclist and removes the doublets,
    where cyclists have participated in multiple races*/
    while(current_position_in_leaderboard <= *current_length_of_leaderboard && found_match == 0 && end_of_array == 0){

        /*Checks if the current a cyclist is already on the list*/
        if(strcmp(list[current_position_in_list_of_cyclists].cyclist_name, output_pointer_with_leaderboard[current_position_in_leaderboard].cyclist_name) == 0){
            output_pointer_with_leaderboard[current_position_in_leaderboard].points += list[current_position_in_list_of_cyclists].points;
            found_match = 1;
        /*Else checks if the leaderboard has any elements in it or the end of the leaderboard has been reached*/
        }else if(*current_length_of_leaderboard == 0 || current_position_in_leaderboard == *current_length_of_leaderboard){
            /*Adds the cyclist to the leaderboard, increases the length of it by 1 and ends the loop*/
            output_pointer_with_leaderboard[*current_length_of_leaderboard] = list[current_position_in_list_of_cyclists];
            *current_length_of_leaderboard += 1;
            end_of_array = 1;
        }else{
            /*If the end has not been reached and no match has been found so far, the next cyclist is checked*/
            current_position_in_leaderboard++;
        }
    }
}
int compare_points(const void* a, const void* b){
    cyclist *pointer_one = (cyclist *) a;
    cyclist *pointer_two = (cyclist *) b;
    /*Here the points of the 2 cyclists are compared and if they are equal,
    their last name is compared*/
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
void print_overall_leaderboard(const cyclist* list, const int list_length){
    int current_position_in_list;
    for(current_position_in_list = 0; current_position_in_list < list_length; current_position_in_list++){
        /*Every cyclist is printed out*/
        printf("%-30s | %d %s %s | Total points: %d\n", list[current_position_in_list].cyclist_name,
                                             list[current_position_in_list].age,
                                             list[current_position_in_list].team,
                                             list[current_position_in_list].nationality,
                                             list[current_position_in_list].points);
    }

    /*Then 3 new lines are added to create space and make it easier to differentiate the outputs*/
    printf("\n\n\n");
}
void get_last_name(cyclist* cyclist){
    int current_position_in_name, exit = 0, full_name_length;
    full_name_length = strlen(cyclist->cyclist_name);

    /*This loops runs through the name from the back, until it meets a lowercase letter,
    in which case it return the position +2, due to the fact that the lowercase letter
    and the space after it, is not wanted in the last name*/
    for(current_position_in_name = full_name_length; current_position_in_name > 0 && exit == 0; current_position_in_name--){
        if(current_position_in_name == islower(*(cyclist->cyclist_name + current_position_in_name))){
            current_position_in_name += 2;
            strncpy(cyclist->last_name, (cyclist->cyclist_name) + current_position_in_name, full_name_length - current_position_in_name);
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
    /*This function makes an array of integers pointing to the locations of a cyclist in the 2 races*/
    make_array_with_positions_of_cyclists_in_the_two_arrays(paris_list_length, amstel_list_length, paris_cyclists, amstel_cyclists, list_placements, &number_of_cyclists_in_both_races);

    /*Then the total time for every cyclist is found*/
    for(i = 0; i < number_of_cyclists_in_both_races; i++){
        list_placements[i][2] = get_total_time(paris_cyclists[list_placements[i][0]].time, amstel_cyclists[list_placements[i][1]].time);
    }

    /*Placement in the list of placement is found for the fastes cyclist.
    i is the position in the placement list according to the fastes cyclist.
    This way list_placements[i][0] is the placement for the cyclist in the Paris race,
    list_placements[i][1] is the placement for the cyclist in the Amstel race, and
    list_placements[i][2] is the total time for the cyclist*/
    i = find_shortest_time(list_placements, number_of_cyclists_in_both_races);

    /*The way list_placements is made means, that list_placements[i] and full_list[i] is the same cyclist*/
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
    int current_position_in_paris_list, current_length_of_placement_list = 0;

    /*The 2 loops compare the cyclists in the 2 races and finds those, who participated in both*/
    for(current_position_in_paris_list = 0; current_position_in_paris_list < paris_list_length; current_position_in_paris_list++){
        checking_amstel_race_and_time_for_both_races(paris_list_length, amstel_list_length, paris_cyclists, amstel_cyclists, current_position_in_paris_list, list_placements, &current_length_of_placement_list);
    }
    *number_of_cyclists_in_both_races = current_length_of_placement_list;
}
void checking_amstel_race_and_time_for_both_races(const int paris_list_length, const int amstel_list_length, const cyclist* paris_cyclists, const cyclist* amstel_cyclists, int current_position_in_paris_list, int list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][3], int* current_length_of_placement_list){
    int current_position_in_amstel_list;
    /*Runs through the list of cyclists in the Amstel race and checks, if they participated
    in the Paris race and if, they finished both races. If they did, their position in
    the 2 lists of the races is added to the list_placements array*/
    for(current_position_in_amstel_list = 0; current_position_in_amstel_list < amstel_list_length; current_position_in_amstel_list++){
        if(strcmp(paris_cyclists[current_position_in_paris_list].cyclist_name, amstel_cyclists[current_position_in_amstel_list].cyclist_name) == 0 &&
        (paris_cyclists[current_position_in_paris_list].time[0] + paris_cyclists[current_position_in_paris_list].time[1] + paris_cyclists[current_position_in_paris_list].time[2]) > 0 &&
        (amstel_cyclists[current_position_in_amstel_list].time[0] + amstel_cyclists[current_position_in_amstel_list].time[1] + amstel_cyclists[current_position_in_amstel_list].time[2]) > 0){
            list_placements[*current_length_of_placement_list][0] = current_position_in_paris_list;
            list_placements[*current_length_of_placement_list][1] = current_position_in_amstel_list;
            *current_length_of_placement_list += 1;
        }
    }
}
int get_total_time(int paris_time[3], int amstel_time[3]){
    /*Simple calculation to finde the total time in seconds*/
    return (paris_time[0]*HOUR_IN_SECONDS + paris_time[1]*MINUTE_IN_SECONDS + paris_time[2]) +
    (amstel_time[0]*HOUR_IN_SECONDS + amstel_time[1]*MINUTE_IN_SECONDS + amstel_time[2]);
}
int find_shortest_time(int list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][3], int list_length){
    int current_position_in_list, current_shortest_time = INFINITY, position_of_shortest_time;
    for(current_position_in_list = 0; current_position_in_list < list_length; current_position_in_list++){
        current_shortest_time = smallest_of(current_shortest_time, list_placements[current_position_in_list][2]);
        if(current_shortest_time == list_placements[current_position_in_list][2]){
            position_of_shortest_time = current_position_in_list;
        }
    }
    return position_of_shortest_time;
}
int smallest_of(const int a, const int b){
    return (a < b ? a : b);
}
void print_total_time(const cyclist* cyclist, const int total_time_in_seconds){
    int hours, minutes, seconds;
    /*prints the total time of the shortest time in hours, minutes and seconds*/
    hours = total_time_in_seconds / HOUR_IN_SECONDS;
    seconds = total_time_in_seconds % HOUR_IN_SECONDS;
    minutes = seconds / MINUTE_IN_SECONDS;
    seconds = seconds % MINUTE_IN_SECONDS;
    printf("%s is one of the cyclists with the shortest time across both races.\n%s had a combined time of: %d hours, %d minutes and %d seconds\n\n\n",
            cyclist->cyclist_name, cyclist->cyclist_name, hours, minutes, seconds);
}
double average_age_for_top_ten(cyclist* list, const int list_length, const int number_of_races){
    int current_position_in_list, current_race_number = 0, current_cyclists_in_top_ten_for_current_race = 0, all_top_tens_length = 0;
    double average_age = 0.0;
    char* current_race;
    cyclist *top_ten_cyclists, *clean_list;
    current_race = (char *)malloc(MAX_RACE_NAME_LENGTH * sizeof(char));
    top_ten_cyclists = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));

    /*First the list is sorted, so that every cyclist in each race is together with decreasing placements*/
    qsort(list, list_length, sizeof(cyclist), compare_race_name);

    /*Then the list of every cyclist is run through. First the race_name of teh current cyclist is compared with the current race.
    If it is the same, then the number of cyclist in top 10 for that race is checked and if there is less than 10, the cyclist is added.
    If the race_name and the name of the current is not equal, then the race is added to the list, as long as the number_of_races is not reached*/
    for(current_position_in_list = 0; current_position_in_list < list_length; current_position_in_list++){
        if(strcmp(current_race, list[current_position_in_list].race_name) != 0 && current_race_number < number_of_races){
            if(current_cyclists_in_top_ten_for_current_race != 0){
                current_race_number++;
            }
            strcpy(current_race, list[current_position_in_list].race_name);
            current_cyclists_in_top_ten_for_current_race = 0;
            top_ten_cyclists[10*current_race_number + current_cyclists_in_top_ten_for_current_race] = list[current_position_in_list];
            current_cyclists_in_top_ten_for_current_race++;
        }else if(current_cyclists_in_top_ten_for_current_race < 10){
                top_ten_cyclists[10*current_race_number + current_cyclists_in_top_ten_for_current_race] = list[current_position_in_list];
                current_cyclists_in_top_ten_for_current_race++;
        }
    }

    /*Then the nubmer of cyclists in a top 10 is found*/
    all_top_tens_length = 10 * current_race_number + current_cyclists_in_top_ten_for_current_race;

    /*And the list is cleaned of doublets*/
    clean_list = remove_doublets(top_ten_cyclists, &all_top_tens_length);

    /*Lastly the average age is calculated, the memory is freed and the average age is returned*/
    calculate_average_age(clean_list, &average_age, all_top_tens_length);

    free(clean_list);
    free(current_race);
    free(top_ten_cyclists);

    return average_age;
}
int compare_race_name(const void* a, const void* b){
    cyclist *pointer_one = (cyclist *) a;
    cyclist *pointer_two = (cyclist *) b;

    /*The race_name is checked for the 2 cyclists and then the placement is checked,
    if they compete in the same race*/
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
    int current_position_in_list, clean_list_length = 0;
    cyclist* clean_list;
    clean_list = (cyclist *)malloc(*list_length * sizeof(cyclist));
    /*The list is run through and all unique cyclists are added to the clean_list*/
    for(current_position_in_list = 0; current_position_in_list < *list_length; current_position_in_list++){
        add_unique_cyclists_to_list(clean_list, list, current_position_in_list, &clean_list_length);
    }
    *list_length = clean_list_length;
    return clean_list;
}
void add_unique_cyclists_to_list(cyclist* clean_list, cyclist* list, int current_position_in_list, int* clean_list_length){
    int current_position_in_clean_list = 0;
    /*The cyclist is checked and added to the clean_list, if not already on it*/
    while(strcmp(clean_list[current_position_in_clean_list].cyclist_name, list[current_position_in_list].cyclist_name) != 0
    && current_position_in_clean_list < *clean_list_length){
        current_position_in_clean_list++;
    }
    if(current_position_in_clean_list == *clean_list_length){
        clean_list[*clean_list_length] = list[current_position_in_list];
        *clean_list_length += 1;
    }
}
void calculate_average_age(cyclist* list, double* average_age, int list_length){
    int current_position_in_array;
    /*Adds up the ages for every cyclist in the clean list, which is without doublets*/
    for(current_position_in_array = 0; current_position_in_array < list_length; current_position_in_array++){
        *average_age += list[current_position_in_array].age;
    }
    /*Finds the average by deviding the added up result with the number of cyclists*/
    *average_age = *average_age / list_length;
}
void handle_input(int* scan_result, int* user_input, int* user_typed_exit){
    /*The while loop makes sure the program does not just exit, if the user input is unexpected*/
    while(*scan_result < 1){
        printf("Welcome to the program for calculating different aspects of:\nParis Roubaix, Amstel Gold Race, La La Fleche Wallonne, and Liege Bastogne Liege.\nYou can decide which one of the following results you want to see, by typing the corresponding number to the terminal.\n");
        printf("\"1\": A list of every Italian cyclist above 30\n");
        printf("\"2\": A list of every Danish cyclist, who has completed a race, and the number of races the cyclist has completed\n");
        printf("\"3\": The top 10 of most points overall\n");
        printf("\"4\": The fastes cyclist to complete both Paris Roubaix and Amstel Gold Race and the total time for both races\n");
        printf("\"5\": The average age for the top 10 cyclists in all the races\n");
        printf("\n(Type \"0\" to exit the program)\n");
        *scan_result = scanf("%d", user_input);
        printf("\n\n\n");
        /*The if-statement checks if the scanf was succesfull and contains an expected input*/
        if(*scan_result < 1 || *user_input > 5  || *user_input < 0){
            printf("This is an invalid input, please type another one from the list (a number between 1-5)\n");
            *scan_result = 0;
            printf("\n\n");
        /*If the input is the exit number, the program closes down*/
        }else if(*user_input == 0){
            printf("You have exited the program.\n");
            *user_typed_exit = 1;
            exit(-1);
        }
    }
}
