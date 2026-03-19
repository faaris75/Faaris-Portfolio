#include <iostream>
#include <iomanip>
#include <cmath>

#include "classes/weight_contribution.h"
#include "classes/Drag.h"
#include "classes/MileageModel.h"
#include "classes/Speed.h"
#include "classes/Temperature.h"

#include "functions/Drag.cpp"
#include "functions/weight_contribution.cpp"
#include "functions/mileagemodel.cpp"
#include "functions/Temperature.cpp"

MileageModel car_mileage(15.0, 1.79, 100.0);
Drag car_drag(130, true, false);
Speed car_speed;
double slope = 10.0;

// Module score functions
double getAerodynamicsScore() { return car_drag.getEfficiencyScore(); }
double getSpeedScore() { return car_speed.getAverageScore(); }
double getMileageScore() { return car_mileage.averageScore(slope); }

int main() {
    std::cout << "Fuel Efficiency Digital Twin - Audi RS6 Avant Performance\n\n";

    // User input
    double passengers_kg, cargo_kg, tank_fraction, ambient_temp;

    std::cout << "Enter total passenger mass (kg):        ";
    std::cin >> passengers_kg;

    std::cout << "Enter cargo mass (kg):                  ";
    std::cin >> cargo_kg;

    std::cout << "Enter fuel level (0.0 = empty, 1.0 = full): ";
    std::cin >> tank_fraction;

    std::cout << "Enter ambient temperature (C):          ";
    std::cin >> ambient_temp;

    // Temperature model = Engine model
    Temperature car_temp(ambient_temp);

    // RS6 tank: 73 L x 0.89 kg/L = 64.97 kg full
    double fuel_kg = tank_fraction * 64.97;

    // Weight
    WeightConfig cfg;
    cfg.passenger_mass_kg = passengers_kg;
    cfg.cargo_mass_kg = cargo_kg;
    cfg.fuel_mass_kg = fuel_kg;

    WeightContribution wc(cfg);
    WeightResult wr = wc.calculate();

    // Simulate speed + engine temperature
    double dt = 0.1;
    double throttle = 0.6;
    double simulatedSpeedKmh = 0.0;
    int simulatedRpm = 900;

    for (int i = 0; i < 50; ++i) {
        car_speed.update(throttle, dt);

        simulatedSpeedKmh += 2.0;
        if (simulatedSpeedKmh > 100.0) {
            simulatedSpeedKmh = 100.0;
        }

        simulatedRpm = 900
                     + static_cast<int>(throttle * 2500.0)
                     + static_cast<int>(simulatedSpeedKmh * 18.0);

        car_temp.update(simulatedRpm, throttle, simulatedSpeedKmh, dt);
    }

    // Engine score comes directly from Temperature::getEfficiency()
    double engineScore = car_temp.getEfficiency();

    // Collect all module scores
    double scores[] = {
        wr.efficiency_score,     // Weight
        getAerodynamicsScore(),  // Aerodynamics
        engineScore,             // Engine (Temperature)
        getSpeedScore(),         // Speed
        getMileageScore()        // Mileage
    };

    double overall = 0.0;
    for (double s : scores) overall += s;
    overall /= 5.0;

    // Results
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "\n--- Weight Breakdown ---\n";
    std::cout << "  Total mass         : " << wr.total_mass_kg << " kg\n";
    std::cout << "  Added load         : " << wr.added_mass_kg << " kg\n";
    std::cout << "  Rolling resistance : " << wr.rolling_resistance_N << " N\n";
    std::cout << "  Fuel consumption   : " << wr.fuel_consumption_L_per_100km << " L/100 km\n";

    std::cout << "\n--- Module Scores ---\n";
    std::cout << "  Weight             : " << scores[0] << " / 100\n";
    std::cout << "  Aerodynamics       : " << scores[1] << " / 100\n";
    std::cout << "  Engine             : " << scores[2] << " / 100\n";
    std::cout << "  Speed              : " << scores[3] << " / 100\n";
    std::cout << "  Mileage            : " << scores[4] << " / 100\n";

    std::cout << "\n  Overall Score      : " << overall << " / 100\n";

    std::cout << "\nThe Drag force is: " << car_drag.getDragForce() << " N\n";

    exportToCsv("CarAnalysis.csv", 0, 200, 10);

    return 0;
}
