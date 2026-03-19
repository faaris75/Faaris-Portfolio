#pragma once

class Temperature {
    private:
        double coolant; // based on environment 
        double oilTemp; // same
        bool waterPump = false;

        double ambientTemp; // set through constructor

        // constants
        const double TARGET_COOLANT_TEMP = 90.0;
        const double TARGET_OIL_TEMP = 105.0;
        const double THERMOSTAT_OPEN_TEMP = 80.0; 
        const double THERMAL_MASS_INVERSE = 0.0012; 
        const double RADIATOR_EFFICIENCY = 0.15; 
        const double PASSIVE_AIR_COOLING = 0.04; 
        const double OIL_COUPLING_FACTOR = 0.015;
        const double OIL_FACTOR = 0.6;
        const double COOLANT_FACTOR = 0.4;
    
    public:
        Temperature(double startTemp);
        void setAmbient(double temp);
        void update(int rpm, double load, double speed, double time);
        double getCoolantTemp();
        double getOilTemp();
        double getEfficiency();
};