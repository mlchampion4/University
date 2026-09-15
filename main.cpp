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
    cout << "(Ввод-вывод осуществяется кастомными операторами)\n";
    cout << "1. Оператор равенства студентов по студ.билету\n";
    cout << "2. Оператор неравенства студентов по студ.билету\n";
    cout << "3. Операторы сравнения студентов по среднему баллу\n";
    cout << "4. Оператор += (зачислить студента на факультет, с ограничением на число студентов на факльтет)\n";
    cout << "5. Оператор += (добавить кафедру на факультет)\n";
    cout << "6. Оператор -= (отчислить студента с факультета)\n";
    cout << "7. Оператор -= (удалить кафедру с факультета)\n";
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
    int choice;
    bool eq;
    auto faculty = make_shared<Faculty>("ФКСиС", 4);

    auto stud1 = Student("Драбудько Никита Геннадьевич", "55830038", "550501", faculty, 50);
    stud1.setMarks(vector<unsigned int> {10, 9, 8, 9});
    auto stud2 = Student("Драбудько Никита Геннадьевич", "55830038", "550501", faculty, 20);
    stud2.setMarks(vector<unsigned int> {10, 9, 8, 9});
    auto stud3 = Student("Безруких Тимофей Игоревич", "55830012", "550501", faculty, 50);
    stud3.setMarks(vector<unsigned int> {9, 5, 7, 4});
    auto ne_stud = Student();
    ne_stud.setFaculty(faculty);
    ne_stud.setMarks(vector<unsigned int> {3, 4, 5, 6});

    auto dept = make_shared<Department>("Кафедра ЭВМ", faculty);

    do {
        printMenu();
        choice = inputInt("Выберите пункт меню: ");

        switch (choice) {
            case 1:
                cout << stud1 << endl;
                cout << stud2 << endl;
                eq = stud1 == stud2;
                cout << eq;
                break;
            case 2:
                cin >> ne_stud;
                cout << ne_stud;
                cout << stud1;
                eq = stud1 != ne_stud;
                cout << eq;
                break;
            case 3:
                cout << stud1 << endl;
                cout << stud3 << endl;
                cout << stud2 << "(для нестрогих сравнений в случае равенства)" << endl;
                eq = stud1 > stud3;
                cout << "Оператор >: " << eq << endl;
                eq = stud1 < stud3;
                cout << "Оператор <: " << eq << endl;
                eq = stud1 >= stud2;
                cout << "Оператор >=: " << eq << endl;
                eq = stud1 <= stud3;
                cout << "Оператор <=: " << eq << endl;
                break;
            case 4:
                *faculty += make_shared<Student>(stud1);
                *faculty += make_shared<Student>(stud2);
                *faculty += make_shared<Student>(stud3);
                *faculty += make_shared<Student>(ne_stud);
                faculty->printFacultyInformafion();
                cout << "Добавление студента в полный факультет: \n";
                *faculty += make_shared<Student>("Бубылда", "55830001", "550505", faculty, 2);
                faculty->printFacultyInformafion();
                break;
            case 5:
                dept->printDepartmentInformation();
                *faculty += dept;
                cout << "Кафедра успешно добавлена! \n";
                faculty->printFacultyInformafion();
                break;
            case 6:
                *faculty -= make_shared<Student>(ne_stud);
                cout << "Студент отчислен \n";
                faculty->printFacultyInformafion();
                break;
            case 7:
                *faculty -= dept;
                cout << "Кафедра успешно удалена \n";
                faculty->printFacultyInformafion();
                break;
        }
    } while (choice != 0);
}