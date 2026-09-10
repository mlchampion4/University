#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>

class Department;
class Student;

class Faculty {
    public:
        Faculty(std::string_view facultyName, unsigned int maxStudentsCount);

        std::string_view getFacultyName() const;
        unsigned int getMaxStudentsCount() const;

        void setFacultyName(std::string_view newFacultyName);
        void setMaxStudentsCount(unsigned int newMaxStudentsCount);

        void addDepartment(std::shared_ptr<Department> newDepartment);
        void addStudent(std::shared_ptr<Student> newStudent);
        void addLecture(std::vector<std::shared_ptr<Student>> group, std::shared_ptr<Department> department, std::string_view teacherName);

        void printFacultyInformafion() const;
        
    private:
        std::string _facultyName;
        std::vector<std::shared_ptr<Department>> _departments;
        unsigned int _maxStudentsCount;
        std::vector<std::shared_ptr<Student>> _students;
};