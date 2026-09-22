#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include "member.h"

class Faculty;

class Student : public UniversityMember {
    public:
        Student(std::string_view fullName,
                std::weak_ptr<Faculty> faculty,
                std::string_view studentNumber,
                std::string_view groupNumber,
                unsigned int maxHoursPerWeek);

        std::string getStudentNumber() const;
        std::string_view getGroupNumber() const;
        unsigned int getHours() const;
        unsigned int getMaxHoursPerWeek() const;
        std::vector<unsigned int> getMarks() const;

        void setGroupNumber(std::string_view newGroupNumber);
        void setStudentNumber(std::string_view newStudentNumber);
        void setHours(unsigned int newHours);
        void setMaxHoursPerWeek(unsigned int newMaxHoursPerWeek);
        void setMarks(std::vector<unsigned int> newMarks);
        
        std::string getType() const override;
        void printInformation(std::ostream& os) const override;
        double calculateMetric() const override;
        void applyEffect(int value) override;
        void readFrom(std::istream& is) override;
        bool equals(const UniversityMember& other) const override;
        std::string getMetricName() const override;

    private:
        std::string _studentNumber;
        std::string _groupNumber;
        unsigned int _maxHoursPerWeek;
        unsigned int _hours;
        std::vector<unsigned int> _marks;
};