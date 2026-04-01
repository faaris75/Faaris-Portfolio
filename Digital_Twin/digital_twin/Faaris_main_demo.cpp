#include <iostream>
#include <iomanip>
#include <fstream>
#include "weight_contribution.h"
#include "Drag.h"
#include "Speed.h"
#include "Temperature.h"
#include "mileagemodel.h"

using namespace std;

int main() {
    // 1. VARIABLES TO RECEIVE FROM PYTHON UI
    double passengers, cargo, fuel_fraction;
    double speed;
    int window_open, roof_load; 
    int rpm;
    double engine_load, ambient_temp;
    double distance, slope_angle, fuel_price;

    // 2. READ INPUTS
    // This perfectly matches the 12 inputs sent by subprocess in your Python code
    if (!(cin >> passengers >> cargo >> fuel_fraction 
              >> speed >> window_open >> roof_load
              >> rpm >> engine_load >> ambient_temp
              >> distance >> slope_angle >> fuel_price)) {
        cerr << "Error reading inputs from Python UI." << endl;
        return 1; 
    }

    // 3. INSTANTIATE MODULES AND CALCULATE

    // --- Weight ---
    WeightConfig w_cfg;
    w_cfg.passenger_mass_kg = passengers;
    w_cfg.cargo_mass_kg = cargo;
    w_cfg.fuel_mass_kg = fuel_fraction * 54.0; // RS6 tank capacity converted to kg
    WeightContribution weight_mod(w_cfg);
    WeightResult w_res = weight_mod.calculate();

    // --- Drag ---
    Drag drag_mod(speed, window_open == 1, roof_load == 1);

    // --- Temperature (Engine Thermals) ---
    Temperature temp_mod(ambient_temp);

    // --- Speed (Engine Dynamics) ---
    Speed speed_mod; 

    // Simulate engine running for 60 seconds to build up temps and average speed score
    for(int i = 0; i < 60; i++) {
        // Temperature uses the user's fixed RPM slider
        temp_mod.update(rpm, engine_load, speed, 1.0); 
        // Speed module uses throttle (engine_load) to simulate dynamic RPM and BSFC
        speed_mod.update(engine_load, 1.0); 
    }

    // --- Mileage Model (Route/Cost) ---
    // Inherit the fuel consumption calculated by the Weight module!
    double base_L_100km = w_res.fuel_consumption_L_per_100km;
    MileageModel mileage_mod(base_L_100km, fuel_price, distance);


    // 4. AGGREGATE SCORES
    double weight_score = w_res.efficiency_score;
    double drag_score = drag_mod.getEfficiencyScore();
    double engine_score = temp_mod.getEfficiency();
    
    // Combine Mileage (route/slope) and Speed (engine load/throttle) scores for the UI's "Route/Speed"
    double route_score = mileage_mod.averageScore(slope_angle);
    double dyn_speed_score = speed_mod.getAverageScore();
    double combined_speed_route_score = (route_score + dyn_speed_score) / 2.0;
    
   
    double tire_score = 100.0 - (speed * 0.15) - (w_res.added_mass_kg * 0.05);
    if (tire_score < 0) tire_score = 0;
    if (tire_score > 100) tire_score = 100;

    double overall_score = (weight_score + drag_score + engine_score + combined_speed_route_score + tire_score) / 5.0;


    // 5. JSON OUTPUT FOR PYTHON UI
    // Ensure this exactly matches the dictionary keys your Python script is looking for
    cout << fixed << setprecision(2);
    cout << "{\n";
    cout << "  \"scores\": {\n";
    cout << "    \"weight_points\": " << weight_score << ",\n";
    cout << "    \"aerodynamics_points\": " << drag_score << ",\n";
    cout << "    \"engine_points\": " << engine_score << ",\n";
    cout << "    \"speed_points\": " << combined_speed_route_score << ",\n";
    cout << "    \"tire_points\": " << tire_score << ",\n";
    cout << "    \"overall_points\": " << overall_score << "\n";
    cout << "  },\n";
    cout << "  \"weight_breakdown\": {\n";
    cout << "    \"total_mass_kg\": " << w_res.total_mass_kg << ",\n";
    cout << "    \"added_mass_kg\": " << w_res.added_mass_kg << ",\n";
    cout << "    \"rolling_resistance_N\": " << w_res.rolling_resistance_N << ",\n";
    cout << "    \"fuel_consumption_L_per_100km\": " << w_res.fuel_consumption_L_per_100km << ",\n";
    cout << "    \"drag_force_N\": " << drag_mod.getDragForce() << ",\n";
    cout << "    \"coolant_temp_c\": " << temp_mod.getCoolantTemp() << ",\n";
    cout << "    \"trip_cost\": " << mileage_mod.costFlat() << "\n";
    cout << "  }\n";
    cout << "}\n";

    // 6. LOG TO CSV
    ofstream csv_file("simulation_history.csv", ios::app);
    if (csv_file.is_open()) {
        csv_file << speed << "," << rpm << "," << overall_score << "," << w_res.fuel_consumption_L_per_100km << "\n";
        csv_file.close();
    }

    return 0;
}