#ifndef DRAG_H
#define DRAG_H

#include <string>

class Drag {
private:
    const double airdensity = 1.225;
    const double dragcoeff = 0.33;
    const double frontalarea = 2.65;

    double speed;
    double airSuspensionFactor;
    double grilleShutterFactor;
    double rearSpoilerFactor;
    double wheelFactor;
    double windowFactor;
    double roofLoadFactor;

public:
    // Constructor
    Drag(double speedkmh, bool windowOpen = false, bool roofLoad = false);

    // Function prototypes
    double getEffectiveDrag();
    double getDragForce();
    double getEfficiencyScore();
};

// Prototype for the standalone CSV function
void exportToCsv(std::string filename, int startSpeed, int endSpeed, int step);

#endif