#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <memory>
#include <utility>
#include "faculty.h"
#include "student.h"
#include "teacher.h"
#include "admin.h"
#include "collection.h"

class UniversityAnalytics {
public:
    static std::map<std::string, std::vector<std::shared_ptr<Student>>>
    groupStudentsByGroup(const Collection<UniversityMember>& members);

    static std::vector<std::shared_ptr<Student>>
    findTopStudents(const Collection<UniversityMember>& members, size_t topN);

    static size_t
    countTeachersByLoad(const Collection<UniversityMember>& members, int minLoad);

    static std::pair<std::shared_ptr<Student>, std::shared_ptr<Student>>
    findMinMaxMetrics(const Collection<UniversityMember>& members);

    static std::set<std::string>
    listUniqueSubjects(const Collection<UniversityMember>& members);

    static std::pair<double, double>
    calculateAverageLoad(const Collection<UniversityMember>& members);
};