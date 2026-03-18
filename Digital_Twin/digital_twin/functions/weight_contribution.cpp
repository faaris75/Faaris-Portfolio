#include "../classes/weight_contribution.h"
#include <algorithm> 

// Score formula: 100 × (floor − FC) / (floor − base), clamped to [0, 100]

WeightContribution::WeightContribution(const WeightConfig& config) : cfg_(config) {}

void WeightContribution::setConfig(const WeightConfig& config) { cfg_ = config; }
const WeightConfig& WeightContribution::getConfig() const { return cfg_; }

double WeightContribution::totalMass() const {
    return cfg_.curb_mass_kg + cfg_.passenger_mass_kg + cfg_.cargo_mass_kg + cfg_.fuel_mass_kg;
}

double WeightContribution::totalWeight()       const { return totalMass() * cfg_.gravity_mps2; }
double WeightContribution::rollingResistance() const { return cfg_.crr * totalWeight(); }
double WeightContribution::addedMass()         const { return cfg_.passenger_mass_kg + cfg_.cargo_mass_kg; }

double WeightContribution::fuelConsumptionDelta() const {
    return (addedMass() / 100.0) * cfg_.consumption_per_100kg;
}

double WeightContribution::fuelConsumption() const {
    return cfg_.base_consumption_L_per_100km + fuelConsumptionDelta();
}

double WeightContribution::efficiencyScore() const {
    const double base  = cfg_.base_consumption_L_per_100km;
    const double floor = cfg_.score_floor_consumption;
    if (floor <= base) return 100.0;
    return std::clamp(100.0 * (floor - fuelConsumption()) / (floor - base), 0.0, 100.0);
}

WeightResult WeightContribution::calculate() const {
    return {
        totalMass(),
        totalWeight(),
        rollingResistance(),
        addedMass(),
        fuelConsumption(),
        fuelConsumptionDelta(),
        efficiencyScore()
    };
}
