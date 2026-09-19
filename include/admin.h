#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include "member.h"

class Administrator : public UniversityMember {
    public:
        Administrator(std::string_view fullName, std::weak_ptr<Faculty> faculty, std::string_view position, int managedPeople);

        std::string getPosition() const;
        int getManagedPeople() const;

        void setPosition(std::string_view newPosition);
        void setManagedPeople(int newManagedPeople);

        std::string getType() const override;
        void printInformation(std::ostream& os) const override;
        double calculateMetric() const override;
        void applyEffect(int value) override;

    private:
        std::string _position;
        int _managedPeople;
};
