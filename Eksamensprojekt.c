#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RACE_NAME_LENGTH 20
#define MAX_CYCLIST_NAME_LENGTH 30
#define ABBREVIATIONS_LENGTH 4
#define NUMBER_OF_SYMPOLS_IN_TIME 8
#define MAX_DIGITS_OF_PLACEMENT 4
#define TOTAL_NUMBER_OF_PARTICIPENTS 1000
#define TOTAL_NUMBER_OF_RACES 10
#define OTL -1
#define DNF -2

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

struct dane_who_finished_a_race{
    char cyclist_name[MAX_CYCLIST_NAME_LENGTH];
    int number_of_finished_races;
}; typedef struct dane_who_finished_a_race dane_who_finished_a_race;

void* read_file_to_list(int* list_length, race* races, int* number_of_races);
void count_up_race_rosters(const int i, const cyclist* output_pointer_to_array_of_cyclists, race* races, int* number_of_races);
void qsort(void *array, size_t lenght, size_t size, int (*compar)(const void *, const void*));
void print_italian_cyclists_above_thirty(const cyclist* list, const int list_length);
void print_list_of_cyclists(const cyclist* list, const int list_length, const int print_points);
dane_who_finished_a_race* danish_cyclists_who_finished_a_race(const cyclist* list, const int full_list_length, int* list_length);
void* create_and_sort_ranglist(cyclist* list, const int full_list_length, int* list_length, const race* races);
void assign_points_to_cyclists_for_every_race(cyclist* list, const int full_list_length, const race* races);
void get_last_name(cyclist* cyclist);
int compare_points(const void* a, const void* b);
void find_fastes_cyclist_of_paris_and_amstel(const cyclist* full_list, const int* full_list_length, cyclist* fastes_cyclist, int* time_for_fastes_cyclist);
void make_list_of_cyclists_paris_amstel(const cyclist* full_list, cyclist* paris_cyclists, cyclist* amstel_cyclists, const int* full_list_length, int* paris_list_length, int* amstel_list_length);
void make_array_with_positions_of_cyclists_in_the_two_arrays(const int paris_list_length, const int amstel_list_length, const cyclist* paris_cyclists, const cyclist* amstel_cyclists, int list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][2]);
int biggest_of(const int a, const int b);

int main(void){

    int i = 0, list_length_of_all_cyclists, list_length_of_danes_who_finished, list_length_of_ranglist, number_of_races = 0, total_time_for_fastes_cyclist;
    cyclist *list_of_cyclists, *ranglist_of_cyclists_with_total_points, fastes_cyclist;
    dane_who_finished_a_race* list_of_danish_cyclists_who_finished_a_race;
    race* races;

    races = (race *)malloc(TOTAL_NUMBER_OF_RACES * sizeof(race));

    list_of_cyclists = read_file_to_list(&list_length_of_all_cyclists, races, &number_of_races);

    /*print_list_of_cyclists(list_of_cyclists, list_length_of_all_cyclists);*/

    print_italian_cyclists_above_thirty(list_of_cyclists, list_length_of_all_cyclists);

    list_of_danish_cyclists_who_finished_a_race = danish_cyclists_who_finished_a_race(list_of_cyclists, list_length_of_all_cyclists, &list_length_of_danes_who_finished);

    for(i = 0; i < list_length_of_danes_who_finished; i++){
        printf("%s %d\n", list_of_danish_cyclists_who_finished_a_race[i].cyclist_name, list_of_danish_cyclists_who_finished_a_race[i].number_of_finished_races);
    }

    ranglist_of_cyclists_with_total_points = create_and_sort_ranglist(list_of_cyclists, list_length_of_all_cyclists, &list_length_of_ranglist, races);

    print_list_of_cyclists(ranglist_of_cyclists_with_total_points, 10, 1);

    find_fastes_cyclist_of_paris_and_amstel(list_of_cyclists, &list_length_of_all_cyclists, &fastes_cyclist, &total_time_for_fastes_cyclist);

    free(list_of_cyclists);
    free(list_of_danish_cyclists_who_finished_a_race);
    free(ranglist_of_cyclists_with_total_points);
    free(races);

    return 0;
}

void* read_file_to_list(int* list_length, race* races, int* number_of_races){
    int i = 0, j = 0, scan_result;
    char temporary_placeholder_placement[4];
    FILE* read_file_pointer = fopen("cykelloeb","r");

    cyclist* output_pointer_to_array_of_cyclists;
    output_pointer_to_array_of_cyclists = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));

    while(!feof(read_file_pointer)){
        scan_result = fscanf(read_file_pointer, " %[a-zA-Z] \"%[a-zA-Z-' ]\" | %d %[A-Z] %[A-Z] | %3[A-Z0-9] %d : %d : %d",
                                                output_pointer_to_array_of_cyclists[i].race_name,
                                                output_pointer_to_array_of_cyclists[i].cyclist_name,
                                                &output_pointer_to_array_of_cyclists[i].age,
                                                output_pointer_to_array_of_cyclists[i].team,
                                                output_pointer_to_array_of_cyclists[i].nationality,
                                                temporary_placeholder_placement,
                                                &output_pointer_to_array_of_cyclists[i].time[0],
                                                &output_pointer_to_array_of_cyclists[i].time[1],
                                                &output_pointer_to_array_of_cyclists[i].time[2]);

        if(strcmp(temporary_placeholder_placement,"OTL") == 0){
            output_pointer_to_array_of_cyclists[i].placement = (-1);
        }else if(strcmp(temporary_placeholder_placement,"DNF") == 0){
            output_pointer_to_array_of_cyclists[i].placement = (-2);
        }else{
            output_pointer_to_array_of_cyclists[i].placement = atoi(temporary_placeholder_placement);
        }
        output_pointer_to_array_of_cyclists[i].points = 0;

        get_last_name(output_pointer_to_array_of_cyclists);

        count_up_race_rosters(i, output_pointer_to_array_of_cyclists, races, number_of_races);

        i++;
    }
    i--;
    if(scan_result < 1){
        /*Insert ERROR code*/
    }
    *list_length = i;

    for(i = 0; i < j; i++){
        printf("%s %d\n", races[i].name, races[i].roster_size);
    }

    fclose(read_file_pointer);

    return output_pointer_to_array_of_cyclists;
}
void count_up_race_rosters(const int i, const cyclist* output_pointer_to_array_of_cyclists, race* races, int* number_of_races){
    int j = *number_of_races, k = 0, found_match = 0, end_of_array = 0;
    while(k <= j && found_match == 0 && end_of_array == 0){
        if((k == j && strcmp(output_pointer_to_array_of_cyclists[i].race_name, races[k].name) != 0)){
            strcpy(races[k].name, output_pointer_to_array_of_cyclists[i].race_name);
            races[k].roster_size = 1;
            j++;
            end_of_array = 1;
        }else if(strcmp(output_pointer_to_array_of_cyclists[i].race_name, races[k].name) == 0){
            races[k].roster_size++;
            found_match = 1;
        }else{
            k++;
        }
    }
    *number_of_races = j;
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
    print_list_of_cyclists(list_of_italian_cyclists_above_thirty, j, 0);
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
void print_list_of_cyclists(const cyclist* list, const int list_length, const int print_points){

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

        printf("%s %s | %d %s %s | %s   %d:%d:%d", list[i].race_name,
                                             list[i].cyclist_name,
                                             list[i].age,
                                             list[i].team,
                                             list[i].nationality,
                                             placement_as_string,
                                             list[i].time[0],
                                             list[i].time[1],
                                             list[i].time[2]);
        if(print_points == 1){
            printf("     Points: %d\n", list[i].points);
        }else{
            printf("\n");
        }
    }
    printf("\n\n\n");
}
void* create_and_sort_ranglist(cyclist* list, const int full_list_length, int* list_length, const race* races){
    int i = 0, j = 0, k = 0, found_match = 0, end_of_array = 0;

    cyclist* output_pointer_with_ranglist;
    output_pointer_with_ranglist = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));

    assign_points_to_cyclists_for_every_race(list, full_list_length, races);

    while(i < full_list_length){
        while(k <= j && found_match == 0 && end_of_array == 0){
            if(strcmp(list[i].cyclist_name, output_pointer_with_ranglist[k].cyclist_name) == 0){
                output_pointer_with_ranglist[k].points += list[i].points;
                found_match = 1;
                /*printf("found_match\n");
                printf("%d\n", output_pointer_with_ranglist[k].points);*/
            }else if(j == 0 || (strcmp(list[i].cyclist_name, output_pointer_with_ranglist[k].cyclist_name) != 0 && k == j)){
                output_pointer_with_ranglist[j] = list[i];
                j++;
                end_of_array = 1;
            }else{
                k++;
            }
        }
        i++;
        k = 0;
        found_match = 0;
        end_of_array = 0;
    }
    *list_length = j;

    qsort(output_pointer_with_ranglist, j, sizeof(struct cyclist), compare_points);

    /*print_list_of_cyclists(output_pointer_with_ranglist, j);*/

    return output_pointer_with_ranglist;

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
void find_fastes_cyclist_of_paris_and_amstel(const cyclist* full_list, const int* full_list_length, cyclist* fastes_cyclist, int* time_for_fastes_cyclist){
    int paris_list_length = 0, amstel_list_length = 0, list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][2];
    cyclist *paris_cyclists, *amstel_cyclists;

    paris_cyclists = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));
    amstel_cyclists = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));

    make_list_of_cyclists_paris_amstel(full_list, paris_cyclists, amstel_cyclists, full_list_length, &paris_list_length, &amstel_list_length);
    make_array_with_positions_of_cyclists_in_the_two_arrays(paris_list_length, amstel_list_length, paris_cyclists, amstel_cyclists, list_placements);

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
void make_array_with_positions_of_cyclists_in_the_two_arrays(const int paris_list_length, const int amstel_list_length, const cyclist* paris_cyclists, const cyclist* amstel_cyclists, int list_placements[TOTAL_NUMBER_OF_PARTICIPENTS][2]){
    int i, j, k = 0;
    for(i = 0; i < paris_list_length; i++){
        for(j = 0; j < amstel_list_length; j++){
            if(strcmp(paris_cyclists[i].cyclist_name, amstel_cyclists[j].cyclist_name) == 0){
                list_placements[k][0] = i;
                list_placements[k][1] = j;
                k++;
            }
        }
    }
}
int biggest_of(const int a, const int b){
    return (a > b ? a : b);
}
