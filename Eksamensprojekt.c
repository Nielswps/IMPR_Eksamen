#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RACE_NAME_LENGTH 20
#define MAX_CYCLIST_NAME_LENGTH 30
#define ABBREVIATIONS_LENGTH 4
#define NUMBER_OF_SYMPOLS_IN_TIME 8
#define MAX_DIGITS_OF_PLACEMENT 4
#define TOTAL_NUMBER_OF_PARTICIPENTS 1000
#define OTL -1
#define DNF -2

struct race{
    char name[MAX_RACE_NAME_LENGTH];
    int roster_size;
};typedef struct race race;

struct cyclist{
    char race_name[MAX_RACE_NAME_LENGTH];
    char cyclist_name[MAX_CYCLIST_NAME_LENGTH];
    int age;
    char team[ABBREVIATIONS_LENGTH];
    char nationality[ABBREVIATIONS_LENGTH];
    int placement;
    char time[NUMBER_OF_SYMPOLS_IN_TIME];
    int points;
}; typedef struct cyclist cyclist;

struct dane_who_finished_a_race{
    char cyclist_name[MAX_CYCLIST_NAME_LENGTH];
    int number_of_finished_races;
}; typedef struct dane_who_finished_a_race dane_who_finished_a_race;

void* read_file_to_list(int* list_length, race* races, int* number_of_races);
void print_italian_cyclists_above_thirty(const cyclist* list, const int list_length);
void print_list_of_cyclists(const cyclist* list, const int list_length);
dane_who_finished_a_race* danish_cyclists_who_finished_a_race(const cyclist* list, const int full_list_length, int* list_length);
cyclist* create_and_sort_ranglist(cyclist* list, const int full_list_length, int* list_length, const race* races);
void assign_point_to_cyclists_for_every_race(cyclist* list, const int full_list_length, const race* races);

int main(void){

    int i = 0, list_length_of_all_cyclists, list_length_of_danes_who_finished, list_length_of_ranglist, number_of_races = 0;
    cyclist* list_of_cyclists;
    dane_who_finished_a_race* list_of_danish_cyclists_who_finished_a_race;
    cyclist* ranglist_of_cyclists_with_total_points;
    race* races;

    races = (race *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(race));

    list_of_cyclists = read_file_to_list(&list_length_of_all_cyclists, races, &number_of_races);

    print_list_of_cyclists(list_of_cyclists, 2/*list_length_of_all_cyclists*/);

    print_italian_cyclists_above_thirty(list_of_cyclists, list_length_of_all_cyclists);

    list_of_danish_cyclists_who_finished_a_race = danish_cyclists_who_finished_a_race(list_of_cyclists, list_length_of_all_cyclists, &list_length_of_danes_who_finished);

    for(i = 0; i < list_length_of_danes_who_finished; i++){
        printf("%s %d\n", list_of_danish_cyclists_who_finished_a_race[i].cyclist_name, list_of_danish_cyclists_who_finished_a_race[i].number_of_finished_races);
    }

    ranglist_of_cyclists_with_total_points = create_and_sort_ranglist(list_of_cyclists, list_length_of_all_cyclists, &list_length_of_ranglist);

    free(list_of_cyclists);
    free(list_of_danish_cyclists_who_finished_a_race);
    free(ranglist_of_cyclists_with_total_points);
    free(races);

    return 0;
}

void* read_file_to_list(int* list_length, race* races, int* number_of_races){
    int i = 0, j = 0, k = 0, found_match = 0, end_of_array = 0, scan_result;
    char temporary_placeholder_placement[4];
    FILE* read_file_pointer = fopen("cykelloeb","r");

    cyclist* output_pointer_to_array_of_cyclists;
    output_pointer_to_array_of_cyclists = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));

    while(!feof(read_file_pointer)){
        scan_result = fscanf(read_file_pointer, " %[a-zA-Z] \"%[a-zA-Z-' ]\" | %d %[A-Z] %[A-Z] | %3[A-Z0-9] %[0-9:-]", output_pointer_to_array_of_cyclists[i].race_name, output_pointer_to_array_of_cyclists[i].cyclist_name, &output_pointer_to_array_of_cyclists[i].age, output_pointer_to_array_of_cyclists[i].team, output_pointer_to_array_of_cyclists[i].nationality, temporary_placeholder_placement, output_pointer_to_array_of_cyclists[i].time);
        if(strcmp(temporary_placeholder_placement,"OTL") == 0){
            output_pointer_to_array_of_cyclists[i].placement = (-1);
        }else if(strcmp(temporary_placeholder_placement,"DNF") == 0){
            output_pointer_to_array_of_cyclists[i].placement = (-2);
        }else{
            output_pointer_to_array_of_cyclists[i].placement = atoi(temporary_placeholder_placement);
        }
        output_pointer_to_array_of_cyclists[i].points = 0;

        while(k <= j && found_match == 0 && end_of_array == 0){
            if((k == j && strcmp(output_pointer_to_array_of_cyclists[i].race_name, races[k].name) != 0)){
                strncpy(races[k].name, output_pointer_to_array_of_cyclists[i].race_name, strlen(output_pointer_to_array_of_cyclists[i].race_name));
                races[k].roster_size = 0;
                j++;
                end_of_array = 1;
            }else if(strcmp(output_pointer_to_array_of_cyclists[i].race_name, races[k].name) == 0){
                races[k].roster_size++;
                found_match = 1;
            }else{
                k++;
            }
        }
        k = 0; found_match = 0; end_of_array = 0;
        i++;
    }
    printf("%d\n", i);
    if(scan_result < 1){
        printf("%d\n", scan_result);
    }
    *list_length = i;
    *number_of_races = j;

    for(i = 0; i < j; i++){
        printf("%s %d\n", races[i].name, races[i].roster_size);
    }

    fclose(read_file_pointer);

    return output_pointer_to_array_of_cyclists;
}

void print_italian_cyclists_above_thirty(const cyclist* list, const int list_length){
    int i = 0, j = 0;
    cyclist list_of_italian_cyclists_above_thirty[TOTAL_NUMBER_OF_PARTICIPENTS];
    while(i < list_length){
        if((strcmp(list[i].nationality,"ITA") == 0) && list[i].age > 30){
            list_of_italian_cyclists_above_thirty[j] = list[i];
            j++;
        }
        i++;
    }
    print_list_of_cyclists(list_of_italian_cyclists_above_thirty, j);
}

dane_who_finished_a_race* danish_cyclists_who_finished_a_race(const cyclist* list, const int full_list_length, int* list_length){
    int i = 0, j = 0, k = 0, found_match = 0, end_of_array = 0;

    dane_who_finished_a_race *list_of_danish_cyclists_who_finished_a_race;
    list_of_danish_cyclists_who_finished_a_race = (dane_who_finished_a_race *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(dane_who_finished_a_race));

    while(i < full_list_length){
        if((strcmp(list[i].nationality,"DEN") == 0) && (list[i].placement > DNF)){
            k = 0;
            found_match = 0;
            end_of_array = 0;
            while(k <= j && found_match == 0 && end_of_array == 0){
                if(strcmp(list[i].cyclist_name, list_of_danish_cyclists_who_finished_a_race[k].cyclist_name) == 0){
                    list_of_danish_cyclists_who_finished_a_race[k].number_of_finished_races++;
                    found_match = 1;
                }else if((strcmp(list[i].cyclist_name,list_of_danish_cyclists_who_finished_a_race[k].cyclist_name) != 0 && k == j) || j == 0){
                    strncpy(list_of_danish_cyclists_who_finished_a_race[j].cyclist_name,list[i].cyclist_name,sizeof(list[i].cyclist_name)+2);
                    list_of_danish_cyclists_who_finished_a_race[j].number_of_finished_races = 1;
                    j++;
                    end_of_array = 1;
                }else{
                    k++;
                }
            }
        }
        i++;
    }
    *list_length = j;
    return list_of_danish_cyclists_who_finished_a_race;
}

void print_list_of_cyclists(const cyclist* list, const int list_length){

    int i;
    char placement_as_string[4];

    for(i = 0; i < list_length; i++){

        if(list[i].placement > -1){
            sprintf(placement_as_string, "%d", list[i].placement);
        }else if(list[i].placement == OTL){
            strncpy(placement_as_string, "OTL", 3);
        }else{
            strncpy(placement_as_string, "DNF", 3);
        }

        printf("%s %s | %d %s %s | %s %s\n", list[i].race_name,
                                             list[i].cyclist_name,
                                             list[i].age,
                                             list[i].team,
                                             list[i].nationality,
                                             placement_as_string,
                                             list[i].time);
    }
    printf("\n\n\n");
}

cyclist* create_and_sort_ranglist(cyclist* list, const int full_list_length, int* list_length, const race* races){
    int i = 0, j = 0, k = 0, found_match = 0, end_of_array = 0;

    cyclist* output_pointer_with_ranglist;
    output_pointer_with_ranglist = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));

    assign_point_to_cyclists_for_every_race(list, full_list_length, races);

    while(i < full_list_length){

        k = 0;
        found_match = 0;
        end_of_array = 0;
        while(k <= j && found_match == 0 && end_of_array == 0){
            if(strcmp(list[i].cyclist_name, output_pointer_with_ranglist[k].cyclist_name) == 0){
                output_pointer_with_ranglist[k].points += list[i].points;
                found_match = 1;
                printf("found_match\n");
            }else if(j == 0 || (strcmp(list[i].cyclist_name, output_pointer_with_ranglist[k].cyclist_name) != 0 && k == j)){
                output_pointer_with_ranglist[j] = list[i];
                j++;
                printf("j: %d\n", j);
                end_of_array = 1;
            }else{
                k++;
                printf("k: %d\n", k);
            }
        }
        i++;
    }
    *list_length = j;
    return output_pointer_with_ranglist;

}

void assign_point_to_cyclists_for_every_race(cyclist* list, const int full_list_length, const race* races){
    int i = 0;

    while(i < full_list_length){
        if(list[i].placement > -1){
            list[i].points += 3;
            
        }else if(list[i].placement == -1){
            list[i].points += 1;
        }
    }
}
