#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include "member.h"

class Faculty;
class Department;
class Subject;

class Teacher : public UniversityMember {
    public:
        Teacher(int id, std::string_view fullName, std::weak_ptr<Faculty> faculty, std::weak_ptr<Department> department, std::shared_ptr<Subject> subject, int maxTeachingLoad);

        std::shared_ptr<Department> getDepartment() const;
        std::shared_ptr<Subject> getSubject() const;
        int getTeachingLoad() const;
        int getId() const;

        void setDepartment(std::weak_ptr<Department> newDepartment);
        void setSubject(std::shared_ptr<Subject> newSubject);

        std::string getType() const override;
        void printInformation(std::ostream& os) const override;
        double calculateMetric() const override;
        void applyEffect(int value) override;
        void readFrom(std::istream& is) override;
        bool equals(const UniversityMember& other) const override;

    private:
        int _id;
        std::weak_ptr<Department> _department;
        std::shared_ptr<Subject> _subject;
        int _teachingLoad;
        int _maxTeachingLoad;
};