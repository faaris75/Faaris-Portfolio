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

double MileageModel::mileageFlat() const {
    return baseMileage;
}

double MileageModel::mileageUphill(double slopeAngle) const {
    double factor = 1 - (sin(slopeAngle * std::numbers::pi / 180) * 0.4);
    return baseMileage * factor;
}

double MileageModel::mileageDownhill(double slopeAngle) const {
    double factor = 1 + (sin(slopeAngle * std::numbers::pi / 180) * 0.3);
    return baseMileage * factor;
}

double MileageModel::fuelUsedFlat() const {
    return distance / mileageFlat();
}

double MileageModel::fuelUsedUphill(double slopeAngle) const {
    return distance / mileageUphill(slopeAngle);
}

double MileageModel::fuelUsedDownhill(double slopeAngle) const {
    return distance / mileageDownhill(slopeAngle);
}

double MileageModel::costFlat() const {
    return fuelUsedFlat() * fuelPrice;
}

double MileageModel::costUphill(double slopeAngle) const {
    return fuelUsedUphill(slopeAngle) * fuelPrice;
}

double MileageModel::costDownhill(double slopeAngle) const {
    return fuelUsedDownhill(slopeAngle) * fuelPrice;
}

int MileageModel::calculateScore(double cost) const
{
    int score = 100 - (cost * 2);

    if(score < 0)
        score = 0;

    return score;
}

void MileageModel::displayScoreBoard(double slopeAngle) const
{
    double flatCost = costFlat();
    double upCost = costUphill(slopeAngle);
    double downCost = costDownhill(slopeAngle);

    std::cout << "\n===== SCOREBOARD =====\n";

    std::cout << "Flat Road Score: "
              << calculateScore(flatCost) << std::endl;

    std::cout << "Uphill Score: "
              << calculateScore(upCost) << std::endl;

    std::cout << "Downhill Score: "
              << calculateScore(downCost) << std::endl;

    std::cout << "======================\n";
}