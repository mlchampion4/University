// main.cpp
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <locale>
#include <sstream>
#include "faculty.h"
#include "department.h"
#include "student.h"
#include "teacher.h"
#include "subject.h"

using namespace std;

void printMenu() {
    cout << "\n========== МЕНЮ ==========\n";
    cout << "1. Показать информацию о факультете\n";
    cout << "2. Показать кафедры\n";
    cout << "3. Показать студентов\n";
    cout << "4. Изменить данные студента\n";
    cout << "5. Изменить данные кафедры\n";
    cout << "6. Поставить занятие с преподавателем\n";
    cout << "0. Выход\n";
    cout << "===========================\n";
}

int inputInt(string_view message) {
    string input;
    int number;
    char extra;

    while (true) {
        cout << message.data();
        getline(cin, input);

        if (stringstream ss(input); ss >> number && !(ss >> extra)) {
            return number;
        }

        cout << "Ошибка! Введите целое число.\n";
    }
}


int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    auto faculty = make_shared<Faculty>("ФКСиС", 150);
    auto dept1 = make_shared<Department>("Кафедра ЭВМ", faculty);
    auto dept2 = make_shared<Department>("Кафедра ВМ", faculty);
    auto stud1 = make_shared<Student>("Драбудько Никита Геннадьевич", "550501", faculty, 50, 50);
    auto stud2 = make_shared<Student>("Иванов Иван Иванович", "550502", faculty, 10, 10);
    auto stud3 = make_shared<Student>("Петров Пётр Петрович", "550503", faculty, 2, 2);
    auto subject = make_shared<Subject>("ПнаЯВУ", 4, CREDIT);
    auto teacher = make_shared<Teacher>("Скиба Ирина Геннадьевна", faculty, dept1, subject);
    int choice;
    vector<shared_ptr<Student>> group = {stud1, stud2, stud3};

    dept1->addTeacher(teacher);

    faculty->addDepartment(dept1);
    faculty->addStudent(stud1);
    faculty->addStudent(stud2);
    faculty->addStudent(stud3);

    do {
        printMenu();
        choice = inputInt("Выберите пункт меню: ");

        switch (choice) {
            case 1:
                faculty->printFacultyInformafion();
                break;
            case 2:
                dept1->printDepartmentInformation();
                cout << "--------------------------" << endl;
                dept2->printDepartmentInformation();
                break;
            case 3:
                for (int i = 0; i < int(group.size()); i++) {
                    group[i]->printStudentInformation();
                    cout << "---------------------------" << endl;
                }
                break;
            case 4:
                stud2->setGroupNumber("450502");
                stud2->setMaxHoursPerWeek(20);
                stud2->printStudentInformation();
                break;
            case 5:
                dept2->setDepartmentName("Кафедра Физики");
                dept2->printDepartmentInformation();
                break;
            case 6:
                faculty->addLecture(group, dept1, "Скиба Ирина Геннадьевна");
        }
    } while (choice != 0);
}