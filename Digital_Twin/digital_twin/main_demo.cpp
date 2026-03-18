#include <iostream>
#include <iomanip>
#include <cmath>

#include "classes/weight_contribution.h"
#include "classes/Drag.h"
#include "classes/MileageModel.h"
#include "classes/Speed.h"

#include "functions/Drag.cpp"
#include "functions/weight_contribution.cpp"
#include "functions/mileagemodel.cpp"


MileageModel car_mileage(15.0, 1.79, 100.0);
Drag car_drag(130, true, false);
Speed car_speed;
double slope = 10.0;

//Ben's Functions//

// ── Placeholder scores (0–100) for the other 4 modules ──
// Replace each of these with the actual module calls once they're ready
double getAerodynamicsScore()   { return car_drag.getEfficiencyScore(); }
double getEngineScore()         { return 100.0; } //Rasiq
double getSpeedScore()   { return car_speed.getAverageScore(); }
double getMileageScore()           { return car_mileage.averageScore(slope); }


int main() {
    std::cout << " Fuel Efficiency Digital Twin - Audi RS6 Avant Performance \n\n";

    // User input 
    double passengers_kg, cargo_kg, tank_fraction;

    std::cout << "Enter total passenger mass (kg):        ";
    std::cin  >> passengers_kg;

    std::cout << "Enter cargo mass (kg):                  ";
    std::cin  >> cargo_kg;

    std::cout << "Enter fuel level (0.0 = empty, 1.0 = full): ";
    std::cin  >> tank_fraction;

    // RS6 tank: 73 L x 0.89 kg/L = 64.97 kg full
    double fuel_kg = tank_fraction * 64.97;

    // Weight
    WeightConfig cfg;
    cfg.passenger_mass_kg = passengers_kg;
    cfg.cargo_mass_kg     = cargo_kg;
    cfg.fuel_mass_kg      = fuel_kg;

    WeightContribution wc(cfg);
    WeightResult wr = wc.calculate();

    double dt = 0.1; 
    double throttle = 0.6; 
    for (int i = 0; i < 50; ++i) { 
        car_speed.update(throttle, dt); 
    } 

    // Collect all module scores
    double scores[] = {
        wr.efficiency_score,     
        getAerodynamicsScore(),  
        getEngineScore(),        
        getSpeedScore(),  
        getMileageScore()           
    };

    double overall = 0.0;
    for (double s : scores) overall += s;
    overall /= 5.0;

   

    // Results
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n--- Weight Breakdown ---\n";
    std::cout << "  Total mass         : " << wr.total_mass_kg                << " kg\n";
    std::cout << "  Added load         : " << wr.added_mass_kg                << " kg\n";
    std::cout << "  Rolling resistance : " << wr.rolling_resistance_N         << " N\n";
    std::cout << "  Fuel consumption   : " << wr.fuel_consumption_L_per_100km << " L/100 km\n";

    std::cout << "\n--- Module Scores ---\n";
    std::cout << "  Weight             : " << scores[0] << " / 100\n";
    std::cout << "  Aerodynamics       : " << scores[1] << " / 100  (placeholder)\n";
    std::cout << "  Engine             : " << scores[2] << " / 100  (placeholder)\n";
    std::cout << "  Speed              : " << scores[3] << " / 100  (placeholder)\n";
    std::cout << "  Mileage            : " << scores[4] << " / 100  (placeholder)\n";

    std::cout << "\n  Overall Score      : " << overall << " / 100\n";

    //Faaris' functions//

    
    cout << "The Drag force is: " << car_drag.getDragForce() << " N" << endl;
   
    
    exportToCsv("CarAnalysis.csv", 0, 200, 10);


    return 0;
}
