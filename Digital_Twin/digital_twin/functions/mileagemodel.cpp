#include <iostream>
#include <cmath>
#include <numbers>
#include "../classes/mileagemodel.h"

MileageModel::MileageModel(double baseMileage, double fuelPrice, double distance)
{
    this->baseMileage = baseMileage;
    this->fuelPrice = fuelPrice;
    this->distance = distance;
}

double MileageModel::mileageFlat() const
{
    return baseMileage;
}

double MileageModel::mileageUphill(double slopeAngle) const
{
    double radians = slopeAngle * std::numbers::pi / 180.0;
    double factor = 1 - (std::sin(radians) * 0.4);
    return baseMileage * factor;
}

double MileageModel::mileageDownhill(double slopeAngle) const
{
    double radians = slopeAngle * std::numbers::pi / 180.0;
    double factor = 1 + (std::sin(radians) * 0.3);
    return baseMileage * factor;
}

double MileageModel::fuelUsedFlat() const
{
    return distance / mileageFlat();
}

double MileageModel::fuelUsedUphill(double slopeAngle) const
{
    return distance / mileageUphill(slopeAngle);
}

double MileageModel::fuelUsedDownhill(double slopeAngle) const
{
    return distance / mileageDownhill(slopeAngle);
}

double MileageModel::costFlat() const
{
    return fuelUsedFlat() * fuelPrice;
}

double MileageModel::costUphill(double slopeAngle) const
{
    return fuelUsedUphill(slopeAngle) * fuelPrice;
}

double MileageModel::costDownhill(double slopeAngle) const
{
    return fuelUsedDownhill(slopeAngle) * fuelPrice;
}

int MileageModel::calculateScore(double cost) const
{
    int score = static_cast<int>(100 - (cost * 2));

    if (score < 0)
        score = 0;

    return score;
}

int MileageModel::scoreFlat() const
{
    return calculateScore(costFlat());
}

int MileageModel::scoreUphill(double slopeAngle) const
{
    return calculateScore(costUphill(slopeAngle));
}

int MileageModel::scoreDownhill(double slopeAngle) const
{
    return calculateScore(costDownhill(slopeAngle));
}
double MileageModel::averageScore(double slopeAngle) const
{
    double total = scoreFlat() 
                 + scoreUphill(slopeAngle) 
                 + scoreDownhill(slopeAngle);

    return total / 3.0;
}
void MileageModel::displayScoreBoard(double slopeAngle) const
{
    std::cout << "\n===== SCOREBOARD =====\n";
    std::cout << "Flat Road Score: " << scoreFlat() << std::endl;
    std::cout << "Uphill Score: " << scoreUphill(slopeAngle) << std::endl;
    std::cout << "Downhill Score: " << scoreDownhill(slopeAngle) << std::endl;
    std::cout << "======================\n";
}