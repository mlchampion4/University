#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>

class Faculty;

class UniversityMember {
    public:
        UniversityMember(std::string_view fullName, std::weak_ptr<Faculty> faculty);
        virtual ~UniversityMember() = default;

        std::string getFullName() const;
        std::weak_ptr<Faculty> getFaculty() const;

        void setFaculty(const std::weak_ptr<Faculty> newFaculty);

        virtual void printInformation(std::ostream& os) const = 0;
        virtual std::string getType() const = 0;
        virtual double calculateMetric() const;
        virtual void applyEffect(int value) = 0;

    protected:
        std::string _fullName;
        std::weak_ptr<Faculty> _faculty;
};

inline std::ostream& operator<<(std::ostream& os, const UniversityMember& member) {
    member.printInformation(os);
    return os;
}