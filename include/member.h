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
        virtual void readFrom(std::istream& is) = 0;
        virtual bool equals(const UniversityMember& other) const = 0;

        friend std::ostream& operator<<(std::ostream& os, const UniversityMember& member);
        friend std::istream& operator>>(std::istream& is, UniversityMember& member);

        bool operator<(const UniversityMember& other) const;
        bool operator>(const UniversityMember& other) const;
        bool operator<=(const UniversityMember& other) const;
        bool operator>=(const UniversityMember& other) const;
        bool operator==(const UniversityMember& other) const;
        bool operator!=(const UniversityMember& other) const;

    protected:
        std::string _fullName;
        std::weak_ptr<Faculty> _faculty;
};