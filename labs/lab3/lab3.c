#include "lab3.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

unsigned long int g_Id = 1;

int max_id(University *university) {
    int max = 0;
    for (int i = 0; i < university->groupsCount; i++) {
        for (int j = 0; j < university->groups[i].studentsCount; j++) {
            if (university->groups[i].students[j].id > max) {
                max = university->groups[i].students[j].id;
            }
        }
    }
    return max;
}

University *initUniversity(const char *fileName) {
    University *university = (University *) malloc(sizeof(University));
    if (university == NULL) {
        return NULL;
    }
    university->groups = NULL;
    university->groupsCount = 0;

    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        return university;
    }

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
            free(group_tmp);

            group = &university->groups[university->groupsCount - 1];
        }
        if (!addNewStudent(group, *student)) {
            continue;
        }
        free(student);
    }
    fclose(file);
    g_Id = max_id(university) + 1;
    return university;
}


void sort_students_by_surname(Group *group) {
    for (int i = (int) group->studentsCount - 1; i > 0; i--) {
        if (strcasecmp(group->students[i].surname, group->students[i - 1].surname) < 0) {
            Student tmp = group->students[i];
            group->students[i] = group->students[i - 1];
            group->students[i - 1] = tmp;
        } else {
            break;
        }
    }
}

bool addNewGroup(University *university, const Group group) {
    if (university == NULL) {
        return false;
    }

    Group *group_tmp = NULL;
    if (university->groupsCount == 0) {
        group_tmp = (Group *) malloc(sizeof(Group));
    } else {
        group_tmp = (Group *) realloc(university->groups, (university->groupsCount + 1) * sizeof(Group));
    }

    if (group_tmp == NULL) {
        return false;
    }

    university->groups = group_tmp;
    university->groups[university->groupsCount] = group;
    university->groupsCount++;

    return true;
}

bool addNewStudent(Group *group, Student student) {
    if (group == NULL) {
        return false;
    }

    if (strcmp(group->name, student.groupName) != 0) {
        return false;
    }

    Student *student_tmp = NULL;
    if (group->studentsCount == 0) {
        student_tmp = (Student *) malloc(sizeof(Student));
    } else {
        student_tmp = (Student *) realloc(group->students, sizeof(Student) * (group->studentsCount + 1));
    }
    if (student_tmp == NULL) {
        return false;
    }

    group->students = student_tmp;

    if (student.id == 0) {
        student.id = g_Id;
    }
    group->students[group->studentsCount] = student;
    group->studentsCount++;

    sort_students_by_surname(group);

    g_Id = student.id > g_Id ? student.id + 1 : g_Id + 1;

    return true;
}


bool removeGroup(University *university, const char *name) {
    if (university == NULL || name == NULL) {
        return false;
    }

    for (int i = 0; i < university->groupsCount; i++) {
        if (strcmp(university->groups[i].name, name) == 0) {
            free(university->groups[i].students);
            memmove(
                    &university->groups[i],
                    &university->groups[i + 1],
                    (university->groupsCount - i - 1) * sizeof(Group)
            );
            university->groupsCount--;
            return true;
        }
    }
    return false;
}

bool removeStudent(University *university, const unsigned long id) {
    if (university == NULL) {
        return false;
    }
    for (int i = 0; i < university->groupsCount; i++) {
        for (int j = 0; j < university->groups[i].studentsCount; j++) {
            if (university->groups[i].students[j].id == id) {
                memmove(
                        &university->groups[i].students[j],
                        &university->groups[i].students[j + 1],
                        (university->groups[i].studentsCount - j - 1) * sizeof(Student)
                );
                university->groups[i].studentsCount--;
                return true;
            }
        }
    }
    return false;
}


Group *getGroup(const University *university, const char *name) {
    if (university == NULL || name == NULL) {
        return NULL;
    }

    for (int i = 0; i < university->groupsCount; i++) {
        if (strcmp(university->groups[i].name, name) == 0) {
            return &university->groups[i];
        }
    }
    return NULL;
}


Student *getStudent(const University *university, const unsigned long id) {
    if (university == NULL) {
        return NULL;
    }

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
    if (university == NULL) {
        return;
    }
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
    if (group.studentsCount == 0) {
        printf("Group is empty\n");
    }
}

void printStudent(const Student student) {
    printf("\t%lu\n", student.id);
    printf("\t\t%s %s\n", student.surname, student.name);
    printf("\t\t%d\n", student.birthYear);
}


void freeUniversity(University *university) {
    if (university == NULL) {
        return;
    }
    for (int i = 0; i < university->groupsCount; i++) {
        free(university->groups[i].students);
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