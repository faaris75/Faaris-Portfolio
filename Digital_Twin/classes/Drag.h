#pragma once
#include <string>

class Drag {
private:
    const double dragcoeff = 0.35;
    const double frontalarea = 2.41;
    const double airdensity = 1.225; // standard at 15C

    double speed = 0.0;
    double ambientTemp;
    double airSuspensionFactor;
    double grilleShutterFactor;
    double rearSpoilerFactor;
    double wheelFactor;
    double windowFactor;
    double roofLoadFactor;

public:
    // Constructor
    Drag(bool windowOpen = false, bool roofLoad = false, double temp = 15);

    // Function prototypes
    void updateSpeed(double speedkmh);
    double calculateDensity(double ambientTemp);
    double CalculateCd();
    double getEffectiveDrag();
    double getDragForce();
    double getEfficiencyScore();
};
