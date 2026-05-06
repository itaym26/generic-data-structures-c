#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <file.h>
#include "dynamic_array.h"

#define OK 0
#define NULL_POINTER -1

typedef struct {
    char* name;
    long id;
} Person;

typedef struct arry_person {
    Person* people;
    int size;
} ap;

void swap_people(Person* a, Person* b) {
    Person temp = *a;
    *a = *b;
    *b = temp;
}

int sort_people_by_id(ap* people_array) {
    if (people_array == NULL || people_array->people == NULL) {
        return NULL_POINTER;
    }
    
    for (int i = 0; i < people_array->size - 1; i++) {
        for (int j = 0; j < people_array->size - i - 1; j++) {
            if (people_array->people[j].id > people_array->people[j + 1].id) {
                swap_people(&people_array->people[j], &people_array->people[j + 1]);
            }
        }
    }
    
    return OK;
}

int sort_people_by_name(ap* people_array) {
    if (people_array == NULL || people_array->people == NULL) {
        return NULL_POINTER;
    }
    
    for (int i = 0; i < people_array->size - 1; i++) {
        for (int j = 0; j < people_array->size - i - 1; j++) {
            if (strcmp(people_array->people[j].name, people_array->people[j + 1].name) > 0) {
                swap_people(&people_array->people[j], &people_array->people[j + 1]);
            }
        }
    }
    
    return OK;
}
int main() {
    //creating an array of people and sorting them by name and ID
    Person p_array[] = {
        {"Charlie", 333},
        {"Alice", 111},
        {"Bob", 222}
    };
    ap my_people = {p_array, 3};

    sort_people_by_name(&my_people);
    printf("People sorted by name:\n");
    for (int i = 0; i < my_people.size; i++) {
        printf("%s - %ld\n", my_people.people[i].name, my_people.people[i].id);
    }
    sort_people_by_id(&my_people);
    printf("People sorted by ID:\n");
    for (int i = 0; i < my_people.size; i++) {
        printf("%s - %ld\n", my_people.people[i].name, my_people.people[i].id);
    }

   FILE* file = fopen("people.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    for (int i = 0; i < my_people.size; i++) {
        fprintf(file, "%s - %ld\n", my_people.people[i].name, my_people.people[i].id);
    }
    fclose(file);

    Person loded_people[3];
    file = fopen("people.txt", "r");    
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    for (int i = 0; i < 3; i++) {
        fscanf(file, "%s - %ld", loded_people[i].name, &loded_people[i].id);
    }
    fclose(file);
    
    for (int i = 0; i < 3; i++) {
        printf("Loaded person from text: %s - %ld\n", loded_people[i].name, loded_people[i].id);
    }

    file = fopen("people.bin", "wb"); 
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    fwrite(my_people.people, sizeof(Person), my_people.size, file);
    fclose(file);

    Person loaded_people1[3];
    file = fopen("people.bin", "rb"); 
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }   
    fread(loaded_people1, sizeof(Person), my_people.size, file);
    fclose(file);

    for (int i = 0; i < 3; i++) {
        printf("Loaded person from binary file: %s - %ld\n", loaded_people1[i].name, loaded_people1[i].id);
    }
    FILE* file = fopen("people.bin", "r+b");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    FILE* file = fopen("people.bin", "rb");
    fseek(file, 2*sizeof(Person), SEEK_SET);
    Person third_person;
    fread(&third_person, sizeof(Person), 1, file);
    fclose(file);
    printf("Third person from binary file: %s - %ld\n", third_person.name, third_person.id);

    fseek(file, 2 * sizeof(Person), SEEK_SET);

    Person new_person;
    strcpy(new_person.name, "David"); 
    new_person.id = 999;              

    fwrite(&new_person, sizeof(Person), 1, file);
    fclose(file);

    printf("Successfully updated the 3rd person in the binary file!\n");

    // 1. Create a dynamic array with an initial size of 2 and a block size of 2
    darr *myArray = DynamicArrayCreate(2, 2);
    if (myArray == NULL) {
        return 1; // Failed to create the dynamic array
    }
    
    // 2. Insert 3 elements (the third one will trigger a resize)
    DynamicArrayInsert(myArray, 10);
    DynamicArrayInsert(myArray, 20);
    DynamicArrayInsert(myArray, 30); 

    // 3. Print the array contents
    printf("Array after insertions: ");
    for (int i = 0; i < myArray->NumOfElements; i++) {
        printf("%d ", myArray->dArray[i]);
    }
    printf("\n");

    // 4. Delete the last element
    int deletedValue;
    if (DynamicArrayDelete(myArray, &deletedValue) == OK) {
        printf("Deleted value: %d\n", deletedValue);
    }

    // 5. Free the memory
    DynamicArrayDestroy(myArray);

    return 0;
}