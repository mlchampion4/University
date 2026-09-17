#include <string>
#include <iostream>
#include <vector>
#include <string_view>
#include <memory>
#include "faculty.h"
#include "student.h"

Student::Student(std::string_view fullName, std::string_view studentNumber, std::string_view groupNumber, std::weak_ptr<Faculty> faculty, unsigned int maxHoursPerWeek)
    : _fullName(fullName), _studentNumber(studentNumber), _groupNumber(groupNumber), _faculty(faculty), _maxHoursPerWeek(maxHoursPerWeek), _hours(maxHoursPerWeek) {}

std::string_view Student::getFullName() const {
    return _fullName;
}

std::string Student::getStudentNumber() const {
    return _studentNumber;
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

std::vector<unsigned int> Student::getMarks() const {
    return _marks;
}

void Student::setGroupNumber(std::string_view newGroupNumber) {
    _groupNumber = newGroupNumber;
}

void Student::setStudentNumber(std::string_view newStudentNumber) {
    _studentNumber = newStudentNumber;
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

void Student::setMarks(std::vector<unsigned int> newMarks) {
    _marks = newMarks;
}

unsigned int Student::calculateAvgMark(std::vector<unsigned int> marks) const {
    if (marks.empty()) return 0;
    unsigned int sum = 0;
    for (unsigned int m : marks) sum += m;
    return sum / static_cast<unsigned int>(marks.size());
}

std::ostream& operator<<(std::ostream& os, const Student& student) {
    os << "ФИО Студента: " << student._fullName << "\n" << "Номер студенческого билета: "
     << student._studentNumber << "\n" << "Факультет: " << student._faculty.lock()->getFacultyName() << "\n"
     << "Группа: " << student._groupNumber << "\n" << "Максимум часов в неделю: "
     << student._maxHoursPerWeek << std::endl;
     os << "Оценки: \n";
     for (const auto& s: student._marks)
        os << s << "\n";
     return os;
}

std::istream& operator>>(std::istream& is, Student& student) {
    std::cout << "Введите полное имя студента: ";
    std::getline(is >> std::ws, student._fullName);
    std::cout << "Введите номер студ. билета: ";
    std::getline(is, student._studentNumber);
    std::cout << "Введите номер группы: ";
    std::getline(is, student._groupNumber);
    std::cout << "Введите максимальное число часов: ";
    is >> student._maxHoursPerWeek;
    return is;
}

bool Student::operator==(const Student& otherStudent) const {
    return _studentNumber == otherStudent.getStudentNumber();
}

bool Student::operator!=(const Student& otherStudent) const {
    return _studentNumber != otherStudent.getStudentNumber();
}

bool Student::operator<(const Student& otherStudent) const {
    return calculateAvgMark(_marks) < otherStudent.calculateAvgMark(otherStudent.getMarks());
}

bool Student::operator>(const Student& otherStudent) const {
    return calculateAvgMark(_marks) > otherStudent.calculateAvgMark(otherStudent.getMarks());
}

bool Student::operator>=(const Student& otherStudent) const {
    return calculateAvgMark(_marks) >= otherStudent.calculateAvgMark(otherStudent.getMarks());
}

bool Student::operator<=(const Student& otherStudent) const {
    return calculateAvgMark(_marks) <= otherStudent.calculateAvgMark(otherStudent.getMarks());
}
