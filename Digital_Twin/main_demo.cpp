#include <iostream>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <format>
#include <fstream>

#include "classes/weight_contribution.h"
#include "classes/Drag.h"
#include "classes/Speed.h"
#include "classes/Temperature.h"

#include "functions/Drag.cpp"
#include "functions/weight_contribution.cpp"
#include "functions/Temperature.cpp"

Speed car_speed;
const double totalTime = 100; // time of simulation in seconds;
const double dt = 0.1; // step increment in seconds

int main() {
    std::cout << "Fuel Efficiency Digital Twin - Audi RS6 Avant Performance\n\n";

    // User input
    double passengers_kg, cargo_kg, tank_fraction, ambient_temp, throttle;
    bool window{false}, roofLoad{false};
    char windowStatus;

    std::cout << "Enter total passenger mass (kg): ";
    std::cin >> passengers_kg;

    std::cout << "Enter cargo mass (kg): ";
    std::cin >> cargo_kg;

    if(cargo_kg > 0) {
        roofLoad = true;
    }

    std::cout << "Window open? (Y/N): ";
    std::cin >> windowStatus;

    if(std::toupper(windowStatus) == 'Y') {
        window = true;
    }

    std::cout << "Enter fuel level (0.0 = empty, 1.0 = full): ";
    std::cin >> tank_fraction;
    tank_fraction = std::clamp(tank_fraction, 0.0, 1.0);

    std::cout << "Enter throttle level (0.0 = no throttle, 1.0 = fully depressed): ";
    std::cin >> throttle;
    tank_fraction = std::clamp(throttle, 0.0, 1.0);

    std::cout << "Enter ambient temperature (C): ";
    std::cin >> ambient_temp;

    Temperature car_temp(85); // prevent a cold start
    car_temp.setAmbient(ambient_temp);

    Drag car_drag(window, roofLoad, ambient_temp);

    // RS6 tank: 73 L x 0.73 kg/L = 53.29 kg full
    double fuel_kg = tank_fraction * 53.29;

    // Weight
    WeightConfig cfg;
    cfg.passenger_mass_kg = passengers_kg;
    cfg.cargo_mass_kg = cargo_kg;
    cfg.fuel_mass_kg = fuel_kg;

    WeightContribution wc(cfg);
    WeightResult wr = wc.calculate();

    double currentTime{0}, dragScore{0}, tempScore{0}, speedScore{0}, weightScore{0};

    auto getSpeedEfficiency = [&](double drag, double resistance, double mass) {
        double idealAccel = car_speed.computeAccel(0, 0, cfg.curb_mass_kg);
        double actualAccel = car_speed.computeAccel(drag, resistance, mass);

        double score = actualAccel / idealAccel * 100;
        return std::clamp(score, 0.0, 100.0);
    };



    std::ofstream carTelemetry{"carTelemetry.csv"};
    carTelemetry << "Time (s),Speed (km/h),Torque (Nm),RPM,Gear,Drag (N),Coolant (C),Oil (C),RPM/s,Acceleration (m/s2)\n";

    std::cout << "\nGenerating car telemetry...\n\n";

    while (currentTime < totalTime) {

        double drag = car_drag.getDragForce();
        double accelRate = car_speed.computeAccel(drag, wr.rolling_resistance_N, wr.total_mass_kg);
        double rpm = car_speed.update(throttle, dt, accelRate);
        double torque = car_speed.getTorque();
        double speed = car_speed.getSpeed();
        double gear = car_speed.getGear();

        car_drag.updateSpeed(speed);
        
        car_temp.update(rpm, throttle, speed, dt); // updates states of variables
        double coolant = car_temp.getCoolantTemp();
        double oil = car_temp.getOilTemp();

        static int tick{0};
        tick++;
        if(tick >= 10) { // run every 1 second
            dragScore += car_drag.getEfficiencyScore();
            tempScore += car_temp.getEfficiency();
            speedScore += getSpeedEfficiency(drag, wr.rolling_resistance_N, wr.total_mass_kg);
            weightScore += wr.efficiency_score;
            tick = 0;
        }

        carTelemetry << currentTime << ","
                  << speed << ","
                  << torque << ","
                  << rpm << ","
                  << gear << "," 
                  << drag << ","
                  << coolant << ","
                  << oil << ","
                  << accelRate << ","
                  << car_speed.getAccel() << "\n";

        currentTime += dt;
    }

    std::cout << "Results saved to carTelemetry.csv\n\n";

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average Efficiency Scores:\n";
    std::cout << "Drag Score: " << dragScore / totalTime << std::endl;
    std::cout << "Temperature Score: " << tempScore / totalTime << std::endl;
    std::cout << "Speed Score: " << speedScore / totalTime << std::endl;
    std::cout << "Weight Score: " << weightScore / totalTime << std::endl;

    return 0;
}
