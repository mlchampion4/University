#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>

class Teacher;
class Faculty;

class Department {
    public:
        Department(std::string_view departmentName, std::shared_ptr<Faculty> faculty);

        std::string_view getDepartmentName() const;
        std::shared_ptr<Faculty> getFaculty() const;
        std::vector<std::shared_ptr<Teacher>> getTeachingStaff() const;

        void setDepartmentName(std::string_view newDepartmentName);
        void addTeacher(std::shared_ptr<Teacher> newTeacher);

        void printDepartmentInformation() const;
        
    private:
        std::string _departmentName;
        std::shared_ptr<Faculty> _faculty;
        std::vector<std::shared_ptr<Teacher>> _teachingStaff;
};