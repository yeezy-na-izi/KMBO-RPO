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

int main(int argc, char *argv[]) {
    char *fileName = "";
    size_t fileNameSize = 0;
    if (argc < 1) {
        getline(&fileName, &fileNameSize, stdin);
    } else {
        fileName = argv[1];
    }

    University *university = initUniversity(fileName);
    Group *group = NULL;
    Student *student = NULL;
    int studentId = 0;
    char *groupName = "";
    size_t groupNameSize = 0;

    int beforeEAction = 0;
    EAction eAction = E_INVALID;
    while (eAction != E_END) {
        printf("Выберите действие:\n");
        for (int i = 1; i <= E_END; i++) {
            printf("%d%s\n", i, Enum2Str((EAction) i));
        }
        scanf("%d", &beforeEAction);
        if (beforeEAction < 1 || beforeEAction > E_END) {
            eAction = E_INVALID;
        } else {
            eAction = (EAction) beforeEAction;
        }
        switch (eAction) {
            case E_ADD_GROUP:
                //
            case E_ADD_STUDENT:
                //
            case E_DEL_GROUP:
                printf("Введите название группы которую нужно удалить:\n");
                getline(&groupName, &groupNameSize, stdin);
                if (removeGroup(university, groupName)) {
                    printf("Группа %s успешно удалена\n", groupName);
                } else {
                    printf("Группа %s не найдена\n", groupName);
                }
            case E_DEL_STUDENT:
                printf("Введите id студента которого нужно удалить:\n");
                scanf("%d", &studentId);
                if (removeStudent(university, studentId)) {
                    printf("Студент с id %d удален\n", studentId);
                } else {
                    printf("Студент с id %d не найден\n", studentId);
                }
            case E_PRINT_UNI:
                printUniversity(university);
            case E_PRINT_GROUP:
                printf("Введите название группы:\n");
                getline(&groupName, &groupNameSize, stdin);
                group = getGroup(university, groupName);
                if (group == NULL) {
                    printf("Группа не найдена\n");
                } else {
                    printGroup(*group);
                }

            case E_PRINT_STUDENT:
                printf("Введите id студента:\n");
                scanf("%d", &studentId);
                student = getStudent(university, studentId);
                if (student == NULL) {
                    printf("Студент не найден\n");
                } else {
                    printStudent(*student);
                }
            case E_END:
                break;
        }
    }

    free(fileName);
    printf("Работа завершена\n");
    printf("Введите имя файла для сохранения:\n");
    getline(&fileName, &fileNameSize, stdin);
    saveToFile(fileName, university);
    free(fileName);
    freeUniversity(university);

    return 0;
}
