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
#include "files.h"
#include "analytics.h"

using namespace std;

UniversityData globalData;

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

string inputString(string_view message) {
    string input;
    cout << message.data();
    getline(cin, input);
    return input;
}

void createTestSystem() {
    globalData.faculties.clear();
    globalData.departments.clear();
    globalData.subjects.clear();
    globalData.members.clear();

    auto f1 = make_shared<Faculty>("ФКСиС", 100);
    auto f2 = make_shared<Faculty>("ФЭМ", 50);
    globalData.faculties.push_back(f1);
    globalData.faculties.push_back(f2);

    auto subj1 = make_shared<Subject>("ОАиП", 120, EXAM);
    auto subj2 = make_shared<Subject>("Математика", 100, CREDIT);
    globalData.subjects.push_back(subj1);
    globalData.subjects.push_back(subj2);

    auto d1 = make_shared<Department>("Кафедра ИТ", f1);
    auto d2 = make_shared<Department>("Кафедра Высшей математики", f2);
    globalData.departments.push_back(d1);
    globalData.departments.push_back(d2);

    *f1 += d1;
    *f2 += d2;

    auto s1 = make_shared<Student>("Иванов И.И.", f1, "ST001", "Гр-1", 30);
    s1->setMarks({9, 8, 10});
    auto s2 = make_shared<Student>("Безруких Т.Н.", f1, "ST006", "Гр-1", 30);
    s2->setMarks({6, 8, 7});

    auto s3 = make_shared<Student>("Петров П.П.", f1, "ST002", "Гр-2", 30);
    s3->setMarks({7, 6});
    auto s4 = make_shared<Student>("Драбудько Н.г.", f1, "ST003", "Гр-2", 30);
    s4->setMarks({4, 10, 7});

    globalData.members.add(s1);
    globalData.members.add(s2);
    globalData.members.add(s3);
    globalData.members.add(s4);

    *f1 += s1;
    *f1 += s2;
    *f1 += s3;
    *f1 += s4;

    auto t1 = make_shared<Teacher>(1, "Сидоров С.С.", f1, d1, subj2, 200);
    t1->applyEffect(50);

    auto t2 = make_shared<Teacher>(2, "Скиба И.Г.", f1, d1, subj1, 150);
    t2->applyEffect(110);

    auto t3 = make_shared<Teacher>(3, "Ковальчук А.М.", f1, d1, subj1, 250);
    t3->applyEffect(200);

    globalData.members.add(t1);
    globalData.members.add(t2);
    globalData.members.add(t3);
    d1->addTeacher(t1);
    d1->addTeacher(t2);
    d1->addTeacher(t3);

    auto a1 = make_shared<Administrator>(1, "Смирнова А.А.", f1, "Декан", 5);
    globalData.members.add(a1);

    auto a2 = make_shared<Administrator>(2, "Никульшин Б.В.", f1, "Зав.Кафедрой", 10);
    globalData.members.add(a2);

    cout << "Тестовая система создана. Объектов: "
         << globalData.faculties.size() << " фак., "
         << globalData.members.size() << " чел.\n";
}

void printCurrentState() {
    cout << "\n--- ТЕКУЩЕЕ СОСТОЯНИЕ СИСТЕМЫ ---\n";

    cout << "Факультеты:\n";
    for (const auto& f : globalData.faculties) {
        cout << " - " << f->getFacultyName()
             << " (макс. " << f->getMaxStudentsCount() << ")\n";
    }

    cout << "Кафедры:\n";
    for (const auto& d : globalData.departments) {
        cout << " - " << d->getDepartmentName()
             << " (" << d->getFaculty()->getFacultyName() << ")\n";
    }

    cout << "Предметы:\n";
    for (const auto& s : globalData.subjects) {
        cout << " - " << s->getSubjectName()
             << " (" << s->getHours() << " ч.)\n";
    }

    cout << "Участники:\n";
    for (const auto& m : globalData.members) {
        if (m) {
            cout << " - [" << m->getType() << "] " << m->getFullName();

            if (m->getType() == "Student") {
                auto s = dynamic_pointer_cast<Student>(m);
                cout << " (Номер билета: " << s->getStudentNumber() << ")";
            } else {
                cout << " (ID: " << m->getId() << ")";
            }
             cout << '\n';
        }
    }

    cout << "--------------------------------\n";
}

void printMenu() {
    cout << "\n========== МЕНЮ ==========\n";
    cout << "ЛР 7:\n";
    cout << "1. Сохранить состояние в файл\n";
    cout << "2. Загрузить состояние из файла\n";
    cout << "3. Добавить запись в журнал\n";
    cout << "4. Сформировать отчет\n";
    cout << "5. Создать тестовую систему\n";
    cout << "6. Вывод текущей системы\n";
    cout << "ЛР 8:\n";
    cout << "7. Группировка студентов по группам\n";
    cout << "8. Топ-N студентов по успеваемости\n";
    cout << "9. Подсчет преподавателей с определенной нагрузкой\n";
    cout << "10. Мин/Макс балл студентов\n";
    cout << "11. Поиск уникальных предметов у преподавателей\n";
    cout << "12. Посчитать среднюю нагрузку преподавателей\n";
    cout << "0. Выход\n";
    cout << "===========================\n";
}


int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int choice;
    string filename = "data.txt";
    string reportFilename = "report.txt";

    do {
        printMenu();
        choice = inputInt("Выберите пункт меню: ");

        switch (choice) {
            case 1:
                try {
                    StorageManager::saveState(filename, globalData);
                    StorageManager::logAction("journal.log", "Сохранение состояния в файл " + filename);
                } catch (const exception& e) {
                    cerr << "Ошибка сохранения: " << e.what() << '\n';
                }
                break;
            case 2:
                try {
                    StorageManager::loadState(filename, globalData);
                    StorageManager::logAction("journal.log", "Загрузка состояния из файла " + filename);
                } catch (const exception& e) {
                    cerr << "Ошибка загрузки: " << e.what() << '\n';
                }
                break;
            case 3: {
                string action = inputString("Введите текст для журнала: ");
                if (!action.empty()) {
                    StorageManager::logAction("journal.log", action);
                    cout << "Запись добавлена в journal.log\n";
                }
                break;
            }
            case 4:
                try {
                    StorageManager::generateReport(reportFilename, globalData);
                    StorageManager::logAction("journal.log", "Сформирован отчёт " + filename);
                } catch (const exception& e) {
                    cerr << "Ошибка создания отчёта: " << e.what() << '\n';
                }
                break;
            case 5:
                createTestSystem();
                break;
            case 6:
                printCurrentState();
                break;
            case 7:
                UniversityAnalytics::groupStudentsByGroup(globalData.members);
                break;
            case 8:
                UniversityAnalytics::findTopStudents(globalData.members, 3);
                break;
            case 9:
                UniversityAnalytics::countTeachersByLoad(globalData.members, 100);
                break;
            case 10:
                UniversityAnalytics::findMinMaxMetrics(globalData.members);
                break;
            case 11:
                UniversityAnalytics::listUniqueSubjects(globalData.members);
                break;
            case 12:
                UniversityAnalytics::calculateAverageLoad(globalData.members);
                break;
        }
    } while (choice != 0);
}