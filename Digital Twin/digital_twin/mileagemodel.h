#pragma once

class MileageModel {

private:
    double baseMileage;
    double fuelPrice;
    double distance;

    int calculateScore(double cost) const;

public:
    MileageModel(double baseMileage, double fuelPrice, double distance);

    double mileageFlat() const;
    double mileageUphill(double slopeAngle) const;
    double mileageDownhill(double slopeAngle) const;

    double fuelUsedFlat() const;
    double fuelUsedUphill(double slopeAngle) const;
    double fuelUsedDownhill(double slopeAngle) const;

    double costFlat() const;
    double costUphill(double slopeAngle) const;
    double costDownhill(double slopeAngle) const;

    void displayScoreBoard(double slopeAngle) const;
};