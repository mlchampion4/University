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
    static constexpr const char* SEC_FACULTIES   = "[FACULTIES]";
    static constexpr const char* SEC_SUBJECTS    = "[SUBJECTS]";
    static constexpr const char* SEC_DEPARTMENTS = "[DEPARTMENTS]";
    static constexpr const char* SEC_MEMBERS     = "[MEMBERS]";

    static constexpr const char* TYPE_STUDENT = "Student";
    static constexpr const char* TYPE_TEACHER = "Teacher";
    static constexpr const char* TYPE_ADMIN   = "Administrator";

    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string trim(const std::string& str);
    static std::string join(const std::vector<unsigned int>& values, char sep);

    static int parseIndexOrThrow(const std::string& token, int limit, const std::string& entity);

    static std::shared_ptr<Faculty>    resolveFaculty(const UniversityData& data, int idx);
    static std::weak_ptr<Faculty>      resolveFacultyWeak(const UniversityData& data, int idx);
    static std::shared_ptr<Department> resolveDepartment(const UniversityData& data, int idx);
    static std::shared_ptr<Subject>    resolveSubject(const UniversityData& data, int idx);

    static void saveFaculties(std::ofstream& ofs, const UniversityData& data);
    static void saveSubjects(std::ofstream& ofs, const UniversityData& data);
    static void saveDepartments(std::ofstream& ofs, const UniversityData& data);
    static void saveMembers(std::ofstream& ofs, const UniversityData& data);
    static void saveStudent(std::ofstream& ofs, const std::shared_ptr<Student>& s, int facIdx);
    static void saveTeacher(std::ofstream& ofs, const std::shared_ptr<Teacher>& t,
                            int facIdx, const UniversityData& data);
    static void saveAdministrator(std::ofstream& ofs, const std::shared_ptr<Administrator>& a, int facIdx);

    static void loadFacultyLine(UniversityData& data, const std::vector<std::string>& tokens);
    static void loadSubjectLine(UniversityData& data, const std::vector<std::string>& tokens);
    static void loadDepartmentLine(UniversityData& data, const std::vector<std::string>& tokens);
    static void loadMemberLine(UniversityData& data, const std::vector<std::string>& tokens);

    static void loadStudent(UniversityData& data, const std::vector<std::string>& tokens);
    static void loadTeacher(UniversityData& data, const std::vector<std::string>& tokens);
    static void loadAdministrator(UniversityData& data, const std::vector<std::string>& tokens);

    static void reportFaculties(std::ofstream& ofs, const UniversityData& data);
    static void reportDepartments(std::ofstream& ofs, const UniversityData& data);
    static void reportSubjects(std::ofstream& ofs, const UniversityData& data);
    static void reportMembers(std::ofstream& ofs, const UniversityData& data);
    static void reportStudent(std::ofstream& ofs, const std::shared_ptr<Student>& s);
    static void reportTeacher(std::ofstream& ofs, const std::shared_ptr<Teacher>& t);
    static void reportAdministrator(std::ofstream& ofs, const std::shared_ptr<Administrator>& a);
};