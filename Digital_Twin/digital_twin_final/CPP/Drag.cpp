#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath> 
#include "Drag.h"

using namespace std;

Drag::Drag(bool windowOpen, bool roofLoad, double temp) : ambientTemp{temp} {
    windowFactor = windowOpen ? 1.12 : 1.0;
    roofLoadFactor = roofLoad ? 1.15 : 1.0;
}

void Drag::updateSpeed(double speedkmh) {
    speed = speedkmh;
}

double Drag::calculateDensity(double ambientTemp) {
    double density = airdensity * (288.15 / (ambientTemp + 273.15)); // derivation in appendix
    return density;
}

double Drag::getEffectiveDrag() { // calculate Cd
    if (speed >= 100) airSuspensionFactor = 0.975;
    else if (speed >= 70) airSuspensionFactor = 0.985;
    else airSuspensionFactor = 1.0;
    grilleShutterFactor = (speed >= 80) ? 0.97 : 1.0;
    rearSpoilerFactor = (speed >= 120) ? 1.01 : 1.005;
    wheelFactor = 1.02;
    return dragcoeff * airSuspensionFactor * grilleShutterFactor * rearSpoilerFactor * wheelFactor * windowFactor * roofLoadFactor;
}

double Drag::getDragForce() {
    // Formula: Fd = 1/2 * rho * Cd * A * v^2
    return 0.5 * calculateDensity(ambientTemp) * getEffectiveDrag() * frontalarea * pow(speed / 3.6, 2);
}

double Drag::getEfficiencyScore() { // compare baseline drag force to actual
    double baselineDragForce = 0.5 * airdensity * dragcoeff * frontalarea * pow(speed / 3.6, 2);
    double actualDragForce = getDragForce();

    if(speed == 0) {
        return 100; // prevent division by 0
    }
    
    double score = baselineDragForce / actualDragForce * 100.0; // return the fraction of the baseline drag force to the calculated
    // can result in a score higher than 100 indicating performance better than calibrated conditions. 
    return (score <= 0) ? 0 : score;
}