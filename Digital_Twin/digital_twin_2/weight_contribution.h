

#pragma once 

// 2024 Audi RS6 Avant Performance (C8): 2090 kg curb, 12.2 L/100km combined, Crr 0.014

struct WeightConfig {
    double curb_mass_kg                 = 2090.0;
    double passenger_mass_kg            = 0.0;
    double cargo_mass_kg                = 0.0;
    double fuel_mass_kg                 = 53.29;   // full tank: ~73 L x 0.75 kg/L
    double crr                          = 0.014; //coeff of rr estimated to 0.014
    double gravity_mps2                 = 9.81;
    double base_consumption_L_per_100km = 12.2;
    double consumption_per_100kg        = 0.5;
    double score_floor_consumption      = 22.2;   // consumption at which score hits 0
};

struct WeightResult {
    double total_mass_kg;
    double total_weight_N;
    double rolling_resistance_N;
    double added_mass_kg;
    double fuel_consumption_L_per_100km;
    double fuel_consumption_delta;
    double efficiency_score;            // 0-100 (100 = factory baseline)
};

class WeightContribution {
public:
    explicit WeightContribution(const WeightConfig& config = WeightConfig{});

    void setConfig(const WeightConfig& config);
    const WeightConfig& getConfig() const;

    WeightResult calculate() const;

    double totalMass()            const;
    double totalWeight()          const;
    double rollingResistance()    const;
    double addedMass()            const;
    double fuelConsumption()      const;
    double fuelConsumptionDelta() const;
    double efficiencyScore()      const;

private:
    WeightConfig cfg_;
};
