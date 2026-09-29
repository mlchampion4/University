#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <memory>
#include <iomanip>
#include "faculty.h"
#include "student.h"
#include "teacher.h"
#include "admin.h"
#include "collection.h"

class UniversityAnalytics {
public:
    static void groupStudentsByGroup(const Collection<UniversityMember>& members);

    static void findTopStudents(const Collection<UniversityMember>& members, size_t topN);

    static void countTeachersByLoad(const Collection<UniversityMember>& members, int minLoad);

    static void findMinMaxMetrics(const Collection<UniversityMember>& members);

    static void listUniqueSubjects(const Collection<UniversityMember>& members);

    static void calculateAverageLoad(const Collection<UniversityMember>& members);
};