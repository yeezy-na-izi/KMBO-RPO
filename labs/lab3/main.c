#include "lab3.h"
#include <stdio.h>
#include <string.h>


typedef enum {
    E_INVALID = 0,
    E_ADD_GROUP,
    E_ADD_STUDENT,
    E_DEL_GROUP,
    E_DEL_STUDENT,
    E_PRINT_UNI,
    E_PRINT_GROUP,
    E_PRINT_STUDENT,
    E_END
} EAction;

const char *Enum2Str(EAction eAct) {
    switch (eAct) {
        case E_INVALID:
            return "- Невалидная команда";
        case E_ADD_GROUP:
            return "- Добавить группу";
        case E_ADD_STUDENT:
            return "- Добавить студента";
        case E_DEL_GROUP:
            return "- Удалить группу";
        case E_DEL_STUDENT:
            return "- Удалить студента";
        case E_PRINT_UNI:
            return "- Напечатать информацию об университете";
        case E_PRINT_GROUP:
            return "- Напечатать информацию о группе";
        case E_PRINT_STUDENT:
            return "- Напечатать информацию о студенте";
        case E_END:
            return "- Окончание работы";
        default:
            return "? - Невалидная команда";
    }
}

Group *add_group_task(University *university) {
    Group *group = NULL;
    char groupName[SIZE];

    printf("Введите название группы:\n");
    scanf("%s", groupName);
    group = (Group *) malloc(sizeof(Group));
    strcpy(group->name, groupName);
    addNewGroup(university, *group);
    return group;
}

void add_student_task(University *university) {
    Student *student = NULL;
    int studentGroupId = 0;
    int flag = 0;
    Group *group = NULL;
    student = (Student *) malloc(sizeof(Student));


    printf("Введите имя студента:\n");
    scanf("%s", student->name);

    printf("Введите фамилию студента:\n");
    scanf("%s", student->surname);

    printf("Введите год рождения студента\n");
    scanf("%d", &student->birthYear);

    printf("Выберете группу студента:\n");
    for (int i = 0; i < university->groupsCount; i++) {
        printf("%d %s\n", i + 1, university->groups[i].name);
    }
    printf("%d %s\n", university->groupsCount + 1, "Новая группа");
    scanf("%d", &studentGroupId);

    if (studentGroupId == university->groupsCount + 1) {
        group = add_group_task(university);
        flag = 1;
    } else {
        group = &university->groups[studentGroupId - 1];
    }
    addNewStudent(group, *student);
    free(student);
    if (flag) {
        free(group);
    }
}

void del_group_task(University *university) {
    char groupName[SIZE];
    memset(groupName, 0, SIZE);

    printf("Введите название группы которую нужно удалить:\n");
    scanf("%s", groupName);
    if (removeGroup(university, groupName)) {
        printf("Группа %s успешно удалена\n", groupName);
    } else {
        printf("Группа %s не найдена\n", groupName);
    }
}

void del_student_task(University *university) {
    int studentId = 0;

    printf("Введите id студента которого нужно удалить: \n");
    scanf("%d", &studentId);
    if (removeStudent(university, studentId)) {
        printf("Студент с id %d удален\n", studentId);
    } else {
        printf("Студент с id %d не найден\n", studentId);
    }
}


void print_university_task(University *university) {
    printUniversity(university);
}

void print_group_task(University *university) {
    char groupName[SIZE];
    memset(groupName, 0, SIZE);
    Group *group = NULL;


    printf("Введите название группы:\n");
    scanf("%s", groupName);
    group = getGroup(university, groupName);
    if (group == NULL) {
        printf("Группа не найдена\n");
    } else {
        printGroup(*group);
    }
}

void print_student_task(University *university) {
    int studentId = 0;
    Student *student = NULL;

    printf("Введите id студента:\n");
    scanf("%d", &studentId);
    student = getStudent(university, studentId);
    if (student == NULL) {
        printf("Студент не найден\n");
    } else {
        printStudent(*student);
    }
}

EAction get_action() {
    int beforeEAction = 0;
    EAction eAction;

    printf("Выберите действие:\n");
    for (int i = 1; i <= E_END; i++) {
        printf("%d %s\n", i, Enum2Str((EAction) i));
    }
    scanf("%d", &beforeEAction);
    if (beforeEAction < 1 || beforeEAction > E_END) {
        eAction = E_INVALID;
    } else {
        eAction = (EAction) beforeEAction;
    }
    return eAction;
}

int main(int argc, char *argv[]) {
    char *fileName = NULL;
    size_t fileNameSize = 0;
    if (argc < 2) {
        printf("Имя файла не задано\n");
        printf("Введите имя файла:\n");
        getline(&fileName, &fileNameSize, stdin);
        fileName[strlen(fileName) - 1] = '\0';
    } else {
        fileName = argv[1];
    }

    University *university = initUniversity(fileName);
    Group *group = NULL;
    EAction eAction = E_INVALID;

    while (eAction != E_END) {
        eAction = get_action();
        switch (eAction) {
            case E_ADD_GROUP:
                group = add_group_task(university);
                free(group);
                Group *group = NULL;
                continue;
            case E_ADD_STUDENT:
                add_student_task(university);
                continue;
            case E_DEL_GROUP:
                del_group_task(university);
                continue;
            case E_DEL_STUDENT:
                del_student_task(university);
                continue;
            case E_PRINT_UNI:
                print_university_task(university);
                continue;
            case E_PRINT_GROUP:
                print_group_task(university);
                continue;
            case E_PRINT_STUDENT:
                print_student_task(university);
                continue;
            case E_INVALID:
                printf("Невалидная команда\n");
                continue;
            case E_END:
                break;
        }
    }

    printf("Работа завершена\n");
    printf("Введите имя файла для сохранения:\n");
    fflush(stdin);
    getline(&fileName, &fileNameSize, stdin);
    printf("Сохранение в файл %s\n", fileName);
    saveToFile(fileName, university);
    freeUniversity(university);
    free(fileName);

    return 0;
}
