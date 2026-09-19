#include <string>
#include <vector>
#include <iostream>
#include <string_view>
#include <memory>
#include "faculty.h"
#include "member.h"

UniversityMember::UniversityMember(std::string_view fullName, std::weak_ptr<Faculty> faculty) :
    _fullName(fullName), _faculty(faculty) {}

std::string UniversityMember::getFullName() const {
    return _fullName;
}

std::weak_ptr<Faculty> UniversityMember::getFaculty() const {
    return _faculty;
}

void UniversityMember::setFaculty(const std::weak_ptr<Faculty> newFaculty) {
    _faculty = newFaculty;
}

double UniversityMember::calculateMetric() const {
    return 0.0;
}