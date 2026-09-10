#include <string>
#include <iostream>
#include <vector>
#include <string_view>
#include <memory>
#include "faculty.h"
#include "student.h"

Student::Student(std::string_view fullName, std::string_view groupNumber, std::weak_ptr<Faculty> faculty, unsigned int maxHoursPerWeek, unsigned int hours)
    : _fullName(fullName), _groupNumber(groupNumber), _faculty(faculty), _maxHoursPerWeek(maxHoursPerWeek), _hours(hours) {}

std::string_view Student::getFullName() const {
    return _fullName;
}

unsigned int Student::getHours() const {
    return _hours;
}

std::string_view Student::getGroupNumber() const {
    return _groupNumber;
}

std::weak_ptr<Faculty> Student::getFaculty() const {
    return _faculty.lock();
}

unsigned int Student::getMaxHoursPerWeek() const {
    return _maxHoursPerWeek;
}

void Student::setGroupNumber(std::string_view newGroupNumber) {
    _groupNumber = newGroupNumber;
}

void Student::setHours(unsigned int newHours) {
    _hours = newHours;
}

void Student::setFaculty(std::weak_ptr<Faculty> newFaculty) {
    _faculty = newFaculty;
}

void Student::setMaxHoursPerWeek(unsigned int newMaxHoursPerWeek) {
    _maxHoursPerWeek = newMaxHoursPerWeek;
}

void Student::printStudentInformation() const {
    std::cout << "ФИО Студента: " << _fullName << std::endl;
    std::cout << "Факультет: " << _faculty.lock()->getFacultyName() << std::endl;
    std::cout << "Группа: " << _groupNumber << std::endl;
    std::cout << "Максимум часов в неделю: " << _maxHoursPerWeek << std::endl;
}