#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RACE_NAME_LENGTH 20
#define MAX_CYCLIST_NAME_LENGTH 30
#define ABBREVIATIONS_LENGTH 4
#define NUMBER_OF_SYMPOLS_IN_TIME 8
#define MAX_DIGITS_OF_PLACEMENT 4
#define TOTAL_NUMBER_OF_PARTICIPENTS 1000

struct cyclist{
    char race_name[MAX_RACE_NAME_LENGTH];
    char cyclist_name[MAX_CYCLIST_NAME_LENGTH];
    int age;
    char team[ABBREVIATIONS_LENGTH];
    char nationality[ABBREVIATIONS_LENGTH];
    char placement[MAX_DIGITS_OF_PLACEMENT];
    char time[NUMBER_OF_SYMPOLS_IN_TIME];
}; typedef struct cyclist cyclist;

void* read_file_to_list(int* list_length);

void print_italian_cyclists_above_thirty(const cyclist* list, const int list_length);

int main(void){

    int /*i = 0,*/ list_length;

    cyclist *list_of_cyclists;

    list_of_cyclists = read_file_to_list(&list_length);

    /*for(i = 0; i < 790; i++){
        printf("%s %s | %d %s %s | %s %s\n", list_of_cyclists[i].race_name, list_of_cyclists[i].cyclist_name, list_of_cyclists[i].age, list_of_cyclists[i].team, list_of_cyclists[i].nationality, list_of_cyclists[i].placement, list_of_cyclists[i].time);
    }*/

    printf("%d\n", list_length);

    print_italian_cyclists_above_thirty(list_of_cyclists, list_length);

    free(list_of_cyclists);

    return 0;
}

void* read_file_to_list(int* list_length){

    int i = 0, scan_result;

    FILE* read_file_pointer = fopen("cykelloeb","r");

    cyclist *output_pointer_to_array_of_cyclists;

    output_pointer_to_array_of_cyclists = (cyclist *)malloc(TOTAL_NUMBER_OF_PARTICIPENTS * sizeof(cyclist));

    while(!feof(read_file_pointer)){
        scan_result = fscanf(read_file_pointer, " %[a-zA-Z] \"%[a-zA-Z-' ]\" | %d %[A-Z] %[A-Z] | %3[A-Z0-9] %[0-9:-]", output_pointer_to_array_of_cyclists[i].race_name, output_pointer_to_array_of_cyclists[i].cyclist_name, &output_pointer_to_array_of_cyclists[i].age, output_pointer_to_array_of_cyclists[i].team, output_pointer_to_array_of_cyclists[i].nationality, output_pointer_to_array_of_cyclists[i].placement, output_pointer_to_array_of_cyclists[i].time);
        i++;
    }
    if(scan_result < 1){
        printf("%d\n", scan_result);
    }
    *list_length = i;

    return output_pointer_to_array_of_cyclists;

    fclose(read_file_pointer);
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
    for(i = 0; i < j; i++){
        printf("%s %s | %d %s %s | %s %s\n", list_of_italian_cyclists_above_thirty[i].race_name, list_of_italian_cyclists_above_thirty[i].cyclist_name, list_of_italian_cyclists_above_thirty[i].age, list_of_italian_cyclists_above_thirty[i].team, list_of_italian_cyclists_above_thirty[i].nationality, list_of_italian_cyclists_above_thirty[i].placement, list_of_italian_cyclists_above_thirty[i].time);
    }
}
