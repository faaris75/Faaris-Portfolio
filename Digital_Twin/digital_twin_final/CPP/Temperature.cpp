
#include <cmath>
#include "Temperature.h"

Temperature::Temperature(double startTemp) {
    ambientTemp = startTemp;
    oilTemp = startTemp + 15; // oil is hotter than coolant
    coolant = startTemp;
}

void Temperature::setAmbient(double temp) {
    ambientTemp = temp;
}
/**
 * Updates the thermal state of the engine.
 * @param rpm Engine speed in Revolutions Per Minute 
 * @param load Engine load (0.0 = Idle, 1.0 = Full Throttle)
 * @param speed Speed in km/h
 * @param dt Time interval
 */
void Temperature::update(int rpm, double load, double speed, double dt) {

        double heatPowerInput = (rpm * load) * 0.028; // explain constant
        double deltaHeat = heatPowerInput * THERMAL_MASS_INVERSE;

        if (coolant > THERMOSTAT_OPEN_TEMP) { // radiator turns on after 88C (show source)
            waterPump = true;
        } else if (coolant < THERMOSTAT_OPEN_TEMP) { // off if less than 88C
            waterPump = false;
        }

        double coolingPower = speed * PASSIVE_AIR_COOLING; // Passive cooling from vehicle movement

        if (waterPump) {
            double airflowMultiplier = 1.0 + (speed * 0.05); 
            coolingPower += (coolant - ambientTemp) * (RADIATOR_EFFICIENCY * airflowMultiplier);
        }

        // Net change = (Heat In - Heat Out) * Time Step
        coolant += (deltaHeat - (coolingPower * THERMAL_MASS_INVERSE)) * dt;
        

        oilTemp += (coolant + 15 - oilTemp) * OIL_COUPLING_FACTOR; // account for difference again
}

double Temperature::getCoolantTemp() {
    return coolant;
}

double Temperature::getOilTemp() {
    return oilTemp;
}

double Temperature::getEfficiency() {
    double coolantError = std::abs(coolant - TARGET_COOLANT_TEMP); // find deviation from ideal temp (90)
    double coolantScore = 100 - coolantError * 1.5; // 1.5% penalty per degree

    double oilError = std::abs(oilTemp - TARGET_OIL_TEMP); // deviation from 105
    double oilScore = 100 - oilError * 2.0; // 2% penalty per degree

    if(coolantScore < 0) {
        coolantScore = 0;
    }
                                // clamp scores
    if (oilScore < 0) {
        oilScore = 0;
    }

    return (coolantScore * COOLANT_FACTOR) + (oilScore * OIL_FACTOR); //coolant is weighted 40% and oil 60%
}
