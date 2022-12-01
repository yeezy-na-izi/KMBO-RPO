#include "lab3.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


University *initUniversity(const char *fileName) {
    University *university = (University *) malloc(sizeof(University));
    if (university == NULL) {
        return NULL;
    }
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        return university;
    }
    university->groups = NULL;
    university->groupsCount = 0;
    while (!feof(file)) {
        Student *student = (Student *) malloc(sizeof(Student));
        fread(student, sizeof(Student), 1, file);
        if (feof(file)) {
            free(student);
            break;
        }
        Group *group = getGroup(university, student->groupName);
        if (group == NULL) {
            group = (Group *) malloc(sizeof(Group));
            strcpy(group->name, student->groupName);
            group->students = NULL;
            group->studentsCount = 0;
            if (!addNewGroup(university, *group)) {
                continue;
            }
        }
        if (!addNewStudent(group, *student)) {
            continue;
        }
    }
    fclose(file);
    return university;
}

bool addNewGroup(University *university, const Group group) {
    printf("addNewGroup: %s\n", group.name);
    if (university == NULL) {
        return false;
    }
    university->groups = (Group **) realloc(university->groups, sizeof(Group *) * (university->groupsCount + 1));
    if (university->groups == NULL) {
        return false;
    }
    university->groups[university->groupsCount] = group;
    university->groupsCount++;
    return true;
}

bool addNewStudent(Group *group, Student student) {
    if (group == NULL) {
        return false;
    }
    if (group->students == NULL) {
        group->students = (Student *) malloc(sizeof(Student));
        group->studentsCount = 0;
    } else {
        group->students = (Student *) realloc(group->students, sizeof(Student) * (group->studentsCount + 1));
    }
    if (group->students == NULL) {
        return false;
    }
    if (student.id == 0) {
        student.id = g_Id;
    }
    group->students[group->studentsCount] = student;
    group->studentsCount++;
    g_Id++;
    return true;
}

bool removeGroup(University *university, const char *name) {
    for (int i = 0; i < university->groupsCount; i++) {
        if (strcmp(university->groups[i].name, name) == 0) {
            free(university->groups[i].students);
            free(&university->groups[i]);
            for (int j = i; j < university->groupsCount - 1; j++) {
                university->groups[j] = university->groups[j + 1];
            }
            university->groupsCount--;
            return true;
        }
    }
}

bool removeStudent(University *university, const unsigned long id) {
    for (int i = 0; i < university->groupsCount; i++) {
        for (int j = 0; j < university->groups[i].studentsCount; j++) {
            if (university->groups[i].students[j].id == id) {
                free(&university->groups[i].students[j]);
                university->groups[i].studentsCount--;
                for (int k = j; k < university->groups[i].studentsCount; k++) {
                    university->groups[i].students[k] = university->groups[i].students[k + 1];
                }
                return true;
            }
        }
    }
    return false;
}

Group *getGroup(const University *university, const char *name) {
    for (int i = 0; i < university->groupsCount; i++) {
        if (strcmp(university->groups[i].name, name) == 0) {
            return &university->groups[i];
        }
    }
    return NULL;
}

Student *getStudent(const University *university, const unsigned long id) {
    for (int i = 0; i < university->groupsCount; i++) {
        for (int j = 0; j < university->groups[i].studentsCount; j++) {
            if (university->groups[i].students[j].id == id) {
                return &university->groups[i].students[j];
            }
        }
    }
    return NULL;

}

void printUniversity(const University *university) {
    for (int i = 0; i < university->groupsCount; i++) {
        printGroup(university->groups[i]);
        printf("------------------------------------------------------------------------------\n");
    }
    if (university->groupsCount == 0) {
        printf("University is empty\n");
    }
}


void printGroup(const Group group) {
    printf("%s\n", group.name);
    for (int i = 0; i < group.studentsCount; i++) {
        printStudent(group.students[i]);
    }

}

void printStudent(const Student student) {
    printf("%lu\n", student.id);
    printf("\t%s %s\n", student.name, student.surname);
    printf("\t%d\n", student.birthYear);
}

void freeUniversity(University *university) {
    for (int i = 0; i < university->groupsCount; i++) {
        free(university->groups[i].students);
    }
    free(university->groups);
    free(university);

}

bool saveToFile(const char *fileName, const University *university) {
    FILE *file = fopen(fileName, "wb");
    if (file == NULL) {
        return false;
    }
    for (int i = 0; i < university->groupsCount; i++) {
        for (int j = 0; j < university->groups[i].studentsCount; j++) {
            fwrite(&university->groups[i].students[j], sizeof(Student), 1, file);
        }
    }
    fclose(file);
    return true;
}

int main() {
    char *fileName = "/Users/yeezy_na_izi/CLionProjects/FirstSem/labs/lab3/kmbo22.dat";
    University *university = initUniversity(fileName);
    if (university == NULL) {
        printf("Error: %s not found", fileName);
        return 1;
    }
    printUniversity(university);
}