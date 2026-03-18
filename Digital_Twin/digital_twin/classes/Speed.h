#pragma once
#include <iomanip>
#include <algorithm>
#include <cmath> 

class Speed { 
    public: 
    Speed( double idleRpm = 800.0, double maxRpm = 7000.0, double accelRate = 3000.0, double decelRate = 2000.0 ) : idleRpm_(idleRpm), maxRpm_(maxRpm), accelRate_(accelRate), decelRate_(decelRate), currentRpm_(idleRpm), totalScore_(0.0), totalTime_(0.0) {} 

double update(double throttle, double dt) { 
   throttle = std::clamp(throttle, 0.0, 1.0); 
   dt = std::max(dt, 0.0); 
 
   const double targetRpm = idleRpm_ + throttle * (maxRpm_ - idleRpm_); 
 
   if (currentRpm_ < targetRpm) 
       currentRpm_ += accelRate_ * throttle * dt; 
   else 
       currentRpm_ -= decelRate_ * dt; 
 
   currentRpm_ = std::clamp(currentRpm_, idleRpm_, maxRpm_); 
 
   // ===== Fuel Model ===== 
   double torque = computeTorque(throttle, currentRpm_); 
   double fuelRate = computeFuelRate(currentRpm_, torque); 
   double score = computeEfficiencyScore(fuelRate); 
 
   totalScore_ += score * dt; 
   totalTime_ += dt; 
 
   lastFuelRate_ = fuelRate; 
   lastTorque_ = torque; 
   lastScore_ = score; 
 
   return currentRpm_; 
} 
 
double getFuelRate() const { return lastFuelRate_; } 
double getTorque() const { return lastTorque_; } 
double getInstantScore() const { return lastScore_; } 
 
double getAverageScore() const { 
   if (totalTime_ == 0.0) return 0.0; 
   return totalScore_ / totalTime_; 
} 
 

private: double idleRpm_; double maxRpm_; double accelRate_; double decelRate_; double currentRpm_; 

double totalScore_; 
double totalTime_; 
 
double lastFuelRate_ = 0.0; 
double lastTorque_ = 0.0; 
double lastScore_ = 0.0; 
 
// ============================ 
// TORQUE MODEL 
// ============================ 
double computeTorque(double throttle, double rpm) const { 
   double maxTorque = 280.0; // Nm typical 2.0L engine 
 
   // Peak torque near 4000 RPM 
   double shape = 1.0 - std::pow((rpm - 4000.0) / 4000.0, 2); 
   shape = std::clamp(shape, 0.25, 1.0); 
 
   // Make throttle slightly nonlinear (realistic airflow behavior) 
   double throttleEffect = std::pow(throttle, 1.2); 
 
   return maxTorque * shape * throttleEffect; 
} 
 
// ============================ 
// BSFC MODEL (g/kWh) 
// ============================ 
double computeBSFC(double rpm, double loadFactor) const { 
   double rpmPenalty = std::pow((rpm - 2500.0) / 2500.0, 2); 
   double loadPenalty = std::pow((loadFactor - 0.8), 2); 
 
   // Real gasoline engines: ~240–420 g/kWh 
   return 260.0 + 180.0 * (rpmPenalty + loadPenalty); 
} 
 
// ============================ 
// FUEL FLOW (g/s) 
// ============================ 
double computeFuelRate(double rpm, double torque) const { 
   double powerKW = (torque * rpm) / 9549.0; 
 
   double loadFactor = torque / 280.0; 
   double bsfc = computeBSFC(rpm, loadFactor); 
 
   double fuelFromPower = (bsfc * powerKW) / 3600.0; 
 
   // Add realistic idle fuel consumption 
   double idleFuel = 0.8; // g/s typical gasoline idle 
 
   return idleFuel + fuelFromPower; 
} 
 
// ============================ 
// SCORE CONVERSION (0–100) 
// ============================ 
double computeEfficiencyScore(double fuelRate) const { 
   // Much tighter realistic window 
   double idealFuelRate = 1.0;   // idle cruising 
   double worstFuelRate = 25.0;  // heavy acceleration 
 
   double normalized = 
       (worstFuelRate - fuelRate) / 
       (worstFuelRate - idealFuelRate); 
 
   return std::clamp(normalized, 0.0, 1.0) * 100.0; 
} 
 

}; 