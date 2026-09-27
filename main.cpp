#include <iostream>
#include <algorithm>
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
#include "admin.h"
#include "collection.h"
#include "algs.h"
#include "exceptions.h"

using namespace std;

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

void printMenu() {
    cout << "\n========== МЕНЮ ==========\n";
    cout << "ЛР 6:\n";
    cout << "1. InvalidDataException\n";
    cout << "2. LimitExceededException\n";
    cout << "3. DuplicateException\n";
    cout << "4. ObjectNotFoundException\n";
    cout << "5. OutOfRangeException\n";
    cout << "6. InvalidOperationException\n";
    cout << "7. RelationException\n";;
    cout << "0. Выход\n";
    cout << "===========================\n";
}


int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int choice;

    do {
        printMenu();
        choice = inputInt("Выберите пункт меню: ");

        switch (choice) {
            case 1: {
                try {
                    auto faculty = make_shared<Faculty>("ФКСиС", 150);
                    auto stud = make_shared<Student>("", faculty, "56", "5505051", 67);
                } catch (const InvalidDataException& e) {
                    cout << e.what() << '\n';
                }
                break;
            }
            case 2: {
                try {
                    auto faculty = make_shared<Faculty>("ФКСиС", 150);
                    auto stud = make_shared<Student>("A", faculty, "55830038", "5505051", 67);
                    stud->setHours(100);
                } catch (const LimitExceededException& e) {
                    cout << e.what() << '\n';
                }
                break;
            }
            case 3: {
                try {
                    auto faculty = make_shared<Faculty>("ФКСиС", 100);
                    auto s1 = make_shared<Student>("Puto", faculty, "55", "551", 20);
                    auto s2 = make_shared<Student>("Vaflya", faculty, "55", "551", 20);
                    cout << *s1 << '\n' << *s2 << '\n';
                    *faculty += s1;
                    *faculty += s2;
                } catch (const DuplicateIdException& e) {
                    cout << e.what() << '\n';
                }
                break;
            }
            case 4: {
                try {
                    Collection<UniversityMember> empty;
                    empty.print(cout);
                    auto f = empty.find([](const UniversityMember& a) {
                        return a.getFullName() == "A";
                    });
                } catch (const ObjectNotFoundException& e) {
                    cout << e.what() << '\n';
                }
                break;
            }
            case 5: {
                try {
                    Collection<Subject> subs;
                    subs.add(make_shared<Subject>("ABAB", 20, EXAM));
                    auto s = subs.get(10);
                } catch (const OutOfRangeException& e) {
                    cout << e.what() << '\n';
                }
                break;
            }
            case 6: {
                try {
                    auto subj = make_shared<Subject>("LALA", 10, CREDIT);
                    subj->setControlType(static_cast<ControlType>(999));
                } catch (const InvalidOperationException& e) {
                    cout << e.what() << '\n';
                }
                break;
            }
            case 7: {
                try {
                    auto faculty = make_shared<Faculty>("SIS", 20);
                    auto dept = make_shared<Department>("FA", faculty);
                    auto subj = make_shared<Subject>("JAVA", 150, EXAM);
                    auto t = make_shared<Teacher>(1, "Puto", faculty, dept, subj, 300);
                    dept->addTeacher(t);
                    dept->printDepartmentInformation();
                    *faculty += dept;
                    *faculty -= dept;
                } catch (const RelationException& e) {
                    cout << e.what() << '\n';
                }
                break;
            }
        }
    } while (choice != 0);
}