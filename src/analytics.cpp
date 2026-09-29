#include "analytics.h"
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include "subject.h" 
#include "student.h"
#include "teacher.h"
#include "admin.h"
#include "faculty.h"
#include "collection.h"

void UniversityAnalytics::groupStudentsByGroup(const Collection<UniversityMember>& members) {
    std::cout << "\n--- Группировка студентов по группам ---\n";
    
    std::map<std::string, std::vector<std::shared_ptr<Student>>> groups;

    for (const auto& m : members) {
        if (m && m->getType() == "Student") {
            auto s = std::dynamic_pointer_cast<Student>(m);
            groups[std::string(s->getGroupNumber())].push_back(s);
        }
    }

    std::for_each(groups.begin(), groups.end(), [](const auto& pair) {
        std::cout << "Группа " << pair.first << " (Студентов: " << pair.second.size() << "):\n";
        for (const auto& s : pair.second) {
            std::cout << "  - " << s->getFullName() << " (Балл: " << s->calculateMetric() << ")\n";
        }
    });
}

void UniversityAnalytics::findTopStudents(const Collection<UniversityMember>& members, size_t topN) {
    std::cout << "\n--- Топ " << topN << " студентов по успеваемости ---\n";

    std::vector<std::shared_ptr<Student>> students;

    std::for_each(members.begin(), members.end(),
        [&students](const std::shared_ptr<UniversityMember>& m) {
            if (m && m->getType() == "Student") {
                if (auto s = std::dynamic_pointer_cast<Student>(m)) {
                    students.push_back(s);
                }
            }
        });

    std::sort(students.begin(), students.end(),
        [](const std::shared_ptr<Student>& a, const std::shared_ptr<Student>& b) {
            return a->calculateMetric() > b->calculateMetric();
        });

    size_t count = 0;
    for (const auto& s : students) {
        if (count++ >= topN) break;
        std::cout << count << ". " << s->getFullName() 
                  << " | Группа: " << s->getGroupNumber() 
                  << " | Балл: " << s->calculateMetric() << "\n";
    }
}

void UniversityAnalytics::countTeachersByLoad(const Collection<UniversityMember>& members, int minLoad) {
    std::cout << "\n--- Преподаватели с нагрузкой > " << minLoad << " ---\n";

    auto count = std::count_if(members.begin(), members.end(),
        [minLoad](const std::shared_ptr<UniversityMember>& m) {
            if (!m || m->getType() != "Teacher") return false;
            auto t = std::dynamic_pointer_cast<Teacher>(m);
            return t->getTeachingLoad() > minLoad;
        });

    std::cout << "Найдено преподавателей: " << count << "\n";
}

void UniversityAnalytics::findMinMaxMetrics(const Collection<UniversityMember>& members) {
    std::cout << "\n--- Мин/Макс метрики ---\n";

    auto minIt = std::min_element(members.begin(), members.end(),
        [](const std::shared_ptr<UniversityMember>& a, const std::shared_ptr<UniversityMember>& b) {
            if (a->getType() != "Student" || b->getType() != "Student") return false;
            return a->calculateMetric() < b->calculateMetric();
        });

    auto maxIt = std::max_element(members.begin(), members.end(),
        [](const std::shared_ptr<UniversityMember>& a, const std::shared_ptr<UniversityMember>& b) {
            if (a->getType() != "Student" || b->getType() != "Student") return false;
            return a->calculateMetric() < b->calculateMetric();
        });

    if (minIt != members.end() && (*minIt)->getType() == "Student") {
        std::cout << "Мин. балл: " << (*minIt)->getFullName() << " (" << (*minIt)->calculateMetric() << ")\n";
    }
    if (maxIt != members.end() && (*maxIt)->getType() == "Student") {
        std::cout << "Макс. балл: " << (*maxIt)->getFullName() << " (" << (*maxIt)->calculateMetric() << ")\n";
    }
}

void UniversityAnalytics::listUniqueSubjects(const Collection<UniversityMember>& members) {
    std::cout << "\n--- Уникальные предметы преподавателей ---\n";
    std::set<std::string> subjects;

    std::for_each(members.begin(), members.end(), [&subjects](const std::shared_ptr<UniversityMember>& m) {
        if (m && m->getType() == "Teacher") {
            auto t = std::dynamic_pointer_cast<Teacher>(m);
            if (t->getSubject()) {
                subjects.insert(std::string(t->getSubject()->getSubjectName()));
            }
        }
    });

    for (const auto& subj : subjects) {
        std::cout << "- " << subj << "\n";
    }
}

void UniversityAnalytics::calculateAverageLoad(const Collection<UniversityMember>& members) {
    std::cout << "\n--- Средняя нагрузка преподавателей ---\n";

    std::vector<int> loads;

    std::for_each(members.begin(), members.end(), [&loads](const std::shared_ptr<UniversityMember>& m) {
        if (m && m->getType() == "Teacher") {
            auto t = std::dynamic_pointer_cast<Teacher>(m);
            loads.push_back(t->getTeachingLoad());
        }
    });

    if (loads.empty()) {
        std::cout << "Нет данных о преподавателях.\n";
        return;
    }

    double sum = std::accumulate(loads.begin(), loads.end(), 0.0);
    double avg = sum / loads.size();

    std::cout << "Общая нагрузка: " << sum << "\n";
    std::cout << "Средняя нагрузка: " << std::fixed << std::setprecision(2) << avg << "\n";
}