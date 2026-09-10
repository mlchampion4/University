#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include <iostream>
#include "department.h"
#include "student.h"
#include "faculty.h"
#include "teacher.h"
#include "subject.h"

Faculty::Faculty(std::string_view facultyName, unsigned int maxStudentsCount)
    : _facultyName(facultyName), _maxStudentsCount(maxStudentsCount) {}

std::string_view Faculty::getFacultyName() const {
    return _facultyName;
}

unsigned int Faculty::getMaxStudentsCount() const {
    return _maxStudentsCount;
}

void Faculty::setFacultyName(std::string_view newFacultyName) {
    _facultyName = newFacultyName;
}

void Faculty::setMaxStudentsCount(unsigned int newMaxStudentsCount) {
    _maxStudentsCount = newMaxStudentsCount;
}

void Faculty::addDepartment(std::shared_ptr<Department> newDepartment) {
    _departments.push_back(newDepartment);
}

void Faculty::addStudent(std::shared_ptr<Student> newStudent) {
    if (int(_students.size()) + 1 > int(_maxStudentsCount)) {
        std::cout << "Факультет полон, студент не зачислен!" << std::endl;
        return;
    }
    _students.push_back(newStudent);
}

int findTeacher(std::shared_ptr<Department> department, std::string_view teacherName) {
    for (int i = 0; i < int(department->getTeachingStaff().size()); i++) {
        if (department->getTeachingStaff()[i]->getFullName() == teacherName) {
            return i;
        }
    }
    
    std::cout << "Преподавателя с таким ФИО не найдено!";
    return -1;
}

void Faculty::addLecture(std::vector<std::shared_ptr<Student>> group, std::shared_ptr<Department> department, std::string_view teacherName) {
    int teacherIdx = findTeacher(department, teacherName);

    if (teacherIdx < 0) {
        return;
    }

    if (group[0]->getHours() > department->getTeachingStaff()[teacherIdx]->getSubject()->getHours()) {
        for (int i = 0; i < int(group.size()); i++) {
            group[i]->setHours(group[i]->getHours() - department->getTeachingStaff()[teacherIdx]->getSubject()->getHours());
        }

        std::cout << "Занятие по предмету " << department->getTeachingStaff()[teacherIdx]->getSubject()->getSubjectName() << " с преподавателем " << department->getTeachingStaff()[teacherIdx]->getFullName() << " успешно установлено. Свободных часов у группы: " << group[0]->getHours() << std::endl;
    }
    else {
        std::cout << "У студентов данной группы нет свободных часов для выставления занятий!" << std::endl;
        return;
    }
}

void Faculty::printFacultyInformafion() const {
    std::cout << "Название факультета: " << _facultyName << std::endl;
    std::cout << "Список кафедр: " << std::endl;
    for (int i = 0; i < int(_departments.size()); i++) {
        std::cout << _departments[i]->getDepartmentName() << std::endl;
    }
    std::cout << "Максимальное число студентов: " << _maxStudentsCount << std::endl;
    std::cout << "Список студентов: " << std::endl;
    for (int i = 0; i < int(_students.size()); i++) {
        std::cout << _students[i]->getFullName() << " группа " << _students[i]->getGroupNumber() << std::endl;
    }
}