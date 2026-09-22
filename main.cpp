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
    cout << "ЛР 3:\n";
    cout << "1. Показать ФИО и факультет (только унаслед. поля)\n";
    cout << "2. Показать спец. поля производных типов\n";
    cout << "3. Унаследованный сеттер\n";
    cout << "4. Унаследованный геттер\n";
    cout << "ЛР 4:\n";
    cout << "5. Полиморфный подсчет метрик для коллекции объектов UniversityMember\n";
    cout << "6. Поиск самого результативного member'а\n";
    cout << "7. Массовое действие для всей коллекции\n";
    cout << "0. Выход\n";
    cout << "===========================\n";
}


int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int choice;
    auto faculty = make_shared<Faculty>("ФКСиС", 150);
    auto newFac = make_shared<Faculty>("ФИТУ", 100);
    auto dept = make_shared<Department>("Кафедра ЭВМ", faculty);
    auto subj = make_shared<Subject>("ПнаЯВУ", 100, CREDIT);
    vector<unique_ptr<UniversityMember>> members;
    members.push_back(make_unique<Student>("Драбудьно Н.Г.", faculty, "55830038", "550501", 30));
    members.push_back(make_unique<Teacher>(1, "Скиба И.Г.", faculty, dept, subj, 50));
    members.push_back(make_unique<Administrator>(1, "Никульшин Б.В.", faculty, "Зав. кафедрой ЭВМ", 4));

    do {
        printMenu();
        choice = inputInt("Выберите пункт меню: ");

        switch (choice) {
            case 1:
                cout << "ФИО и факультет: \n";
                for (const auto& m : members) {
                    cout << m->getFullName() << '\n';
                    cout << m->getFaculty().lock()->getFacultyName() << '\n';
                }
                break;
            case 2:
                cout << "Спец. поля производных типов:\n";
                for (const auto& m : members) {
                    cout << "--- " << m->getType() << " ---\n";
        
                    if (auto* student = dynamic_cast<Student*>(m.get())) {
                        cout << "  Номер билета: " << student->getStudentNumber() << '\n';
                        cout << "  Группа: " << student->getGroupNumber() << '\n';
                        cout << "  Макс. часов/нед.: " << student->getMaxHoursPerWeek() << '\n';
                        cout << "  Оценки: ";
                        for (auto mark : student->getMarks()) cout << mark << " ";
                            cout << '\n';
                    }

                    else if (auto* teacher = dynamic_cast<Teacher*>(m.get())) {
                        cout << "  Предмет: " << teacher->getSubject()->getSubjectName() << '\n';
                        cout << "  Макс. нагрузка: " << teacher->getMaxTeachingLoad() << '\n';
                    }

                    else if (auto* admin = dynamic_cast<Administrator*>(m.get())) {
                        cout << "  Должность: " << admin->getPosition() << '\n';
                        cout << "  Подчинённых: " << admin->getManagedPeople() << '\n';
                    }
                }
                break;
            case 3:
                cout << "Унаследованный сеттер:\n";

                members[0]->setFaculty(newFac);
                members[1]->setFaculty(newFac);
                members[2]->setFaculty(newFac);

                for (const auto& m : members) {
                    cout << *m << '\n';
                }
                break;
            case 4:
                for (const auto& m : members) {
                    cout << m->getFullName() << "  " 
                         << m->getFaculty().lock()->getFacultyName() << '\n';
                }
                break;
            case 5:
                for (const auto& m : members) {
                    cout << "--- " << m->getType() << " ---\n";
                    cout << m->getFullName() << '\n';
                    cout << m->getMetricName() << ": " << m->calculateMetric() << '\n';
                }
                break;
            case 6: {
                cout << "Самый результативный member:\n";
                auto it = std::max_element(
                    members.begin(),
                    members.end(),
                    [](const unique_ptr<UniversityMember>& a,
                       const unique_ptr<UniversityMember>& b) {
                        return a->calculateMetric() < b->calculateMetric();
                       }
                );
                UniversityMember* best = it->get();

                cout << "Тип:     " << best->getType() << '\n';
                cout << "ФИО:     " << best->getFullName() << '\n';
                cout << "Метрика: " << best->calculateMetric() << '\n';
                break;
            }
            case 7:
                for (const auto& m : members) {
                    m->applyEffect(5);
                }
            break;
        }
    } while (choice != 0);
}