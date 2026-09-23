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
    cout << "ЛР 5:\n";
    cout << "8. Показать размер коллекции\n";
    cout << "9. Получить элемент по индексу\n";
    cout << "10. Удалить по индексу\n";
    cout << "11. Удалить по критерию\n";
    cout << "12. Поиск по критерию\n";
    cout << "13. Средняя метрика\n";
    cout << "14. Сортировка по условию\n";
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
    Collection<UniversityMember> members;
    members.add(make_shared<Student>("Драбудько Н.Г.", faculty, "55830038", "550501", 30));
    members.add(make_shared<Teacher>(1, "Скиба И.Г.", faculty, dept, subj, 50));
    members.add(make_shared<Administrator>(1, "Никульшин Б.В.", faculty, "Зав. кафедрой ЭВМ", 4));
    Collection<Subject> subjects;
    subjects.add(make_shared<Subject>("ПнаЯВУ", 30, CREDIT));
    subjects.add(make_shared<Subject>("ЭП", 30, CREDIT));
    subjects.add(make_shared<Subject>("СПЭ", 30, EXAM));

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

                for (const auto& m : members) {
                    m->setFaculty(newFac);
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
                auto best = maxByMetric(members);

                if (best) {
                    cout << "Тип:     " << best->getType() << '\n';
                    cout << "ФИО:     " << best->getFullName() << '\n';
                    cout << "Метрика: " << best->calculateMetric() << '\n';
                }
                break;
            }
            case 7:
                for (const auto& m : members) {
                    m->applyEffect(5);
                }
                break;

            case 8:
                cout << "UniversityMember\n";
                cout << members.size() << '\n';
                cout<< "Subjects\n";
                cout << subjects.size();
                break;
            
            case 9: {
                cout << "UniversityMember\n";
                auto m = members.get(2);
                m->printInformation(cout);
                cout << "Subject\n";
                auto s = subjects.get(2);
                s->printSubjectInformation();
                break;
            }

            case 10: {
                cout << "UniversityMember\n";
                members.removeAt(1);
                for (const auto& m : members) {
                    cout << *m << '\n';
                }
                cout<< "Subjects\n";
                subjects.removeAt(1);
                for (const auto& s : subjects) {
                    s->printSubjectInformation();
                }
                break;
            }    
            
            case 11: {
                string mName = "Никульшин Б.В.";
                string sName = "СПЭ";

                cout << "UniversityMember\n";
                size_t removed1 = members.removeIf([&mName](const UniversityMember& m) {
                    return m.getFullName() == mName;
                });
                for (const auto& m : members) {
                    cout << *m << '\n';
                }
                cout << "Удалено: " << removed1 << '\n';
                cout << "Subject\n";
                size_t removed2 = subjects.removeIf([&sName](const Subject& s) {
                    return string(s.getSubjectName()) == sName;
                });
                for (const auto& s : subjects) {
                    s->printSubjectInformation();
                }
                cout << "Удалено: " << removed2 << '\n';
                break;
            }

            case 12: {
                string mName = "Драбудько Н.Г.";
                string sName = "ПнаЯВУ";

                cout << "UniversityMember\n";
                auto found1 = members.find([&mName](const UniversityMember& m) {
                    return m.getFullName() == mName;
                });

                if (found1) {
                    cout << *found1 << '\n';
                } else {
                    cout << "Не найдено\n";
                }

                cout << "Subject\n";
                auto found2 = subjects.find([&sName](const Subject& s) {
                    return string(s.getSubjectName()) == sName;
                });

                if (found2) {
                    found2->printSubjectInformation();
                } else {
                    cout << "Не найдено\n";
                }
                break;
            }

            case 13: {
                cout << "UniversityMember\n";
                double m = averageMetric(members);
                cout << "Средняя метрика для работников университета: " << m << '\n';
                break;
            }

            case 14: {
                cout << "UniversityMember\n";
                auto sorted = sortBy<UniversityMember>(members, 
                [](const UniversityMember& a, const UniversityMember& b) {
                    return a.calculateMetric() > b.calculateMetric();
                });

                for (const auto& s : sorted) {
                    cout << s->getFullName() << " --- " << s->calculateMetric() << '\n';
                }
                break;
            }
        }
    } while (choice != 0);
}