#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>

class Faculty;
class Department;
class Subject;

class Teacher {
    public:
        Teacher(std::string_view fullName, std::weak_ptr<Faculty> faculty, std::weak_ptr<Department> department, std::shared_ptr<Subject> subject);

        std::string_view getFullName() const;
        std::shared_ptr<Faculty> getFaculty() const;
        std::shared_ptr<Department> getDepartment() const;
        std::shared_ptr<Subject> getSubject() const;

        void setFaculty(std::weak_ptr<Faculty> newFaculty);
        void setDepartment(std::weak_ptr<Department> newDepartment);
        void setSubject(std::shared_ptr<Subject> newSubject);

        void printTeacherInformation() const;

    private:
        std::string _fullName;
        std::weak_ptr<Faculty> _faculty;
        std::weak_ptr<Department> _department;
        std::shared_ptr<Subject> _subject;
};