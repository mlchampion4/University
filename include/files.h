#pragma once

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <map>

#include "faculty.h"
#include "department.h"
#include "subject.h"
#include "student.h"
#include "teacher.h"
#include "admin.h"
#include "collection.h"
#include "exceptions.h"

struct UniversityData {
    std::vector<std::shared_ptr<Faculty>> faculties;
    std::vector<std::shared_ptr<Department>> departments;
    std::vector<std::shared_ptr<Subject>> subjects;
    Collection<UniversityMember> members;
};

class StorageManager {
public:
    static void saveState(const std::string& filename, const UniversityData& data);

    static void loadState(const std::string& filename, UniversityData& data);

    static void logAction(const std::string& logFilename, const std::string& action);

    static void generateReport(const std::string& reportFilename, const UniversityData& data);

private:
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string trim(const std::string& str);
};