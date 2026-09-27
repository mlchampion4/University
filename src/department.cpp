#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include <iostream>
#include "faculty.h"
#include "teacher.h"
#include "department.h"
#include "exceptions.h"

Department::Department(std::string_view departmentName, std::shared_ptr<Faculty> faculty)
    : _departmentName(departmentName), _faculty(faculty) {
        if (departmentName.empty()) throw InvalidDataException("Название кафедры не может быть пустым");
        if (!faculty) throw InvalidDataException("Факультет не может быть null");
    }

std::string_view Department::getDepartmentName() const {
    return _departmentName;
}

std::shared_ptr<Faculty> Department::getFaculty() const {
    return _faculty;
}

std::vector<std::shared_ptr<Teacher>> Department::getTeachingStaff() const {
    return _teachingStaff;
}

void Department::setDepartmentName(std::string_view newDepartmentName) {
    if (newDepartmentName.empty()) throw InvalidDataException("Название кафедры не может быть пустым");
    _departmentName = newDepartmentName;
}

void Department::addTeacher(std::shared_ptr<Teacher> newTeacher) {
    if (!newTeacher) throw InvalidDataException("Преподаватель не может быть null");
    for (const auto& t : _teachingStaff) {
        if (t && t->getId() == newTeacher->getId()) throw DuplicateIdException("Преподаватель с ID " +
                std::to_string(newTeacher->getId()) + " уже есть на кафедре");
    }
    _teachingStaff.push_back(newTeacher);
}

void Department::printDepartmentInformation() const {
    std::cout << "Название кафедры: " << _departmentName << std::endl;
    std::cout << "Факультет: " << _faculty->getFacultyName() << std::endl;
    std::cout << "Cocтав кафедры: " << std::endl;
    for (int i = 0; i < int(_teachingStaff.size()); i++) {
        std::cout << _teachingStaff[i]->getFullName() << std::endl;
    }
}