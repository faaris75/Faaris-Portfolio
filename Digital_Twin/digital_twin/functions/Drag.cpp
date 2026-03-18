#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath> 
#include "../classes/Drag.h"

using namespace std;

// --- Class Methods Implementation ---

Drag::Drag(double speedkmh, bool windowOpen, bool roofLoad) {
    speed = speedkmh;
    
    // Logic for factors
    if (speed >= 100) airSuspensionFactor = 0.975;
    else if (speed >= 70) airSuspensionFactor = 0.985;
    else airSuspensionFactor = 1.0;

    grilleShutterFactor = (speed >= 80) ? 0.97 : 1.0;
    rearSpoilerFactor = (speed >= 120) ? 1.01 : 1.005;
    wheelFactor = 1.02;
    windowFactor = windowOpen ? 1.12 : 1.0;
    roofLoadFactor = roofLoad ? 1.15 : 1.0;
}

double Drag::getEffectiveDrag() {
    return dragcoeff * airSuspensionFactor * grilleShutterFactor * rearSpoilerFactor * wheelFactor * windowFactor * roofLoadFactor;
}

double Drag::getDragForce() {
    // Formula: Fd = 1/2 * rho * Cd * A * v^2
    return 0.5 * airdensity * getEffectiveDrag() * frontalarea * pow(speed / 3.6, 2);
}

double Drag::getEfficiencyScore() {
    double perfectDragCoeff = 0.31;
    double newDragCoeff = getEffectiveDrag();
    double changeInDrag = (newDragCoeff - perfectDragCoeff);
    double penalty = changeInDrag / 0.005; 
    double score = 100.0 - penalty;
    return (score <= 0) ? 0 : score;
}

// --- Standalone Helper Functions ---

void exportToCsv(string filename, int startSpeed, int endSpeed, int step) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file." << endl;
        return;
    }

    file << "Speed(km/h),Effective_Cd,Drag_Force(N),Efficiency_Score\n";
    file << fixed << setprecision(4);

    

    for (int s = startSpeed; s <= endSpeed; s += step) {
        Drag car(s, false, false);
        file << s << "," 
             << car.getEffectiveDrag() << "," 
             << car.getDragForce() << "," 
             << car.getEfficiencyScore() << "\n";
    }
    file.close();
    cout << "Successfully wrote data to " << filename << endl;
}

// --- Main Entry Point ---

