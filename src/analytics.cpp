#include "analytics.h"
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>
#include "subject.h"
#include "student.h"
#include "teacher.h"
#include "admin.h"
#include "faculty.h"
#include "collection.h"

std::map<std::string, std::vector<std::shared_ptr<Student>>>
UniversityAnalytics::groupStudentsByGroup(const Collection<UniversityMember>& members) {
    std::map<std::string, std::vector<std::shared_ptr<Student>>> groups;

    for (const auto& m : members) {
        if (m && m->getType() == "Student") {
            auto s = std::dynamic_pointer_cast<Student>(m);
            groups[std::string(s->getGroupNumber())].push_back(s);
        }
    }

    return groups;
}

std::vector<std::shared_ptr<Student>>
UniversityAnalytics::findTopStudents(const Collection<UniversityMember>& members, size_t topN) {
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

    if (students.size() > topN) {
        students.resize(topN);
    }

    return students;
}

size_t UniversityAnalytics::countTeachersByLoad(const Collection<UniversityMember>& members, int minLoad) {
    return std::count_if(members.begin(), members.end(),
        [minLoad](const std::shared_ptr<UniversityMember>& m) {
            if (!m || m->getType() != "Teacher") return false;
            auto t = std::dynamic_pointer_cast<Teacher>(m);
            return t->getTeachingLoad() > minLoad;
        });
}

std::pair<std::shared_ptr<Student>, std::shared_ptr<Student>>
UniversityAnalytics::findMinMaxMetrics(const Collection<UniversityMember>& members) {
    std::shared_ptr<Student> minStudent = nullptr;
    std::shared_ptr<Student> maxStudent = nullptr;

    for (const auto& m : members) {
        if (m && m->getType() == "Student") {
            auto s = std::dynamic_pointer_cast<Student>(m);
            if (!minStudent || s->calculateMetric() < minStudent->calculateMetric()) {
                minStudent = s;
            }
            if (!maxStudent || s->calculateMetric() > maxStudent->calculateMetric()) {
                maxStudent = s;
            }
        }
    }

    return {minStudent, maxStudent};
}

std::set<std::string>
UniversityAnalytics::listUniqueSubjects(const Collection<UniversityMember>& members) {
    std::set<std::string> subjects;

    std::for_each(members.begin(), members.end(),
        [&subjects](const std::shared_ptr<UniversityMember>& m) {
            if (m && m->getType() == "Teacher") {
                auto t = std::dynamic_pointer_cast<Teacher>(m);
                if (t->getSubject()) {
                    subjects.insert(std::string(t->getSubject()->getSubjectName()));
                }
            }
        });

    return subjects;
}

std::pair<double, double>
UniversityAnalytics::calculateAverageLoad(const Collection<UniversityMember>& members) {
    std::vector<int> loads;

    std::for_each(members.begin(), members.end(),
        [&loads](const std::shared_ptr<UniversityMember>& m) {
            if (m && m->getType() == "Teacher") {
                auto t = std::dynamic_pointer_cast<Teacher>(m);
                loads.push_back(t->getTeachingLoad());
            }
        });

    if (loads.empty()) {
        return {0.0, 0.0};
    }

    double sum = std::accumulate(loads.begin(), loads.end(), 0.0);
    double avg = sum / loads.size();
    return {sum, avg};
}