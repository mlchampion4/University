#include <string>
#include <vector>
#include <iostream>
#include <string_view>
#include <memory>
#include "faculty.h"
#include "department.h"
#include "subject.h"
#include "teacher.h"

Teacher::Teacher(std::string_view fullName, std::weak_ptr<Faculty> faculty, std::weak_ptr<Department> department, std::shared_ptr<Subject> subject)
    : _fullName(fullName), _faculty(faculty), _department(department), _subject(subject) {}

std::string_view Teacher::getFullName() const {
    return _fullName;
}

std::shared_ptr<Faculty> Teacher::getFaculty() const {
    return _faculty.lock();
}

std::shared_ptr<Department> Teacher::getDepartment() const {
    return _department.lock();
}

std::shared_ptr<Subject> Teacher::getSubject() const {
    return _subject;
}

void Teacher::setDepartment(std::weak_ptr<Department> newDepartment) {
    _department = newDepartment;
}

void Teacher::setFaculty(std::weak_ptr<Faculty> newFaculty) {
    _faculty = newFaculty;
}

void Teacher::setSubject(std::shared_ptr<Subject> newSubject) {
    _subject = newSubject;
}

void Teacher::printTeacherInformation() const {
    std::cout << "ФИО преподавателя: " << _fullName << std::endl;
    std::cout << "Факультет: " << _faculty.lock()->getFacultyName() << std::endl;
    std::cout << "Кафедра: " << _department.lock()->getDepartmentName() << std::endl;
    std::cout << "Предмет: " << _subject->getSubjectName() << std::endl;
}