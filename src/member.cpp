#include <string>
#include <vector>
#include <iostream>
#include <string_view>
#include <memory>
#include "faculty.h"
#include "member.h"

UniversityMember::UniversityMember(int id, std::string_view fullName, std::weak_ptr<Faculty> faculty) :
    _id(id), _fullName(fullName), _faculty(faculty) {}

UniversityMember::UniversityMember(std::string_view fullName, std::weak_ptr<Faculty> faculty) :
    _fullName(fullName), _faculty(faculty) {}

std::string UniversityMember::getFullName() const {
    return _fullName;
}

int UniversityMember::getId() const {
    return _id;
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

std::ostream& operator<<(std::ostream& os, const UniversityMember& member) {
    member.printInformation(os);
    return os;
}

std::istream& operator>>(std::istream& is, UniversityMember& member) {
    member.readFrom(is);
    return is;
}

bool UniversityMember::operator<(const UniversityMember& other) const {
    return this->calculateMetric() < other.calculateMetric();
}

bool UniversityMember::operator>(const UniversityMember& other) const {
    return this->calculateMetric() > other.calculateMetric();
}

bool UniversityMember::operator<=(const UniversityMember& other) const {
    return this->calculateMetric() <= other.calculateMetric();
}

bool UniversityMember::operator>=(const UniversityMember& other) const {
    return this->calculateMetric() >= other.calculateMetric();
}

bool UniversityMember::operator==(const UniversityMember& other) const {
    return equals(other);
}

bool UniversityMember::operator!=(const UniversityMember& other) const {
    return !(*this == other);
}