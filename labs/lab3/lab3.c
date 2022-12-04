#include "lab3.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

unsigned long int g_Id = 1;

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

    Group *group = NULL;
    Student *student = NULL;
    Group *group_tmp = NULL;

    while (!feof(file)) {
        student = (Student *) malloc(sizeof(Student));
        fread(student, sizeof(Student), 1, file);
        if (feof(file)) {
            free(student);
            break;
        }
        group = getGroup(university, student->groupName);
        if (group == NULL) {
            group_tmp = (Group *) malloc(sizeof(Group));
            strcpy(group_tmp->name, student->groupName);
            group_tmp->students = NULL;
            group_tmp->studentsCount = 0;
            if (!addNewGroup(university, *group_tmp)) {
                continue;
            }
            group = &university->groups[university->groupsCount - 1];
        }
        if (!addNewStudent(group, *student)) {
            continue;
        }
    }
    fclose(file);
    return university;
}

bool addNewGroup(University *university, const Group group) {
    if (university == NULL) {
        return false;
    }
    if (university->groupsCount == 0) {
        university->groups = (Group *) malloc(sizeof(Group));
    } else {
        university->groups = (Group *) realloc(university->groups, sizeof(Group) * (university->groupsCount + 1));
    }
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
    if (group->studentsCount == 0) {
        group->students = (Student *) malloc(sizeof(Student));
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


char *strip(char *str) {
    char *end;
    while (isspace((unsigned char) *str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) end--;
    end[1] = '\0';
    return str;
}

bool removeGroup(University *university, const char *name) {
    for (int i = 0; i < university->groupsCount; i++) {
        if (strcmp(strip(university->groups[i].name), strip((char *) name)) == 0) {
            free(university->groups[i].students);
            free(&university->groups[i]);
            for (int j = i; j < university->groupsCount - 1; j++) {
                university->groups[j] = university->groups[j + 1];
            }
            university->groupsCount--;
            return true;
        }
    }
    return false;
}

bool removeStudent(University *university, const unsigned long id) {
    for (int i = 0; i < university->groupsCount; i++) {
        for (int j = 0; j < university->groups[i].studentsCount; j++) {
            if (university->groups[i].students[j].id == id) {
                for (int k = j; k < university->groups[i].studentsCount; k++) {
                    university->groups[i].students[k] = university->groups[i].students[k + 1];
                }
                university->groups[i].studentsCount--;
                return true;
            }
        }
    }
    return false;
}


Group *getGroup(const University *university, const char *name) {
    for (int i = 0; i < university->groupsCount; i++) {
        if (strcmp(strip(university->groups[i].name), strip((char *) name)) == 0) {
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
        printf("%s\n", university->groups[i].name);
        printGroup(university->groups[i]);
        printf("------------------------------------------------------------------------------\n");
    }
    if (university->groupsCount == 0) {
        printf("University is empty\n");
    }
}


void printGroup(const Group group) {
    for (int i = 0; i < group.studentsCount; i++) {
        printStudent(group.students[i]);
    }

}

void printStudent(const Student student) {
    printf("%lu\n", student.id);
    printf("\t%s %s\n", student.name, student.surname);
    printf("\t%d\n", student.birthYear);
}


void freeGroup(Group *group) {
    free(group->students);
//    free(group);
}

void freeUniversity(University *university) {
    for (int i = 0; i < university->groupsCount; i++) {
        freeGroup(&university->groups[i]);
    }
    free(university->groups);
    free(university);
}

bool saveToFile(const char *fileName, const University *university) {
    FILE *file = fopen(fileName, "w+b");
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
